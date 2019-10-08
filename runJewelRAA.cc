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
#include "include/SmearRandomKick.hh"

// Observables
//    1. Leading hadrons and photons
//    2. DPhi trigger object and recoil jets (leading and subleading, and all)
//    3. DPhi between different recoil jet axes
//    4. WTA on constituents or WTA on everything?
//    5. WTA vs smaller jets, two AK4 subjets
//    6. Inclusive Particles with PDG ID and isHadron

using namespace std;
using namespace fastjet;

// ./runJewelSub -hard  /eos/project/j/jetquenching/JetWorkshop2017/samples/jewel/DiJet/RecoilOn_0_10/Jewel_0_T_0.pu14 -pileup XXXXX -nev 10

int main(int argc, char *argv[]);
bool CompareJet(const PseudoJet &J1, const PseudoJet &J2);
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


   double kick = cmdline.value<double>("-kick", 0);

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

      vector<PseudoJet> ParticlesMergedPreKick = mixer.particles();
      vector<PseudoJet> ParticlesMerged = SmearRandomKick(ParticlesMergedPreKick,kick);
      
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
      //   Jet clustering
      //---------------------------------------------------------------------------

       DoJet(Writer, JetDefinition02, Area, JetDefinition01, WTADefinition, 0.2, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet02");
       DoJet(Writer, JetDefinition02, Area, JetDefinition01, WTADefinition, 0.3, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet03");
      DoJet(Writer, JetDefinition04, Area, JetDefinition01, WTADefinition, 0.4, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet04");
       DoJet(Writer, JetDefinition06, Area, JetDefinition01, WTADefinition, 0.6, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet06");
       DoJet(Writer, JetDefinition08, Area, JetDefinition01, WTADefinition, 0.8, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet08");
       DoJet(Writer, JetDefinition10, Area, JetDefinition01, WTADefinition, 1.0, ParticlesSignal, ParticlesDummy, JetSelector, "SignalJet10");

/*      
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
*/
      //---------------------------------------------------------------------------
      //   Write tree
      //---------------------------------------------------------------------------

      // Give variable we want to write out to treeWriter.
      // Only vectors of the types 'jetCollection', and 'double', 'int', 'PseudoJet' are supported

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

bool CompareJet(const PseudoJet &J1, const PseudoJet &J2)
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

   Writer.addCollection(Tag + "",        JC);
   Writer.addCollection(Tag + "Jewel",   JCJewel);
}



