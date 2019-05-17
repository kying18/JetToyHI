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
#include "include/softDropCounter.hh"
#include "include/treeWriter.hh"
#include "include/jetMatcher.hh"
#include "include/randomCones.hh"
#include "include/Angularity.hh"
#include "include/jewelMatcher.hh"

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
   double JetR                = 0.4;
   double ghostRapMax         = 6.0;
   double ghost_area          = 0.005;
   int    active_area_repeats = 1;
   GhostedAreaSpec ghost_spec(ghostRapMax, active_area_repeats, ghost_area);
   AreaDefinition Area = AreaDefinition(active_area, ghost_spec);
   JetDefinition Definition(antikt_algorithm, JetR);

   Selector JetSelector = SelectorAbsRapMax(3.0);

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
      //   Jet clustering
      //---------------------------------------------------------------------------

      string Tag = "SignalJet";

      ClusterSequenceArea Cluster(ParticlesSignal, Definition, Area);
      jetCollection JC(sorted_by_pt(JetSelector(Cluster.inclusive_jets(10))));
      jetCollection JCJewel(GetCorrectedJets(JC.getJet(), ParticlesDummy));

      softDropGroomer SD(0.1, 0.0, JetR);
      jetCollection JCSD(SD.doGrooming(JC));
      JCSD.addVector(Tag + "SDZG",      SD.getZgs());
      JCSD.addVector(Tag + "SDNBranch", SD.getNDroppedSubjets());
      JCSD.addVector(Tag + "SDDR12",    SD.getDR12());
      JCSD.addVector(Tag + "SDSubjet1", SD.getSubjets1());
      JCSD.addVector(Tag + "SDSubjet2", SD.getSubjets2());

      jetCollection JCSDJewel(GetCorrectedJets(SD.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> SDJewel
         = GetCorrectedSubJets(SD.getConstituents1(), SD.getConstituents2(), ParticlesDummy);
      JCSDJewel.addVector(Tag + "SDJewelZG",      CalculateZG(SDJewel));
      JCSDJewel.addVector(Tag + "SDJewelDR12",    CalculateDR(SDJewel));
      JCSDJewel.addVector(Tag + "SDJewelSubjet1", CalculateSubjet1(SDJewel));
      JCSDJewel.addVector(Tag + "SDJewelSubjet2", CalculateSubjet2(SDJewel));

      softDropCounter CounterCA(0.0, 0.0, 0.4, 0.0);  //zcut, beta, jet R, r cut
      softDropCounter CounterAK(0.0, 0.0, 0.4, 0.0);  //zcut, beta, jet R, r cut
      softDropCounter CounterKT(0.0, 0.0, 0.4, 0.0);  //zcut, beta, jet R, r cut
      CounterCA.setAlgorithm(0);
      CounterAK.setAlgorithm(-1);
      CounterKT.setAlgorithm(1);
      CounterCA.run(JC);
      CounterAK.run(JC);
      CounterKT.run(JC);
      JC.addVector(Tag + "CAZGs", CounterCA.GetZGs());
      JC.addVector(Tag + "CADRs", CounterCA.GetDRs());
      JC.addVector(Tag + "AKZGs", CounterAK.GetZGs());
      JC.addVector(Tag + "AKDRs", CounterAK.GetDRs());
      JC.addVector(Tag + "KTZGs", CounterKT.GetZGs());
      JC.addVector(Tag + "KTDRs", CounterKT.GetDRs());

      //---------------------------------------------------------------------------
      //   Write tree
      //---------------------------------------------------------------------------

      // Give variable we want to write out to treeWriter.
      // Only vectors of the types 'jetCollection', and 'double', 'int', 'PseudoJet' are supported

      Writer.addCollection(Tag + "", JC);

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



