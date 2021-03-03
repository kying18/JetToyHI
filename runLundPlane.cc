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

vector<int> getConstituentVector(vector<vector<PseudoJet>> &constituents) {
   vector<int> cVector(constituents.size());
   for (int i=0; i<(int)constituents.size(); i++) {
      cVector[i] = constituents[i].size();
   }

   return cVector;

}


int main(int argc, char *argv[])
{
   auto start_time = chrono::steady_clock::now();

   CmdLine cmdline(argc, argv);

   int EventCount = cmdline.value<int>("-nev", 1);
   //bool verbose = cmdline.present("-verbose");

   bool DoPythiaShower = cmdline.present("-pythiashower");
   bool DoSubtraction = cmdline.present("-subtraction");

   cout << "will run on " << EventCount << " events" << endl;

   // Uncomment to silence fastjet banner
   ClusterSequence::set_fastjet_banner_stream(NULL);

   //to write info to root tree
   treeWriter Writer("JetTree");

   //Jet definition
   double JetR                = cmdline.value<double>("-r", 0.4);
   double ghostRapMax         = 6.0;
   double ghost_area          = 0.005;
   int    active_area_repeats = 1;
   GhostedAreaSpec ghost_spec(ghostRapMax, active_area_repeats, ghost_area);
   AreaDefinition Area = AreaDefinition(active_area, ghost_spec);
   JetDefinition Definition(antikt_algorithm, JetR);
   JetDefinition WTADefinition(antikt_algorithm, 10.0, WTA_pt_scheme);

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
      vector<PseudoJet> ParticlesSignal;
      vector<PseudoJet> ParticlesIntermediate;
      for(PseudoJet &P : ParticlesMerged)
      {
         int type = P.user_info<PU14>().vertex_number();
         switch(type)
         {
         case 1:  // this is background
            // fill into ParticlesReal
            ParticlesReal.push_back(P);
            break;
         case 0:  // this is signal
            // fill into ParticlesReal
            // fill into ParticlesSignal
            ParticlesReal.push_back(P);
            ParticlesSignal.push_back(P);
            break;
         case -1:   // this is dummy
            // fill into ParticlesDummy
            if (P.perp() < 1e-5 && fabs(P.pz()) > 2000) {
               break;
            }
            ParticlesDummy.push_back(P);
            break;
         default:   // all other cases are intermediate particles
            // fill into ParticlesIntermediate
            ParticlesIntermediate.push_back(P);
            break;
         }
      }

      //---------------------------------------------------------------------------
      //   Leading parton history
      //---------------------------------------------------------------------------
      
      vector<PseudoJet> Parton;
      vector<double> PartonZG, PartonDR;
      vector<PseudoJet> PartonSJ1, PartonSJ2;
      
      if(DoPythiaShower)
      {
         const EventList &List = mixer.get_hard_list();

         vector<int> HardParticles = List.GetListByStatus(23);   // pythia8

         int HardIndex = -1;
         for(int i = 0; i < (int)HardParticles.size(); i++)
            if(HardIndex < 0 || List.P[HardIndex].perp() < List.P[HardParticles[i]].perp())
               HardIndex = HardParticles[i];
         Parton.push_back(List.P[HardIndex]);

         if(HardIndex >= 0)
         {
            vector<int> HardShower = List.TraceShower(HardIndex);
            HardShower = List.KeepParton(HardShower);
            if(HardShower.size() > 0)
               HardShower.pop_back();

            PartonZG = List.GetZGs(HardShower);
            PartonDR = List.GetDRs(HardShower);
            PartonSJ1 = List.GetSJ1s(HardShower);
            PartonSJ2 = List.GetSJ2s(HardShower);
         }
      }

      //---------------------------------------------------------------------------
      //   find leading photon
      //---------------------------------------------------------------------------

      int PhotonIndex = -1;
      vector<double> Particle0PT, Particle0Phi;
      for(int i = 0; i < (int)ParticlesSignal.size(); i++)
      {
         const int &ID = ParticlesSignal[i].user_info<PU14>().pdg_id();

         if(ID != 22)
            continue;


         double AbsEta = fabs(ParticlesSignal[i].eta());
         if(AbsEta > 2.5)
            continue;

         if(PhotonIndex < 0 || ParticlesSignal[PhotonIndex].perp() < ParticlesSignal[i].perp()) {
            PhotonIndex = i;
         }
      }

      vector<PseudoJet> LeadingPhoton;
      if(PhotonIndex >= 0)
         LeadingPhoton.push_back(ParticlesSignal[PhotonIndex]);

      //---------------------------------------------------------------------------
      //   opposite hemisphere selection
      //---------------------------------------------------------------------------

      vector<PseudoJet> HemisphereSignal, HemisphereAll;
      vector<double> LeadingPhotonPhi, LeadingPhotonPt;

      if(LeadingPhoton.size() > 0)
      {
         double PhotonPhi = LeadingPhoton[0].phi();
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
         LeadingPhotonPhi.push_back(PhotonPhi);
         LeadingPhotonPt.push_back(LeadingPhoton[0].pt());
      } else {
         LeadingPhotonPhi.push_back(-1);
         LeadingPhotonPt.push_back(-1);
      }
      
      //---------------------------------------------------------------------------
      //   find leading ZBoson
      //---------------------------------------------------------------------------
      int ePIndex=-1;
      int eMIndex=-1;

      for(int i = 0; i < (int)ParticlesSignal.size(); i++)
      {
         const int &ID = ParticlesSignal[i].user_info<PU14>().pdg_id();
         if (ePIndex>=0&&eMIndex>=0) continue;
         if(fabs(ID) != 13) {
            continue;
         }
         double AbsEta = fabs(ParticlesSignal[i].eta());
         if(AbsEta > 2.5)
            continue;
         if (ID==13 && ePIndex<0) ePIndex=i;
         if (ID==-13 && eMIndex<0) eMIndex=i;
      }

      vector<PseudoJet> LeadingZBoson;
      if(ePIndex >= 0&&eMIndex>=0)
      LeadingZBoson.push_back(ParticlesSignal[ePIndex]+ParticlesSignal[eMIndex]);
      //---------------------------------------------------------------------------
      //   opposite hemisphere selection
      //---------------------------------------------------------------------------
      // vector<PseudoJet> HemisphereSignal, HemisphereAll;
      vector<double> LeadingZPhi;
   
      if(LeadingZBoson.size() > 0)  // rather than trying to get ZBosonIndex
      {
         // double ZBosonPhi = ParticlesReal[ZBosonIndex].phi();
         
         double ZBosonPhi = LeadingZBoson[0].phi();
         for(int i = 0; i < (int)ParticlesReal.size(); i++)
         {
            double ParticlePhi = ParticlesReal[i].phi();
            double DPhi = ZBosonPhi - ParticlePhi;

            if(DPhi < -M_PI)   DPhi = DPhi + 2 * M_PI;
            if(DPhi > +M_PI)   DPhi = DPhi - 2 * M_PI;

            if(DPhi > -M_PI / 2 && DPhi < M_PI / 2)
               continue;

            HemisphereAll.push_back(ParticlesReal[i]);
         }
         for(int i = 0; i < (int)ParticlesSignal.size(); i++)
         {
            double ParticlePhi = ParticlesSignal[i].phi();
            double DPhi = ZBosonPhi - ParticlePhi;

            if(DPhi < -M_PI)   DPhi = DPhi + 2 * M_PI;
            if(DPhi > +M_PI)   DPhi = DPhi - 2 * M_PI;

            if(DPhi > -M_PI / 2 && DPhi < M_PI / 2)
               continue;

            HemisphereSignal.push_back(ParticlesSignal[i]);
         }
         LeadingZPhi.push_back(ZBosonPhi);
      } else {
         LeadingZPhi.push_back(-1);
      }

      //---------------------------------------------------------------------------
      //   Hemisphere clustering
      //---------------------------------------------------------------------------

      if(HemisphereSignal.size() > 0)
      {
         ClusterSequence HemisphereSignalCluster(HemisphereSignal, WTADefinition);
         jetCollection HemisphereJetSignal(HemisphereSignalCluster.exclusive_jets(1));
         Writer.addCollection("HemisphereSignal", HemisphereJetSignal);
      }
      else
      {
         jetCollection HemisphereJetSignal();
         Writer.addCollection("HemisphereSignal", HemisphereSignal);
      }

      if(HemisphereAll.size() > 0)
      {
         ClusterSequence HemisphereAllCluster(HemisphereAll, WTADefinition);
         jetCollection HemisphereJetAll(HemisphereAllCluster.exclusive_jets(1));
         Writer.addCollection("HemisphereAll",    HemisphereJetAll);
      }
      else
      {
         jetCollection HemisphereJetAll();
         Writer.addCollection("HemisphereAll", HemisphereAll);
      }

      //---------------------------------------------------------------------------
      //   Jet clustering
      //---------------------------------------------------------------------------

      string Tag = "SignalJet";

      ClusterSequenceArea Cluster(ParticlesReal, Definition, Area);
      jetCollection JC(sorted_by_pt(JetSelector(Cluster.inclusive_jets(10))));
      jetCollection JCJewel(GetCorrectedJets(JC.getJet(), ParticlesDummy));

      vector<double> Rho, RhoM;
      vector<int> JConstituents;
      vector<vector<double>> JConstituentPt;
      vector<vector<double>> px, py, pz;
      vector<vector<double>> pid;

      csSubtractor Subtractor(JetR, 0, -1, 0.005, 6.0, 3.0);
      Subtractor.setInputParticles(ParticlesReal);
      jetCollection JCSub(Subtractor.doSubtraction());
      Rho.push_back(Subtractor.getRho());
      RhoM.push_back(Subtractor.getRhoM());

      jetCollection &JCC = (DoSubtraction ? JCSub : JC);

      softDropGroomer SD1(0.10, 0.00, JetR);
      softDropGroomer SD2(0.50, 1.50, JetR);
      softDropGroomer SD3(0.25, 0.00, JetR);
      softDropGroomer SD4(0.15, -1.0, JetR);
      softDropGroomer SD5(5.00, 5.00, JetR);
      softDropGroomer SD6(0.40, 0.00, JetR);

      jetCollection JCSD1(SD1.doGrooming(JCC));
      jetCollection JCSD2(SD2.doGrooming(JCC));
      jetCollection JCSD3(SD3.doGrooming(JCC));
      jetCollection JCSD4(SD4.doGrooming(JCC));
      jetCollection JCSD5(SD5.doGrooming(JCC));
      jetCollection JCSD6(SD6.doGrooming(JCC));

      vector<vector<PseudoJet>> ConstituentsSD1 = SD1.getConstituents();
      vector<vector<PseudoJet>> ConstituentsSD2 = SD2.getConstituents();
      vector<vector<PseudoJet>> ConstituentsSD3 = SD3.getConstituents();
      vector<vector<PseudoJet>> ConstituentsSD4 = SD4.getConstituents();
      vector<vector<PseudoJet>> ConstituentsSD5 = SD5.getConstituents();
      vector<vector<PseudoJet>> ConstituentsSD6 = SD6.getConstituents();

      vector<int> JSD1Constituents = getConstituentVector(ConstituentsSD1);
      vector<int> JSD2Constituents = getConstituentVector(ConstituentsSD2);
      vector<int> JSD3Constituents = getConstituentVector(ConstituentsSD3);
      vector<int> JSD4Constituents = getConstituentVector(ConstituentsSD4);
      vector<int> JSD5Constituents = getConstituentVector(ConstituentsSD5);
      vector<int> JSD6Constituents = getConstituentVector(ConstituentsSD6);

      for (auto J : JCC.getJet()) {
         JConstituents.push_back(J.constituents().size());
         vector<double> JCPt;
         vector<double> JCPx, JCPy, JCPz, JCPid;
         for(int i = 0; i < (int)J.constituents().size(); i++) {
            JCPt.push_back(J.constituents()[i].pt());
            JCPx.push_back(J.constituents()[i].px());
            JCPy.push_back(J.constituents()[i].py());
            JCPz.push_back(J.constituents()[i].pz());
            // JCPid.push_back(J.constituents()[i].E());
            JCPid.push_back((double)J.constituents()[i].user_info<PU14>().pdg_id());
         }
         JConstituentPt.push_back(JCPt);
         // JCPx.push_back(J.px());
         // JCPy.push_back(J.py());
         // JCPz.push_back(J.pz());
         // // JCPid.push_back(J.E());
         // JCPid.push_back(J.user_info<PU14>().pdg_id());
         px.push_back(JCPx);
         py.push_back(JCPy);
         pz.push_back(JCPz);
         pid.push_back(JCPid);
      }
      // px.push_back(JCPx);
      // py.push_back(JCPy);
      // pz.push_back(JCPz);
      // pid.push_back(JCPid);

      JCSD1.addVector(Tag + "SD1NConstituent", JSD1Constituents);
      JCSD1.addVector(Tag + "SD1ZG",      SD1.getZgs());
      JCSD1.addVector(Tag + "SD1NBranch", SD1.getNDroppedSubjets());
      JCSD1.addVector(Tag + "SD1DR12",    SD1.getDR12());
      JCSD1.addVector(Tag + "SD1Subjet1", SD1.getSubjets1());
      JCSD1.addVector(Tag + "SD1Subjet2", SD1.getSubjets2());

      JCSD2.addVector(Tag + "SD2NConstituent", JSD2Constituents);
      JCSD2.addVector(Tag + "SD2ZG",      SD2.getZgs());
      JCSD2.addVector(Tag + "SD2NBranch", SD2.getNDroppedSubjets());
      JCSD2.addVector(Tag + "SD2DR12",    SD2.getDR12());
      JCSD2.addVector(Tag + "SD2Subjet1", SD2.getSubjets1());
      JCSD2.addVector(Tag + "SD2Subjet2", SD2.getSubjets2());

      JCSD3.addVector(Tag + "SD3NConstituent", JSD3Constituents);
      JCSD3.addVector(Tag + "SD3ZG",      SD3.getZgs());
      JCSD3.addVector(Tag + "SD3NBranch", SD3.getNDroppedSubjets());
      JCSD3.addVector(Tag + "SD3DR12",    SD3.getDR12());
      JCSD3.addVector(Tag + "SD3Subjet1", SD3.getSubjets1());
      JCSD3.addVector(Tag + "SD3Subjet2", SD3.getSubjets2());

      JCSD4.addVector(Tag + "SD4NConstituent", JSD4Constituents);
      JCSD4.addVector(Tag + "SD4ZG",      SD4.getZgs());
      JCSD4.addVector(Tag + "SD4NBranch", SD4.getNDroppedSubjets());
      JCSD4.addVector(Tag + "SD4DR12",    SD4.getDR12());
      JCSD4.addVector(Tag + "SD4Subjet1", SD4.getSubjets1());
      JCSD4.addVector(Tag + "SD4Subjet2", SD4.getSubjets2());
      
      JCSD5.addVector(Tag + "SD5NConstituent", JSD5Constituents);
      JCSD5.addVector(Tag + "SD5ZG",      SD5.getZgs());
      JCSD5.addVector(Tag + "SD5NBranch", SD5.getNDroppedSubjets());
      JCSD5.addVector(Tag + "SD5DR12",    SD5.getDR12());
      JCSD5.addVector(Tag + "SD5Subjet1", SD5.getSubjets1());
      JCSD5.addVector(Tag + "SD5Subjet2", SD5.getSubjets2());

      JCSD6.addVector(Tag + "SD6NConstituent", JSD6Constituents);
      JCSD6.addVector(Tag + "SD6ZG",      SD6.getZgs());
      JCSD6.addVector(Tag + "SD6NBranch", SD6.getNDroppedSubjets());
      JCSD6.addVector(Tag + "SD6DR12",    SD6.getDR12());
      JCSD6.addVector(Tag + "SD6Subjet1", SD6.getSubjets1());
      JCSD6.addVector(Tag + "SD6Subjet2", SD6.getSubjets2());

      jetCollection JCSD1Jewel(GetCorrectedJets(SD1.getConstituents(), ParticlesDummy));
      jetCollection JCSD2Jewel(GetCorrectedJets(SD2.getConstituents(), ParticlesDummy));
      jetCollection JCSD3Jewel(GetCorrectedJets(SD3.getConstituents(), ParticlesDummy));
      jetCollection JCSD4Jewel(GetCorrectedJets(SD4.getConstituents(), ParticlesDummy));
      jetCollection JCSD5Jewel(GetCorrectedJets(SD5.getConstituents(), ParticlesDummy));
      jetCollection JCSD6Jewel(GetCorrectedJets(SD6.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> SD1Jewel
         = GetCorrectedSubJets(SD1.getConstituents1(), SD1.getConstituents2(), ParticlesDummy);
      vector<pair<PseudoJet, PseudoJet>> SD2Jewel
         = GetCorrectedSubJets(SD2.getConstituents1(), SD2.getConstituents2(), ParticlesDummy);
      vector<pair<PseudoJet, PseudoJet>> SD3Jewel
         = GetCorrectedSubJets(SD3.getConstituents1(), SD3.getConstituents2(), ParticlesDummy);
      vector<pair<PseudoJet, PseudoJet>> SD4Jewel
         = GetCorrectedSubJets(SD4.getConstituents1(), SD4.getConstituents2(), ParticlesDummy);
      vector<pair<PseudoJet, PseudoJet>> SD5Jewel
         = GetCorrectedSubJets(SD5.getConstituents1(), SD5.getConstituents2(), ParticlesDummy);
      vector<pair<PseudoJet, PseudoJet>> SD6Jewel
         = GetCorrectedSubJets(SD6.getConstituents1(), SD6.getConstituents2(), ParticlesDummy);
      JCSD1Jewel.addVector(Tag + "SD1JewelZG",      CalculateZG(SD1Jewel));
      JCSD1Jewel.addVector(Tag + "SD1JewelDR12",    CalculateDR(SD1Jewel));
      JCSD1Jewel.addVector(Tag + "SD1JewelSubjet1", CalculateSubjet1(SD1Jewel));
      JCSD1Jewel.addVector(Tag + "SD1JewelSubjet2", CalculateSubjet2(SD1Jewel));
      JCSD2Jewel.addVector(Tag + "SD2JewelZG",      CalculateZG(SD2Jewel));
      JCSD2Jewel.addVector(Tag + "SD2JewelDR12",    CalculateDR(SD2Jewel));
      JCSD2Jewel.addVector(Tag + "SD2JewelSubjet1", CalculateSubjet1(SD2Jewel));
      JCSD2Jewel.addVector(Tag + "SD2JewelSubjet2", CalculateSubjet2(SD2Jewel));
      JCSD3Jewel.addVector(Tag + "SD3JewelZG",      CalculateZG(SD3Jewel));
      JCSD3Jewel.addVector(Tag + "SD3JewelDR12",    CalculateDR(SD3Jewel));
      JCSD3Jewel.addVector(Tag + "SD3JewelSubjet1", CalculateSubjet1(SD3Jewel));
      JCSD3Jewel.addVector(Tag + "SD3JewelSubjet2", CalculateSubjet2(SD3Jewel));
      JCSD4Jewel.addVector(Tag + "SD4JewelZG",      CalculateZG(SD4Jewel));
      JCSD4Jewel.addVector(Tag + "SD4JewelDR12",    CalculateDR(SD4Jewel));
      JCSD4Jewel.addVector(Tag + "SD4JewelSubjet1", CalculateSubjet1(SD4Jewel));
      JCSD4Jewel.addVector(Tag + "SD4JewelSubjet2", CalculateSubjet2(SD4Jewel));
      JCSD5Jewel.addVector(Tag + "SD5JewelZG",      CalculateZG(SD5Jewel));
      JCSD5Jewel.addVector(Tag + "SD5JewelDR12",    CalculateDR(SD5Jewel));
      JCSD5Jewel.addVector(Tag + "SD5JewelSubjet1", CalculateSubjet1(SD5Jewel));
      JCSD5Jewel.addVector(Tag + "SD5JewelSubjet2", CalculateSubjet2(SD5Jewel));
      JCSD6Jewel.addVector(Tag + "SD6JewelZG",      CalculateZG(SD6Jewel));
      JCSD6Jewel.addVector(Tag + "SD6JewelDR12",    CalculateDR(SD6Jewel));
      JCSD6Jewel.addVector(Tag + "SD6JewelSubjet1", CalculateSubjet1(SD6Jewel));
      JCSD6Jewel.addVector(Tag + "SD6JewelSubjet2", CalculateSubjet2(SD6Jewel));

      softDropCounter CounterCA(0.0, 0.0, JetR, 0.0);  //zcut, beta, jet R, r cut
      softDropCounter CounterCAAK(0.0, 0.0, JetR, 0.0);  //zcut, beta, jet R, r cut
      softDropCounter CounterAK(0.0, 0.0, JetR, 0.0);  //zcut, beta, jet R, r cut
      softDropCounter CounterCAKT(0.0, 0.0, JetR, 0.0);  //zcut, beta, jet R, r cut
      softDropCounter CounterKT(0.0, 0.0, JetR, 0.0);  //zcut, beta, jet R, r cut
      CounterCA.setAlgorithm(0);
      CounterCAAK.setAlgorithm(-0.5);
      CounterAK.setAlgorithm(-1);
      CounterCAKT.setAlgorithm(0.5);
      CounterKT.setAlgorithm(1);
      CounterCA.run(JCC, ParticlesDummy);
      CounterCAAK.run(JCC, ParticlesDummy);
      CounterAK.run(JCC, ParticlesDummy);
      CounterCAKT.run(JCC, ParticlesDummy);
      CounterKT.run(JCC, ParticlesDummy);
      JCC.addVector(Tag + "NConstituent", JConstituents);
      JCC.addVector(Tag + "ConstituentPt", JConstituentPt);
      // JCC.addVector(Tag + "Px", JCPx);
      // JCC.addVector(Tag + "Py", JCPy);
      // JCC.addVector(Tag + "Pz", JCPz);
      // JCC.addVector(Tag + "Pid", JCPid);
      JCC.addVector(Tag + "Px", px);
      JCC.addVector(Tag + "Py", py);
      JCC.addVector(Tag + "Pz", pz);
      JCC.addVector(Tag + "Pid", pid);
      JCC.addVector(Tag + "CAZGs", CounterCA.GetZGs());
      JCC.addVector(Tag + "CADRs", CounterCA.GetDRs());
      JCC.addVector(Tag + "CAPT1s", CounterCA.GetPT1s());
      JCC.addVector(Tag + "CAEta1s", CounterCA.GetEta1s());
      JCC.addVector(Tag + "CAPhi1s", CounterCA.GetPhi1s());
      JCC.addVector(Tag + "CAPT2s", CounterCA.GetPT2s());
      JCC.addVector(Tag + "CAEta2s", CounterCA.GetEta2s());
      JCC.addVector(Tag + "CAPhi2s", CounterCA.GetPhi2s());
      JCC.addVector(Tag + "CAAKZGs", CounterCAAK.GetZGs());
      JCC.addVector(Tag + "CAAKDRs", CounterCAAK.GetDRs());
      JCC.addVector(Tag + "CAAKPT1s", CounterCAAK.GetPT1s());
      JCC.addVector(Tag + "CAAKEta1s", CounterCAAK.GetEta1s());
      JCC.addVector(Tag + "CAAKPhi1s", CounterCAAK.GetPhi1s());
      JCC.addVector(Tag + "CAAKPT2s", CounterCAAK.GetPT2s());
      JCC.addVector(Tag + "CAAKEta2s", CounterCAAK.GetEta2s());
      JCC.addVector(Tag + "CAAKPhi2s", CounterCAAK.GetPhi2s());
      JCC.addVector(Tag + "CAKTZGs", CounterCAKT.GetZGs());
      JCC.addVector(Tag + "CAKTDRs", CounterCAKT.GetDRs());
      JCC.addVector(Tag + "CAKTPT1s", CounterCAKT.GetPT1s());
      JCC.addVector(Tag + "CAKTEta1s", CounterCAKT.GetEta1s());
      JCC.addVector(Tag + "CAKTPhi1s", CounterCAKT.GetPhi1s());
      JCC.addVector(Tag + "CAKTPT2s", CounterCAKT.GetPT2s());
      JCC.addVector(Tag + "CAKTEta2s", CounterCAKT.GetEta2s());
      JCC.addVector(Tag + "CAKTPhi2s", CounterCAKT.GetPhi2s());
      JCC.addVector(Tag + "AKZGs", CounterAK.GetZGs());
      JCC.addVector(Tag + "AKDRs", CounterAK.GetDRs());
      JCC.addVector(Tag + "AKPT1s", CounterAK.GetPT1s());
      JCC.addVector(Tag + "AKEta1s", CounterAK.GetEta1s());
      JCC.addVector(Tag + "AKPhi1s", CounterAK.GetPhi1s());
      JCC.addVector(Tag + "AKPT2s", CounterAK.GetPT2s());
      JCC.addVector(Tag + "AKEta2s", CounterAK.GetEta2s());
      JCC.addVector(Tag + "AKPhi2s", CounterAK.GetPhi2s());
      JCC.addVector(Tag + "KTZGs", CounterKT.GetZGs());
      JCC.addVector(Tag + "KTDRs", CounterKT.GetDRs());
      JCC.addVector(Tag + "KTPT1s", CounterKT.GetPT1s());
      JCC.addVector(Tag + "KTEta1s", CounterKT.GetEta1s());
      JCC.addVector(Tag + "KTPhi1s", CounterKT.GetPhi1s());
      JCC.addVector(Tag + "KTPT2s", CounterKT.GetPT2s());
      JCC.addVector(Tag + "KTEta2s", CounterKT.GetEta2s());
      JCC.addVector(Tag + "KTPhi2s", CounterKT.GetPhi2s());

      //---------------------------------------------------------------------------
      //   Write tree
      //---------------------------------------------------------------------------

      // Give variable we want to write out to treeWriter.
      // Only vectors of the types 'jetCollection', and 'double', 'int', 'PseudoJet' are supported

      Writer.addCollection(Tag + "", JCC);
      Writer.addCollection(Tag + "Jewel", JCJewel);

      Writer.addCollection("Rho",  Rho);
      Writer.addCollection("RhoM", RhoM);

      Writer.addCollection(Tag + "SD1", JCSD1);
      Writer.addCollection(Tag + "SD2", JCSD2);
      Writer.addCollection(Tag + "SD3", JCSD3);
      Writer.addCollection(Tag + "SD4", JCSD4);
      Writer.addCollection(Tag + "SD5", JCSD5);
      Writer.addCollection(Tag + "SD6", JCSD6);
      Writer.addCollection(Tag + "SD1Jewel", JCSD1Jewel);
      Writer.addCollection(Tag + "SD2Jewel", JCSD2Jewel);
      Writer.addCollection(Tag + "SD3Jewel", JCSD3Jewel);
      Writer.addCollection(Tag + "SD4Jewel", JCSD4Jewel);
      Writer.addCollection(Tag + "SD5Jewel", JCSD5Jewel);
      Writer.addCollection(Tag + "SD6Jewel", JCSD6Jewel);

      Writer.addCollection("EventWeight",      EventWeight);
      Writer.addCollection("LeadingZPhi", LeadingZPhi);
      Writer.addCollection("LeadingPhotonPhi", LeadingPhotonPhi);
      Writer.addCollection("LeadingPhotonPt", LeadingPhotonPt);

      if(DoPythiaShower)
      {
         Writer.addCollection("Parton",           Parton);
         Writer.addCollection("PartonZGs",        PartonZG);
         Writer.addCollection("PartonDRs",        PartonDR);
         Writer.addCollection("PartonSJ1s",       PartonSJ1);
         Writer.addCollection("PartonSJ2s",       PartonSJ2);
      }

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



