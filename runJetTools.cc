#include <iostream>
#include <chrono>
#include <map>

#include "TFile.h"
#include "TTree.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"

#include "include/ProgressBar.h"

#include "PU14/EventMixer.hh"
#include "PU14/CmdLine.hh"
#include "PU14/PU14.hh"
#include "PU14/HepPID/ParticleIDMethods.hh"

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

// Observables
//    1. Leading hadrons and photons
//    2. DPhi trigger object and recoil jets (leading and subleading, and all)
//    3. DPhi between different recoil jet axes
//    4. WTA on constituents or WTA on everything?
//    5. WTA vs smaller jets, two AK4 subjets

using namespace std;
using namespace fastjet;

// ./runJewelSub -hard  /eos/project/j/jetquenching/JetWorkshop2017/samples/jewel/DiJet/RecoilOn_0_10/Jewel_0_T_0.pu14 -pileup XXXXX -nev 10

int main(int argc, char *argv[]);
bool CompareJet(PseudoJet &J1, PseudoJet &J2);
void DoJet(treeWriter &Writer, JetDefinition &Definition, AreaDefinition Area,
   JetDefinition &SubjetDefinition, JetDefinition &WTADefinition, double R,
   vector<PseudoJet> &Particles, vector<PseudoJet> &Dummy,
   Selector &JetSelector, string Tag);

int main(int argc, char *argv[])
{
   auto start_time = chrono::steady_clock::now();

   CmdLine cmdline(argc, argv);

   int EventCount = cmdline.value<int>("-nev", 1);
   //bool verbose = cmdline.present("-verbose");

   cout << "will run on " << EventCount << " events" << endl;

   // Uncomment to silence fastjet banner
   ClusterSequence::set_fastjet_banner_stream(NULL);

   //to write info to root tree
   treeWriter Writer("JetTree");

   //Jet definition
   double ghostRapMax         = 6.0;
   double ghost_area          = 0.005;
   int    active_area_repeats = 1;
   GhostedAreaSpec ghost_spec(ghostRapMax, active_area_repeats, ghost_area);
   AreaDefinition Area = AreaDefinition(active_area, ghost_spec);
   JetDefinition JetDefinition01(antikt_algorithm, 0.1);
   JetDefinition JetDefinition02(antikt_algorithm, 0.2);
   JetDefinition JetDefinition04(antikt_algorithm, 0.4);
   JetDefinition JetDefinition06(antikt_algorithm, 0.6);
   JetDefinition JetDefinition08(antikt_algorithm, 0.8);
   JetDefinition JetDefinition10(antikt_algorithm, 1.0);
   JetDefinition WTADefinition(antikt_algorithm, 10.0, WTA_pt_scheme);
   JetDefinition WTADefinition02(antikt_algorithm, 0.2, WTA_pt_scheme);
   JetDefinition WTADefinition04(antikt_algorithm, 0.4, WTA_pt_scheme);
   JetDefinition WTADefinition06(antikt_algorithm, 0.6, WTA_pt_scheme);
   JetDefinition WTADefinition08(antikt_algorithm, 0.8, WTA_pt_scheme);
   JetDefinition WTADefinition10(antikt_algorithm, 1.0, WTA_pt_scheme);

   Selector JetSelector = SelectorAbsRapMax(3.0);

   // Angularity width(1.,1.,R);
   // Angularity pTD(0.,2.,R);

   ProgressBar Bar(cout, EventCount);
   Bar.SetStyle(-1);

   EventMixer mixer(&cmdline);  //the mixing machinery from PU14 workshop

   // loop over events
   int iEvent = 0;
   unsigned int EntryDiv = (EventCount > 200) ? EventCount / 200 : 1;
   while ( mixer.next_event() && iEvent < EventCount )
   {
      // increment event number
      iEvent = iEvent + 1;

      Bar.Update(iEvent);
      Bar.PrintWithMod(EntryDiv);

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
      //   find leading hadrons
      //---------------------------------------------------------------------------

      vector<PseudoJet> LeadingHadron;
      for(int i = 0; i < (int)ParticlesReal.size(); i++)
      {
         const int &ID = ParticlesReal[i].user_info<PU14>().pdg_id();
         if(HepPID::isHadron(ID) == false)
            continue;
         if(ParticlesReal[i].eta() < -3 || ParticlesReal[i].eta() > 3)
            continue;
         LeadingHadron.push_back(ParticlesReal[i]);
      }
      sort(LeadingHadron.begin(), LeadingHadron.end(), CompareJet);
      if(LeadingHadron.size() > 5)
         LeadingHadron.erase(LeadingHadron.begin() + 5, LeadingHadron.end());

      //---------------------------------------------------------------------------
      //   find leading photon
      //---------------------------------------------------------------------------

      int PhotonIndex = -1;
      for(int i = 0; i < (int)ParticlesSignal.size(); i++)
      {
         const int &ID = ParticlesSignal[i].user_info<PU14>().pdg_id();

         if(ID != 22)
            continue;

         double AbsEta = fabs(ParticlesSignal[i].eta());
         if(AbsEta > 2.5)
            continue;

         if(PhotonIndex < 0 || ParticlesSignal[PhotonIndex].perp() < ParticlesSignal[i].perp())
            PhotonIndex = i;
      }

      vector<PseudoJet> LeadingPhoton;
      if(PhotonIndex >= 0)
         LeadingPhoton.push_back(ParticlesSignal[PhotonIndex]);

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

      DoJet(Writer, JetDefinition02, Area, JetDefinition01, WTADefinition, 0.2, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet02");
      DoJet(Writer, JetDefinition04, Area, JetDefinition01, WTADefinition, 0.4, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet04");
      DoJet(Writer, JetDefinition06, Area, JetDefinition01, WTADefinition, 0.6, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet06");
      DoJet(Writer, JetDefinition08, Area, JetDefinition01, WTADefinition, 0.8, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet08");
      DoJet(Writer, JetDefinition10, Area, JetDefinition01, WTADefinition, 1.0, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet10");
      
      DoJet(Writer, JetDefinition02, Area, JetDefinition01, WTADefinition, 0.2, ParticlesReal, ParticlesDummy, JetSelector, "AllJet02");
      DoJet(Writer, JetDefinition04, Area, JetDefinition01, WTADefinition, 0.4, ParticlesReal, ParticlesDummy, JetSelector, "AllJet04");
      DoJet(Writer, JetDefinition06, Area, JetDefinition01, WTADefinition, 0.6, ParticlesReal, ParticlesDummy, JetSelector, "AllJet06");
      DoJet(Writer, JetDefinition08, Area, JetDefinition01, WTADefinition, 0.8, ParticlesReal, ParticlesDummy, JetSelector, "AllJet08");
      DoJet(Writer, JetDefinition10, Area, JetDefinition01, WTADefinition, 1.0, ParticlesReal, ParticlesDummy, JetSelector, "AllJet10");

      DoJet(Writer, WTADefinition02, Area, JetDefinition01, WTADefinition, 0.2, ParticlesSignal, ParticlesDummy, JetSelector, "WTASignalJet02");
      DoJet(Writer, WTADefinition04, Area, JetDefinition01, WTADefinition, 0.4, ParticlesSignal, ParticlesDummy, JetSelector, "WTASignalJet04");
      DoJet(Writer, WTADefinition06, Area, JetDefinition01, WTADefinition, 0.6, ParticlesSignal, ParticlesDummy, JetSelector, "WTASignalJet06");
      DoJet(Writer, WTADefinition08, Area, JetDefinition01, WTADefinition, 0.8, ParticlesSignal, ParticlesDummy, JetSelector, "WTASignalJet08");
      DoJet(Writer, WTADefinition10, Area, JetDefinition01, WTADefinition, 1.0, ParticlesSignal, ParticlesDummy, JetSelector, "WTASignalJet10");
      
      DoJet(Writer, WTADefinition02, Area, JetDefinition01, WTADefinition, 0.2, ParticlesReal, ParticlesDummy, JetSelector, "WTAAllJet02");
      DoJet(Writer, WTADefinition04, Area, JetDefinition01, WTADefinition, 0.4, ParticlesReal, ParticlesDummy, JetSelector, "WTAAllJet04");
      DoJet(Writer, WTADefinition06, Area, JetDefinition01, WTADefinition, 0.6, ParticlesReal, ParticlesDummy, JetSelector, "WTAAllJet06");
      DoJet(Writer, WTADefinition08, Area, JetDefinition01, WTADefinition, 0.8, ParticlesReal, ParticlesDummy, JetSelector, "WTAAllJet08");
      DoJet(Writer, WTADefinition10, Area, JetDefinition01, WTADefinition, 1.0, ParticlesReal, ParticlesDummy, JetSelector, "WTAAllJet10");

      //---------------------------------------------------------------------------
      //   Write tree
      //---------------------------------------------------------------------------

      // Give variable we want to write out to treeWriter.
      // Only vectors of the types 'jetCollection', and 'double', 'int', 'PseudoJet' are supported

      Writer.addCollection("LeadingPhoton",    LeadingPhoton);
      Writer.addCollection("HemisphereSignal", HemisphereJetSignal);
      Writer.addCollection("HemisphereAll",    HemisphereJetAll);
      Writer.addCollection("LeadingHadron",    LeadingHadron);
      
      Writer.addCollection("EventWeight",      EventWeight);

      Writer.fillTree();

   } //event loop

   Bar.Update(EventCount);
   Bar.Print();
   Bar.PrintLine();

   TTree *trOut = Writer.getTree();

   TFile *fout = new TFile(cmdline.value<string>("-output", "JetToyHIResult.root").c_str(), "RECREATE");
   trOut->Write();
   fout->Write();
   fout->Close();

   double time_in_seconds = chrono::duration_cast<chrono::milliseconds>
      (chrono::steady_clock::now() - start_time).count() / 1000.0;
   cout << "Execution time: " << time_in_seconds << " seconds" << endl;
}

bool CompareJet(PseudoJet &J1, PseudoJet &J2)
{ 
   return (J1.perp() > J2.perp());
} 

void DoJet(treeWriter &Writer, JetDefinition &Definition, AreaDefinition Area,
   JetDefinition &SubjetDefinition, JetDefinition &WTADefinition, double R,
   vector<PseudoJet> &Particles, vector<PseudoJet> &Dummy,
   Selector &JetSelector, string Tag)
{
   ClusterSequenceArea Cluster(Particles, Definition, Area);
   jetCollection JC(sorted_by_pt(JetSelector(Cluster.inclusive_jets(10))));
   jetCollection JCJewel(GetCorrectedJets(JC.getJet(), Dummy));

   softDropGroomer SD(0.1, 0.0, R);
   jetCollection JCSD(SD.doGrooming(JC));
   JCSD.addVector(Tag + "SDZG",      SD.getZgs());
   JCSD.addVector(Tag + "SDNBranch", SD.getNDroppedSubjets());
   JCSD.addVector(Tag + "SDDR12",    SD.getDR12());
   JCSD.addVector(Tag + "SDSubjet1", SD.getSubjets1());
   JCSD.addVector(Tag + "SDSubjet2", SD.getSubjets2());
   
   jetCollection JCSDJewel(GetCorrectedJets(SD.getConstituents(), Dummy));
   vector<pair<PseudoJet, PseudoJet>> SDJewel
      = GetCorrectedSubJets(SD.getConstituents1(), SD.getConstituents2(), Dummy);
   JCSDJewel.addVector(Tag + "SDJewelZG",      CalculateZG(SDJewel));
   JCSDJewel.addVector(Tag + "SDJewelDR12",    CalculateDR(SDJewel));
   JCSDJewel.addVector(Tag + "SDJewelSubjet1", CalculateSubjet1(SDJewel));
   JCSDJewel.addVector(Tag + "SDJewelSubjet2", CalculateSubjet2(SDJewel));

   vector<PseudoJet> SJ1, SJ2;
   for(auto J : JC.getJet())
   {
      ClusterSequence SJCluster(J.constituents(), SubjetDefinition);
      vector<PseudoJet> SJ = sorted_by_pt(SJCluster.inclusive_jets(2));
      if(SJ.size() > 0)   SJ1.push_back(SJ[0]);
      else                SJ1.push_back(PseudoJet(0, 0, 0, 0));
      if(SJ.size() > 1)   SJ2.push_back(SJ[1]);
      else                SJ2.push_back(PseudoJet(0, 0, 0, 0));
   }
   JC.addVector(Tag + "SJ1", SJ1);
   JC.addVector(Tag + "SJ2", SJ2);

   vector<PseudoJet> WTAAxis;
   for(auto J : JC.getJet())
   {
      ClusterSequence WTACluster(J.constituents(), WTADefinition);
      vector<PseudoJet> Axis = sorted_by_pt(WTACluster.exclusive_jets(1));
      if(Axis.size() > 0)   WTAAxis.push_back(Axis[0]);
      else                  WTAAxis.push_back(PseudoJet(0, 0, 0, 0));
   }
   JC.addVector(Tag + "WTAAxis", WTAAxis);

   Writer.addCollection(Tag + "",        JC);
   Writer.addCollection(Tag + "Jewel",   JCJewel);
   Writer.addCollection(Tag + "SD",      JCSD);
   Writer.addCollection(Tag + "SDJewel", JCSDJewel);
}



