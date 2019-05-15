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
   
   int EventCount = cmdline.value<int>("-nev", 1);
   //bool verbose = cmdline.present("-verbose");

   cout << "will run on " << EventCount << " events" << endl;

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

   ProgressBar Bar(cout, EventCount);
   Bar.SetStyle(-1);

   EventMixer mixer(&cmdline);  //the mixing machinery from PU14 workshop

   // loop over events
   int iev = 0;
   unsigned int entryDiv = (EventCount > 200) ? EventCount / 200 : 1;
   while ( mixer.next_event() && iev < EventCount )
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

      ClusterSequenceArea ClusterSignal02(ParticlesSignal, JetDefinition02, area_def);
      jetCollection JCSignal02(sorted_by_pt(jet_selector(ClusterSignal02.inclusive_jets())));
      jetCollection JCSignal02Jewel(GetCorrectedJets(JCSignal02.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterSignal04(ParticlesSignal, JetDefinition04, area_def);
      jetCollection JCSignal04(sorted_by_pt(jet_selector(ClusterSignal04.inclusive_jets())));
      jetCollection JCSignal04Jewel(GetCorrectedJets(JCSignal04.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterSignal06(ParticlesSignal, JetDefinition06, area_def);
      jetCollection JCSignal06(sorted_by_pt(jet_selector(ClusterSignal06.inclusive_jets())));
      jetCollection JCSignal06Jewel(GetCorrectedJets(JCSignal06.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterSignal08(ParticlesSignal, JetDefinition08, area_def);
      jetCollection JCSignal08(sorted_by_pt(jet_selector(ClusterSignal08.inclusive_jets())));
      jetCollection JCSignal08Jewel(GetCorrectedJets(JCSignal08.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterSignal10(ParticlesSignal, JetDefinition10, area_def);
      jetCollection JCSignal10(sorted_by_pt(jet_selector(ClusterSignal10.inclusive_jets())));
      jetCollection JCSignal10Jewel(GetCorrectedJets(JCSignal10.getJet(), ParticlesDummy));
      
      ClusterSequenceArea ClusterAll02(ParticlesReal, JetDefinition02, area_def);
      jetCollection JCAll02(sorted_by_pt(jet_selector(ClusterAll02.inclusive_jets())));
      jetCollection JCAll02Jewel(GetCorrectedJets(JCAll02.getJet(), ParticlesDummy));
      
      ClusterSequenceArea ClusterAll04(ParticlesReal, JetDefinition04, area_def);
      jetCollection JCAll04(sorted_by_pt(jet_selector(ClusterAll04.inclusive_jets())));
      jetCollection JCAll04Jewel(GetCorrectedJets(JCAll04.getJet(), ParticlesDummy));
      
      ClusterSequenceArea ClusterAll06(ParticlesReal, JetDefinition06, area_def);
      jetCollection JCAll06(sorted_by_pt(jet_selector(ClusterAll06.inclusive_jets())));
      jetCollection JCAll06Jewel(GetCorrectedJets(JCAll06.getJet(), ParticlesDummy));
      
      ClusterSequenceArea ClusterAll08(ParticlesReal, JetDefinition08, area_def);
      jetCollection JCAll08(sorted_by_pt(jet_selector(ClusterAll08.inclusive_jets())));
      jetCollection JCAll08Jewel(GetCorrectedJets(JCAll08.getJet(), ParticlesDummy));
      
      ClusterSequenceArea ClusterAll10(ParticlesReal, JetDefinition10, area_def);
      jetCollection JCAll10(sorted_by_pt(jet_selector(ClusterAll10.inclusive_jets())));
      jetCollection JCAll10Jewel(GetCorrectedJets(JCAll10.getJet(), ParticlesDummy));

      //---------------------------------------------------------------------------
      //   Groom the jets
      //---------------------------------------------------------------------------

      softDropGroomer SDSignal02(0.1, 0.0, 0.2);
      softDropGroomer SDSignal04(0.1, 0.0, 0.4);
      softDropGroomer SDSignal06(0.1, 0.0, 0.6);
      softDropGroomer SDSignal08(0.1, 0.0, 0.8);
      softDropGroomer SDSignal10(0.1, 0.0, 1.0);
      softDropGroomer SDAll02(0.1, 0.0, 0.2);
      softDropGroomer SDAll04(0.1, 0.0, 0.4);
      softDropGroomer SDAll06(0.1, 0.0, 0.6);
      softDropGroomer SDAll08(0.1, 0.0, 0.8);
      softDropGroomer SDAll10(0.1, 0.0, 1.0);
      
      jetCollection JCSignal02SD(SDSignal02.doGrooming(JCSignal02));
      JCSignal02SD.addVector("SignalJet02SDZG",      SDSignal02.getZgs());
      JCSignal02SD.addVector("SignalJet02SDNBranch", SDSignal02.getNDroppedSubjets());
      JCSignal02SD.addVector("SignalJet02SDDR12",    SDSignal02.getDR12());
      jetCollection JCSignal04SD(SDSignal04.doGrooming(JCSignal04));
      JCSignal04SD.addVector("SignalJet04SDZG",      SDSignal04.getZgs());
      JCSignal04SD.addVector("SignalJet04SDNBranch", SDSignal04.getNDroppedSubjets());
      JCSignal04SD.addVector("SignalJet04SDDR12",    SDSignal04.getDR12());
      jetCollection JCSignal06SD(SDSignal06.doGrooming(JCSignal06));
      JCSignal06SD.addVector("SignalJet06SDZG",      SDSignal06.getZgs());
      JCSignal06SD.addVector("SignalJet06SDNBranch", SDSignal06.getNDroppedSubjets());
      JCSignal06SD.addVector("SignalJet06SDDR12",    SDSignal06.getDR12());
      jetCollection JCSignal08SD(SDSignal08.doGrooming(JCSignal08));
      JCSignal08SD.addVector("SignalJet08SDZG",      SDSignal08.getZgs());
      JCSignal08SD.addVector("SignalJet08SDNBranch", SDSignal08.getNDroppedSubjets());
      JCSignal08SD.addVector("SignalJet08SDDR12",    SDSignal08.getDR12());
      jetCollection JCSignal10SD(SDSignal10.doGrooming(JCSignal10));
      JCSignal10SD.addVector("SignalJet10SDZG",      SDSignal10.getZgs());
      JCSignal10SD.addVector("SignalJet10SDNBranch", SDSignal10.getNDroppedSubjets());
      JCSignal10SD.addVector("SignalJet10SDDR12",    SDSignal10.getDR12());

      jetCollection JCSignal02SDJewel(GetCorrectedJets(SDSignal02.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal02SDJewel
         = GetCorrectedSubJets(SDSignal02.getConstituents1(), SDSignal02.getConstituents2(), ParticlesDummy);
      JCSignal02SDJewel.addVector("SignalJet02SDJewelZG",   CalculateZG(Signal02SDJewel));
      JCSignal02SDJewel.addVector("SignalJet02SDJewelDR12", CalculateDR(Signal02SDJewel));
      jetCollection JCSignal04SDJewel(GetCorrectedJets(SDSignal04.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal04SDJewel
         = GetCorrectedSubJets(SDSignal04.getConstituents1(), SDSignal04.getConstituents2(), ParticlesDummy);
      JCSignal04SDJewel.addVector("SignalJet04SDJewelZG",   CalculateZG(Signal04SDJewel));
      JCSignal04SDJewel.addVector("SignalJet04SDJewelDR12", CalculateDR(Signal04SDJewel));
      jetCollection JCSignal06SDJewel(GetCorrectedJets(SDSignal06.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal06SDJewel
         = GetCorrectedSubJets(SDSignal06.getConstituents1(), SDSignal06.getConstituents2(), ParticlesDummy);
      JCSignal06SDJewel.addVector("SignalJet06SDJewelZG",   CalculateZG(Signal06SDJewel));
      JCSignal06SDJewel.addVector("SignalJet06SDJewelDR12", CalculateDR(Signal06SDJewel));
      jetCollection JCSignal08SDJewel(GetCorrectedJets(SDSignal08.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal08SDJewel
         = GetCorrectedSubJets(SDSignal08.getConstituents1(), SDSignal08.getConstituents2(), ParticlesDummy);
      JCSignal08SDJewel.addVector("SignalJet08SDJewelZG",   CalculateZG(Signal08SDJewel));
      JCSignal08SDJewel.addVector("SignalJet08SDJewelDR12", CalculateDR(Signal08SDJewel));
      jetCollection JCSignal10SDJewel(GetCorrectedJets(SDSignal10.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal10SDJewel
         = GetCorrectedSubJets(SDSignal10.getConstituents1(), SDSignal10.getConstituents2(), ParticlesDummy);
      JCSignal10SDJewel.addVector("SignalJet10SDJewelZG",   CalculateZG(Signal10SDJewel));
      JCSignal10SDJewel.addVector("SignalJet10SDJewelDR12", CalculateDR(Signal10SDJewel));

      jetCollection JCAll02SD(SDAll02.doGrooming(JCAll02));
      JCAll02SD.addVector("AllJet02SDZG",      SDAll02.getZgs());
      JCAll02SD.addVector("AllJet02SDNBranch", SDAll02.getNDroppedSubjets());
      JCAll02SD.addVector("AllJet02SDDR12",    SDAll02.getDR12());
      jetCollection JCAll04SD(SDAll04.doGrooming(JCAll04));
      JCAll04SD.addVector("AllJet04SDZG",      SDAll04.getZgs());
      JCAll04SD.addVector("AllJet04SDNBranch", SDAll04.getNDroppedSubjets());
      JCAll04SD.addVector("AllJet04SDDR12",    SDAll04.getDR12());
      jetCollection JCAll06SD(SDAll06.doGrooming(JCAll06));
      JCAll06SD.addVector("AllJet06SDZG",      SDAll06.getZgs());
      JCAll06SD.addVector("AllJet06SDNBranch", SDAll06.getNDroppedSubjets());
      JCAll06SD.addVector("AllJet06SDDR12",    SDAll06.getDR12());
      jetCollection JCAll08SD(SDAll08.doGrooming(JCAll08));
      JCAll08SD.addVector("AllJet08SDZG",      SDAll08.getZgs());
      JCAll08SD.addVector("AllJet08SDNBranch", SDAll08.getNDroppedSubjets());
      JCAll08SD.addVector("AllJet08SDDR12",    SDAll08.getDR12());
      jetCollection JCAll10SD(SDAll10.doGrooming(JCAll10));
      JCAll10SD.addVector("AllJet10SDZG",      SDAll10.getZgs());
      JCAll10SD.addVector("AllJet10SDNBranch", SDAll10.getNDroppedSubjets());
      JCAll10SD.addVector("AllJet10SDDR12",    SDAll10.getDR12());

      jetCollection JCAll02SDJewel(GetCorrectedJets(SDAll02.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All02SDJewel
         = GetCorrectedSubJets(SDAll02.getConstituents1(), SDAll02.getConstituents2(), ParticlesDummy);
      JCAll02SDJewel.addVector("AllJet02SDJewelZG",   CalculateZG(All02SDJewel));
      JCAll02SDJewel.addVector("AllJet02SDJewelDR12", CalculateDR(All02SDJewel));
      jetCollection JCAll04SDJewel(GetCorrectedJets(SDAll04.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All04SDJewel
         = GetCorrectedSubJets(SDAll04.getConstituents1(), SDAll04.getConstituents2(), ParticlesDummy);
      JCAll04SDJewel.addVector("AllJet04SDJewelZG",   CalculateZG(All04SDJewel));
      JCAll04SDJewel.addVector("AllJet04SDJewelDR12", CalculateDR(All04SDJewel));
      jetCollection JCAll06SDJewel(GetCorrectedJets(SDAll06.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All06SDJewel
         = GetCorrectedSubJets(SDAll06.getConstituents1(), SDAll06.getConstituents2(), ParticlesDummy);
      JCAll06SDJewel.addVector("AllJet06SDJewelZG",   CalculateZG(All06SDJewel));
      JCAll06SDJewel.addVector("AllJet06SDJewelDR12", CalculateDR(All06SDJewel));
      jetCollection JCAll08SDJewel(GetCorrectedJets(SDAll08.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All08SDJewel
         = GetCorrectedSubJets(SDAll08.getConstituents1(), SDAll08.getConstituents2(), ParticlesDummy);
      JCAll08SDJewel.addVector("AllJet08SDJewelZG",   CalculateZG(All08SDJewel));
      JCAll08SDJewel.addVector("AllJet08SDJewelDR12", CalculateDR(All08SDJewel));
      jetCollection JCAll10SDJewel(GetCorrectedJets(SDAll10.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All10SDJewel
         = GetCorrectedSubJets(SDAll10.getConstituents1(), SDAll10.getConstituents2(), ParticlesDummy);
      JCAll10SDJewel.addVector("AllJet10SDJewelZG",   CalculateZG(All10SDJewel));
      JCAll10SDJewel.addVector("AllJet10SDJewelDR12", CalculateDR(All10SDJewel));

      //---------------------------------------------------------------------------
      //   Write tree
      //---------------------------------------------------------------------------

      // Give variable we want to write out to treeWriter.
      // Only vectors of the types 'jetCollection', and 'double', 'int', 'PseudoJet' are supported

      Writer.addCollection("SignalJet02",        JCSignal02);
      Writer.addCollection("SignalJet02Jewel",   JCSignal02Jewel);
      Writer.addCollection("SignalJet02SD",      JCSignal02SD);
      Writer.addCollection("SignalJet02SDJewel", JCSignal02SDJewel);
      Writer.addCollection("SignalJet04",        JCSignal04);
      Writer.addCollection("SignalJet04Jewel",   JCSignal04Jewel);
      Writer.addCollection("SignalJet04SD",      JCSignal04SD);
      Writer.addCollection("SignalJet04SDJewel", JCSignal04SDJewel);
      Writer.addCollection("SignalJet06",        JCSignal06);
      Writer.addCollection("SignalJet06Jewel",   JCSignal06Jewel);
      Writer.addCollection("SignalJet06SD",      JCSignal06SD);
      Writer.addCollection("SignalJet06SDJewel", JCSignal06SDJewel);
      Writer.addCollection("SignalJet08",        JCSignal08);
      Writer.addCollection("SignalJet08Jewel",   JCSignal08Jewel);
      Writer.addCollection("SignalJet08SD",      JCSignal08SD);
      Writer.addCollection("SignalJet08SDJewel", JCSignal08SDJewel);
      Writer.addCollection("SignalJet10",        JCSignal10);
      Writer.addCollection("SignalJet10Jewel",   JCSignal10Jewel);
      Writer.addCollection("SignalJet10SD",      JCSignal10SD);
      Writer.addCollection("SignalJet10SDJewel", JCSignal10SDJewel);
      
      Writer.addCollection("AllJet02",           JCAll02);
      Writer.addCollection("AllJet02Jewel",      JCAll02Jewel);
      Writer.addCollection("AllJet02SD",         JCAll02SD);
      Writer.addCollection("AllJet02SDJewel",    JCAll02SDJewel);
      Writer.addCollection("AllJet04",           JCAll04);
      Writer.addCollection("AllJet04Jewel",      JCAll04Jewel);
      Writer.addCollection("AllJet04SD",         JCAll04SD);
      Writer.addCollection("AllJet04SDJewel",    JCAll04SDJewel);
      Writer.addCollection("AllJet06",           JCAll06);
      Writer.addCollection("AllJet06Jewel",      JCAll06Jewel);
      Writer.addCollection("AllJet06SD",         JCAll06SD);
      Writer.addCollection("AllJet06SDJewel",    JCAll06SDJewel);
      Writer.addCollection("AllJet08",           JCAll08);
      Writer.addCollection("AllJet08Jewel",      JCAll08Jewel);
      Writer.addCollection("AllJet08SD",         JCAll08SD);
      Writer.addCollection("AllJet08SDJewel",    JCAll08SDJewel);
      Writer.addCollection("AllJet10",           JCAll10);
      Writer.addCollection("AllJet10Jewel",      JCAll10Jewel);
      Writer.addCollection("AllJet10SD",         JCAll10SD);
      Writer.addCollection("AllJet10SDJewel",    JCAll10SDJewel);

      Writer.addCollection("HemisphereSignal", HemisphereJetSignal);
      Writer.addCollection("HemisphereAll",    HemisphereJetAll);

      Writer.addCollection("EventWeight",      EventWeight);

      Writer.fillTree();

   } //event loop

   Bar.Update(EventCount);
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
