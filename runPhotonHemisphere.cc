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
   treeWriter trw("jetTree");

   //Jet definition
   double R                   = 0.4;
   double ghostRapMax         = 6.0;
   double ghost_area          = 0.005;
   int    active_area_repeats = 1;
   GhostedAreaSpec ghost_spec(ghostRapMax, active_area_repeats, ghost_area);
   AreaDefinition area_def = AreaDefinition(active_area,ghost_spec);
   JetDefinition jet_def(antikt_algorithm, R);
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
      
      ClusterSequence

      //---------------------------------------------------------------------------
      //   Jet clustering
      //---------------------------------------------------------------------------

      ClusterSequenceArea ClusterSignal(ParticlesSignal, jet_def, area_def);
      jetCollection JetCollectionSignal(sorted_by_pt(jet_selector(ClusterSignal.inclusive_jets())));
      jetCollection JetCollectionSignalJewel(GetCorrectedJets(JetCollectionSignal.getJet(), ParticlesDummy));

      //---------------------------------------------------------------------------
      //   Groom the jets
      //---------------------------------------------------------------------------

      softDropGroomer SoftDropSignal(0.1, 0.0, R);
      jetCollection JetCollectionSignalSD(SoftDropSignal.doGrooming(JetCollectionSignal));
      JetCollectionSignalSD.addVector("SignalJetSDZG",      SoftDropSignal.getZgs());
      JetCollectionSignalSD.addVector("SignalJetSDNBranch", SoftDropSignal.getNDroppedSubjets());
      JetCollectionSignalSD.addVector("SignalJetSDDR12",    SoftDropSignal.getDR12());

      jetCollection JetCollectionSignalSDJewel(GetCorrectedJets(SoftDropSignal.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> SigSDJewel
         = GetCorrectedSubJets(SoftDropSignal.getConstituents1(), SoftDropSignal.getConstituents2(), ParticlesDummy);
      JetCollectionSignalSDJewel.addVector("SignalJetSDJewelZG",   CalculateZG(SigSDJewel));
      JetCollectionSignalSDJewel.addVector("SignalJetSDJewelDR12", CalculateDR(SigSDJewel));

      //---------------------------------------------------------------------------
      //   Write tree
      //---------------------------------------------------------------------------

      //Give variable we want to write out to treeWriter.
      //Only vectors of the types 'jetCollection', and 'double', 'int', 'PseudoJet' are supported

      trw.addCollection("SignalJet",        JetCollectionSignal);
      trw.addCollection("SignalJetJewel",   JetCollectionSignalJewel);
      trw.addCollection("SignalJetSD",      JetCollectionSignalSD);
      trw.addCollection("SignalJetSDJewel", JetCollectionSignalSDJewel);
      trw.addCollection("EventWeight",      EventWeight);

      trw.fillTree();

   }//event loop

   Bar.Update(nEvent);
   Bar.Print();
   Bar.PrintLine();

   TTree *trOut = trw.getTree();

   TFile *fout = new TFile(cmdline.value<string>("-output", "JetToyHIResulJewelSub.root").c_str(), "RECREATE");
   trOut->Write();
   fout->Write();
   fout->Close();

   double time_in_seconds = chrono::duration_cast<chrono::milliseconds>
      (chrono::steady_clock::now() - start_time).count() / 1000.0;
   cout << "runFromFile: " << time_in_seconds << endl;
}
