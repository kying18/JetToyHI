#include <iostream>
#include <chrono>

#include "TFile.h"
#include "TTree.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"

#include "include/ProgressBar.h"

#include "PU14/EventMixer.hh"
#include "PU14/CmdLine.hh"
#include "PU14/PU14.hh"

#include "include/extraInfo.hh"
#include "include/jetCollection.hh"
#include "include/csSubtractor.hh"
#include "include/csSubtractorFullEvent.hh"
#include "include/skSubtractor.hh"
#include "include/softDropGroomer.hh"
#include "include/treeWriter.hh"
#include "include/jetMatcher.hh"
#include "include/randomCones.hh"
#include "include/Angularity.hh"
#include "include/jewelMatcher.hh"

using namespace std;
using namespace fastjet;

//./runJewelSub -hard  /eos/project/j/jetquenching/JetWorkshop2017/samples/jewel/DiJet/RecoilOn_0_10/Jewel_0_T_0.pu14 -nev 10

int main (int argc, char *argv[])
{
   auto start_time = chrono::steady_clock::now();

   CmdLine cmdline(argc,argv);
   // inputs read from command line
   int nEvent = cmdline.value<int>("-nev", 1);
   //bool verbose = cmdline.present("-verbose");

   cout << "will run on " << nEvent << " events" << endl;

   // Uncomment to silence fastjet banner
   ClusterSequence::set_fastjet_banner_stream(NULL);

   //to write info to root tree
   treeWriter Writer("JetTree");

   //Jet definition
   double R                   = 0.4;
   double ghostRapMax         = 6.0;
   double ghost_area          = 0.005;
   int    active_area_repeats = 1;
   GhostedAreaSpec ghost_spec(ghostRapMax, active_area_repeats, ghost_area);
   AreaDefinition area_def = AreaDefinition(active_area,ghost_spec);
   JetDefinition JetDefinition02(antikt_algorithm, 0.2);
   JetDefinition JetDefinition04(antikt_algorithm, 0.4);
   JetDefinition JetDefinition06(antikt_algorithm, 0.6);
   JetDefinition JetDefinition08(antikt_algorithm, 0.8);
   JetDefinition JetDefinition10(antikt_algorithm, 1.0);
   JetDefinition WTADefinition(antikt_algorithm, 10.0, WTA_pt_scheme);

   double jetRapMax = 3.0;
   Selector jet_selector = SelectorAbsRapMax(jetRapMax);

   Angularity width(1.,1.,R);
   Angularity pTD(0.,2.,R);

   ProgressBar Bar(cout, nEvent);
   Bar.SetStyle(-1);

   EventMixer mixer(&cmdline);  //the mixing machinery from PU14 workshop

   // loop over events
   int iev = 0;
   unsigned int entryDiv = (nEvent > 200) ? nEvent / 200 : 1;
   while ( mixer.next_event() && iev < nEvent )
   {
      // increment event number    
      iev++;

      Bar.Update(iev);
      Bar.PrintWithMod(entryDiv);

      vector<PseudoJet> ParticlesMerged = mixer.particles();

      vector<double> EventWeight;
      EventWeight.push_back(mixer.hard_weight());
      EventWeight.push_back(mixer.pu_weight());

      //---------------------------------------------------------------------------
      //   sort out particles
      //---------------------------------------------------------------------------

      vector<PseudoJet> ParticlesDummy, ParticlesReal;
      vector<PseudoJet> ParticlesBackground, ParticlesSignal;
      SelectorVertexNumber(-1).sift(ParticlesMerged, ParticlesDummy, ParticlesReal);
      SelectorVertexNumber(0).sift(ParticlesReal, ParticlesSignal, ParticlesBackground);

      for(int i = 0; i < (int)ParticlesDummy.size(); i++)
      {
         if(ParticlesDummy[i].perp() < 1e-5 && fabs(ParticlesDummy[i].pz()) > 2000)
         {
            ParticlesDummy.erase(ParticlesDummy.begin() + i);
            i = i - 1;
         }
      }

      //---------------------------------------------------------------------------
      //   find leading photon
      //---------------------------------------------------------------------------

      int PhotonIndex = -1;
      for(int i = 0; i < (int)ParticlesReal.size(); i++)
      {
         int ID = ParticlesReal[i].user_info<extraInfo>().pdg_id();

         if(ID != 22)
            continue;

         double AbsEta = fabs(ParticlesReal[i].eta());
         if(AbsEta > 2.5)
            continue;

         if(PhotonIndex < 0 || ParticlesReal[PhotonIndex].perp() < ParticlesReal[i].perp())
            PhotonIndex = i;
      }

      //---------------------------------------------------------------------------
      //   opposite hemisphere selection
      //---------------------------------------------------------------------------

      vector<PseudoJet> HemisphereSignal, HemisphereAll;
      if(PhotonIndex >= 0)
      {
         double PhotonPhi = ParticlesReal[PhotonIndex].phi();
         for(int i = 0; i < (int)ParticlesReal.size(); i++)
         {
            double ParticlePhi = ParticlesReal[i].phi();
            double DPhi = PhotonPhi - ParticlePhi;

            if(DPhi < -M_PI)   DPhi = DPhi + 2 * M_PI;
            if(DPhi > +M_PI)   DPhi = DPhi - 2 * M_PI;

            if(DPhi > -M_PI / 2 && DPhi < M_PI / 2)
               continue;

            HemisphereAll.push_back(ParticlesReal[i]);
         }
         for(int i = 0; i < (int)ParticlesSignal.size(); i++)
         {
            double ParticlePhi = ParticlesSignal[i].phi();
            double DPhi = PhotonPhi - ParticlePhi;

            if(DPhi < -M_PI)   DPhi = DPhi + 2 * M_PI;
            if(DPhi > +M_PI)   DPhi = DPhi - 2 * M_PI;

            if(DPhi > -M_PI / 2 && DPhi < M_PI / 2)
               continue;

            HemisphereSignal.push_back(ParticlesSignal[i]);
         }
      }

      //---------------------------------------------------------------------------
      //   Hemisphere clustering
      //---------------------------------------------------------------------------
      
      ClusterSequence HemisphereSignalCluster(HemisphereSignal, WTADefinition);
      jetCollection HemisphereJetSignal(HemisphereSignalCluster.exclusive_jets(1));
      
      ClusterSequence HemisphereAllCluster(HemisphereAll, WTADefinition);
      jetCollection HemisphereJetAll(HemisphereAllCluster.exclusive_jets(1));

      //---------------------------------------------------------------------------
      //   Jet clustering
      //---------------------------------------------------------------------------

      ClusterSequenceArea ClusterSignal04(ParticlesSignal, JetDefinition04, area_def);
      jetCollection JCSignal04(sorted_by_pt(jet_selector(ClusterSignal04.inclusive_jets())));
      jetCollection JCSignal04Jewel(GetCorrectedJets(JCSignal04.getJet(), ParticlesDummy));
      
      ClusterSequenceArea ClusterAll04(ParticlesReal, JetDefinition04, area_def);
      jetCollection JCAll04(sorted_by_pt(jet_selector(ClusterAll04.inclusive_jets())));
      jetCollection JCAll04Jewel(GetCorrectedJets(JCAll04.getJet(), ParticlesDummy));

      //---------------------------------------------------------------------------
      //   Groom the jets
      //---------------------------------------------------------------------------

      softDropGroomer SDSignal04(0.1, 0.0, R);
      jetCollection JCSignal04SD(SDSignal04.doGrooming(JCSignal04));
      JCSignal04SD.addVector("SignalJet04SDZG",      SDSignal04.getZgs());
      JCSignal04SD.addVector("SignalJet04SDNBranch", SDSignal04.getNDroppedSubjets());
      JCSignal04SD.addVector("SignalJet04SDDR12",    SDSignal04.getDR12());

      jetCollection JCSignal04SDJewel(GetCorrectedJets(SDSignal04.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal04SDJewel
         = GetCorrectedSubJets(SDSignal04.getConstituents1(), SDSignal04.getConstituents2(), ParticlesDummy);
      JCSignal04SDJewel.addVector("SignalJet04SDJewelZG",   CalculateZG(Signal04SDJewel));
      JCSignal04SDJewel.addVector("SignalJet04SDJewelDR12", CalculateDR(Signal04SDJewel));

      softDropGroomer SDAll04(0.1, 0.0, R);
      jetCollection JCAll04SD(SDAll04.doGrooming(JCAll04));
      JCAll04SD.addVector("AllJet04SDZG",      SDAll04.getZgs());
      JCAll04SD.addVector("AllJet04SDNBranch", SDAll04.getNDroppedSubjets());
      JCAll04SD.addVector("AllJet04SDDR12",    SDAll04.getDR12());

      jetCollection JCAll04SDJewel(GetCorrectedJets(SDAll04.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All04SDJewel
         = GetCorrectedSubJets(SDAll04.getConstituents1(), SDAll04.getConstituents2(), ParticlesDummy);
      JCAll04SDJewel.addVector("AllJet04SDJewelZG",   CalculateZG(All04SDJewel));
      JCAll04SDJewel.addVector("AllJet04SDJewelDR12", CalculateDR(All04SDJewel));

      //---------------------------------------------------------------------------
      //   Write tree
      //---------------------------------------------------------------------------

      // Give variable we want to write out to treeWriter.
      // Only vectors of the types 'jetCollection', and 'double', 'int', 'PseudoJet' are supported

      Writer.addCollection("SignalJet04",        JCSignal04);
      Writer.addCollection("SignalJet04Jewel",   JCSignal04Jewel);
      Writer.addCollection("SignalJet04SD",      JCSignal04SD);
      Writer.addCollection("SignalJet04SDJewel", JCSignal04SDJewel);
      
      Writer.addCollection("AllJet04",           JCAll04);
      Writer.addCollection("AllJet04Jewel",      JCAll04Jewel);
      Writer.addCollection("AllJet04SD",         JCAll04SD);
      Writer.addCollection("AllJet04SDJewel",    JCAll04SDJewel);

      Writer.addCollection("HemisphereSignal", HemisphereJetSignal);
      Writer.addCollection("HemisphereAll",    HemisphereJetAll);

      Writer.addCollection("EventWeight",      EventWeight);

      Writer.fillTree();

   } //event loop

   Bar.Update(nEvent);
   Bar.Print();
   Bar.PrintLine();

   TTree *trOut = Writer.getTree();

   TFile *fout = new TFile(cmdline.value<string>("-output", "JetToyHIResulJewelSub.root").c_str(), "RECREATE");
   trOut->Write();
   fout->Write();
   fout->Close();

   double time_in_seconds = chrono::duration_cast<chrono::milliseconds>
      (chrono::steady_clock::now() - start_time).count() / 1000.0;
   cout << "runFromFile: " << time_in_seconds << endl;
}
