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

//./runJewelSub -hard  /eos/project/j/jetquenching/JetWorkshop2017/samples/jewel/DiJet/RecoilOn_0_10/Jewel_0_T_0.pu14 -pileup XXXXX -nev 10

int main (int argc, char *argv[])
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
   AreaDefinition area_def = AreaDefinition(active_area, ghost_spec);
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
      jetCollection JCSignal02(sorted_by_pt(JetSelector(ClusterSignal02.inclusive_jets())));
      jetCollection JCSignal02Jewel(GetCorrectedJets(JCSignal02.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterSignal04(ParticlesSignal, JetDefinition04, area_def);
      jetCollection JCSignal04(sorted_by_pt(JetSelector(ClusterSignal04.inclusive_jets())));
      jetCollection JCSignal04Jewel(GetCorrectedJets(JCSignal04.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterSignal06(ParticlesSignal, JetDefinition06, area_def);
      jetCollection JCSignal06(sorted_by_pt(JetSelector(ClusterSignal06.inclusive_jets())));
      jetCollection JCSignal06Jewel(GetCorrectedJets(JCSignal06.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterSignal08(ParticlesSignal, JetDefinition08, area_def);
      jetCollection JCSignal08(sorted_by_pt(JetSelector(ClusterSignal08.inclusive_jets())));
      jetCollection JCSignal08Jewel(GetCorrectedJets(JCSignal08.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterSignal10(ParticlesSignal, JetDefinition10, area_def);
      jetCollection JCSignal10(sorted_by_pt(JetSelector(ClusterSignal10.inclusive_jets())));
      jetCollection JCSignal10Jewel(GetCorrectedJets(JCSignal10.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterAll02(ParticlesReal, JetDefinition02, area_def);
      jetCollection JCAll02(sorted_by_pt(JetSelector(ClusterAll02.inclusive_jets())));
      jetCollection JCAll02Jewel(GetCorrectedJets(JCAll02.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterAll04(ParticlesReal, JetDefinition04, area_def);
      jetCollection JCAll04(sorted_by_pt(JetSelector(ClusterAll04.inclusive_jets())));
      jetCollection JCAll04Jewel(GetCorrectedJets(JCAll04.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterAll06(ParticlesReal, JetDefinition06, area_def);
      jetCollection JCAll06(sorted_by_pt(JetSelector(ClusterAll06.inclusive_jets())));
      jetCollection JCAll06Jewel(GetCorrectedJets(JCAll06.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterAll08(ParticlesReal, JetDefinition08, area_def);
      jetCollection JCAll08(sorted_by_pt(JetSelector(ClusterAll08.inclusive_jets())));
      jetCollection JCAll08Jewel(GetCorrectedJets(JCAll08.getJet(), ParticlesDummy));

      ClusterSequenceArea ClusterAll10(ParticlesReal, JetDefinition10, area_def);
      jetCollection JCAll10(sorted_by_pt(JetSelector(ClusterAll10.inclusive_jets())));
      jetCollection JCAll10Jewel(GetCorrectedJets(JCAll10.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterSignal02(ParticlesSignal, WTADefinition02, area_def);
      jetCollection WTAJCSignal02(sorted_by_pt(JetSelector(WTAClusterSignal02.inclusive_jets())));
      jetCollection WTAJCSignal02Jewel(GetCorrectedJets(WTAJCSignal02.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterSignal04(ParticlesSignal, WTADefinition04, area_def);
      jetCollection WTAJCSignal04(sorted_by_pt(JetSelector(WTAClusterSignal04.inclusive_jets())));
      jetCollection WTAJCSignal04Jewel(GetCorrectedJets(WTAJCSignal04.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterSignal06(ParticlesSignal, WTADefinition06, area_def);
      jetCollection WTAJCSignal06(sorted_by_pt(JetSelector(WTAClusterSignal06.inclusive_jets())));
      jetCollection WTAJCSignal06Jewel(GetCorrectedJets(WTAJCSignal06.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterSignal08(ParticlesSignal, WTADefinition08, area_def);
      jetCollection WTAJCSignal08(sorted_by_pt(JetSelector(WTAClusterSignal08.inclusive_jets())));
      jetCollection WTAJCSignal08Jewel(GetCorrectedJets(WTAJCSignal08.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterSignal10(ParticlesSignal, WTADefinition10, area_def);
      jetCollection WTAJCSignal10(sorted_by_pt(JetSelector(WTAClusterSignal10.inclusive_jets())));
      jetCollection WTAJCSignal10Jewel(GetCorrectedJets(WTAJCSignal10.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterAll02(ParticlesReal, WTADefinition02, area_def);
      jetCollection WTAJCAll02(sorted_by_pt(JetSelector(WTAClusterAll02.inclusive_jets())));
      jetCollection WTAJCAll02Jewel(GetCorrectedJets(WTAJCAll02.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterAll04(ParticlesReal, WTADefinition04, area_def);
      jetCollection WTAJCAll04(sorted_by_pt(JetSelector(WTAClusterAll04.inclusive_jets())));
      jetCollection WTAJCAll04Jewel(GetCorrectedJets(WTAJCAll04.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterAll06(ParticlesReal, WTADefinition06, area_def);
      jetCollection WTAJCAll06(sorted_by_pt(JetSelector(WTAClusterAll06.inclusive_jets())));
      jetCollection WTAJCAll06Jewel(GetCorrectedJets(WTAJCAll06.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterAll08(ParticlesReal, WTADefinition08, area_def);
      jetCollection WTAJCAll08(sorted_by_pt(JetSelector(WTAClusterAll08.inclusive_jets())));
      jetCollection WTAJCAll08Jewel(GetCorrectedJets(WTAJCAll08.getJet(), ParticlesDummy));

      ClusterSequenceArea WTAClusterAll10(ParticlesReal, WTADefinition10, area_def);
      jetCollection WTAJCAll10(sorted_by_pt(JetSelector(WTAClusterAll10.inclusive_jets())));
      jetCollection WTAJCAll10Jewel(GetCorrectedJets(WTAJCAll10.getJet(), ParticlesDummy));

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
      softDropGroomer WTASDSignal02(0.1, 0.0, 0.2);
      softDropGroomer WTASDSignal04(0.1, 0.0, 0.4);
      softDropGroomer WTASDSignal06(0.1, 0.0, 0.6);
      softDropGroomer WTASDSignal08(0.1, 0.0, 0.8);
      softDropGroomer WTASDSignal10(0.1, 0.0, 1.0);
      softDropGroomer WTASDAll02(0.1, 0.0, 0.2);
      softDropGroomer WTASDAll04(0.1, 0.0, 0.4);
      softDropGroomer WTASDAll06(0.1, 0.0, 0.6);
      softDropGroomer WTASDAll08(0.1, 0.0, 0.8);
      softDropGroomer WTASDAll10(0.1, 0.0, 1.0);

      jetCollection JCSignal02SD(SDSignal02.doGrooming(JCSignal02));
      JCSignal02SD.addVector("SignalJet02SDZG",      SDSignal02.getZgs());
      JCSignal02SD.addVector("SignalJet02SDNBranch", SDSignal02.getNDroppedSubjets());
      JCSignal02SD.addVector("SignalJet02SDDR12",    SDSignal02.getDR12());
      JCSignal02SD.addVector("SignalJet02SDSubjet1", SDSignal02.getSubjets1());
      JCSignal02SD.addVector("SignalJet02SDSubjet2", SDSignal02.getSubjets2());
      jetCollection JCSignal04SD(SDSignal04.doGrooming(JCSignal04));
      JCSignal04SD.addVector("SignalJet04SDZG",      SDSignal04.getZgs());
      JCSignal04SD.addVector("SignalJet04SDNBranch", SDSignal04.getNDroppedSubjets());
      JCSignal04SD.addVector("SignalJet04SDDR12",    SDSignal04.getDR12());
      JCSignal04SD.addVector("SignalJet04SDSubjet1", SDSignal04.getSubjets1());
      JCSignal04SD.addVector("SignalJet04SDSubjet2", SDSignal04.getSubjets2());
      jetCollection JCSignal06SD(SDSignal06.doGrooming(JCSignal06));
      JCSignal06SD.addVector("SignalJet06SDZG",      SDSignal06.getZgs());
      JCSignal06SD.addVector("SignalJet06SDNBranch", SDSignal06.getNDroppedSubjets());
      JCSignal06SD.addVector("SignalJet06SDDR12",    SDSignal06.getDR12());
      JCSignal06SD.addVector("SignalJet06SDSubjet1", SDSignal06.getSubjets1());
      JCSignal06SD.addVector("SignalJet06SDSubjet2", SDSignal06.getSubjets2());
      jetCollection JCSignal08SD(SDSignal08.doGrooming(JCSignal08));
      JCSignal08SD.addVector("SignalJet08SDZG",      SDSignal08.getZgs());
      JCSignal08SD.addVector("SignalJet08SDNBranch", SDSignal08.getNDroppedSubjets());
      JCSignal08SD.addVector("SignalJet08SDDR12",    SDSignal08.getDR12());
      JCSignal08SD.addVector("SignalJet08SDSubjet1", SDSignal08.getSubjets1());
      JCSignal08SD.addVector("SignalJet08SDSubjet2", SDSignal08.getSubjets2());
      jetCollection JCSignal10SD(SDSignal10.doGrooming(JCSignal10));
      JCSignal10SD.addVector("SignalJet10SDZG",      SDSignal10.getZgs());
      JCSignal10SD.addVector("SignalJet10SDNBranch", SDSignal10.getNDroppedSubjets());
      JCSignal10SD.addVector("SignalJet10SDDR12",    SDSignal10.getDR12());
      JCSignal10SD.addVector("SignalJet10SDSubjet1", SDSignal10.getSubjets1());
      JCSignal10SD.addVector("SignalJet10SDSubjet2", SDSignal10.getSubjets2());

      jetCollection JCSignal02SDJewel(GetCorrectedJets(SDSignal02.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal02SDJewel
         = GetCorrectedSubJets(SDSignal02.getConstituents1(), SDSignal02.getConstituents2(), ParticlesDummy);
      JCSignal02SDJewel.addVector("SignalJet02SDJewelZG",      CalculateZG(Signal02SDJewel));
      JCSignal02SDJewel.addVector("SignalJet02SDJewelDR12",    CalculateDR(Signal02SDJewel));
      JCSignal02SDJewel.addVector("SignalJet02SDJewelSubjet1", CalculateSubjet1(Signal02SDJewel));
      JCSignal02SDJewel.addVector("SignalJet02SDJewelSubjet2", CalculateSubjet2(Signal02SDJewel));
      jetCollection JCSignal04SDJewel(GetCorrectedJets(SDSignal04.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal04SDJewel
         = GetCorrectedSubJets(SDSignal04.getConstituents1(), SDSignal04.getConstituents2(), ParticlesDummy);
      JCSignal04SDJewel.addVector("SignalJet04SDJewelZG",      CalculateZG(Signal04SDJewel));
      JCSignal04SDJewel.addVector("SignalJet04SDJewelDR12",    CalculateDR(Signal04SDJewel));
      JCSignal04SDJewel.addVector("SignalJet04SDJewelSubjet1", CalculateSubjet1(Signal04SDJewel));
      JCSignal04SDJewel.addVector("SignalJet04SDJewelSubjet2", CalculateSubjet2(Signal04SDJewel));
      jetCollection JCSignal06SDJewel(GetCorrectedJets(SDSignal06.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal06SDJewel
         = GetCorrectedSubJets(SDSignal06.getConstituents1(), SDSignal06.getConstituents2(), ParticlesDummy);
      JCSignal06SDJewel.addVector("SignalJet06SDJewelZG",      CalculateZG(Signal06SDJewel));
      JCSignal06SDJewel.addVector("SignalJet06SDJewelDR12",    CalculateDR(Signal06SDJewel));
      JCSignal06SDJewel.addVector("SignalJet06SDJewelSubjet1", CalculateSubjet1(Signal06SDJewel));
      JCSignal06SDJewel.addVector("SignalJet06SDJewelSubjet2", CalculateSubjet2(Signal06SDJewel));
      jetCollection JCSignal08SDJewel(GetCorrectedJets(SDSignal08.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal08SDJewel
         = GetCorrectedSubJets(SDSignal08.getConstituents1(), SDSignal08.getConstituents2(), ParticlesDummy);
      JCSignal08SDJewel.addVector("SignalJet08SDJewelZG",      CalculateZG(Signal08SDJewel));
      JCSignal08SDJewel.addVector("SignalJet08SDJewelDR12",    CalculateDR(Signal08SDJewel));
      JCSignal08SDJewel.addVector("SignalJet08SDJewelSubjet1", CalculateSubjet1(Signal08SDJewel));
      JCSignal08SDJewel.addVector("SignalJet08SDJewelSubjet2", CalculateSubjet2(Signal08SDJewel));
      jetCollection JCSignal10SDJewel(GetCorrectedJets(SDSignal10.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> Signal10SDJewel
         = GetCorrectedSubJets(SDSignal10.getConstituents1(), SDSignal10.getConstituents2(), ParticlesDummy);
      JCSignal10SDJewel.addVector("SignalJet10SDJewelZG",      CalculateZG(Signal10SDJewel));
      JCSignal10SDJewel.addVector("SignalJet10SDJewelDR12",    CalculateDR(Signal10SDJewel));
      JCSignal10SDJewel.addVector("SignalJet10SDJewelSubjet1", CalculateSubjet1(Signal10SDJewel));
      JCSignal10SDJewel.addVector("SignalJet10SDJewelSubjet2", CalculateSubjet2(Signal10SDJewel));

      jetCollection JCAll02SD(SDAll02.doGrooming(JCAll02));
      JCAll02SD.addVector("AllJet02SDZG",      SDAll02.getZgs());
      JCAll02SD.addVector("AllJet02SDNBranch", SDAll02.getNDroppedSubjets());
      JCAll02SD.addVector("AllJet02SDDR12",    SDAll02.getDR12());
      JCAll02SD.addVector("AllJet02SDSubjet1", SDAll02.getSubjets1());
      JCAll02SD.addVector("AllJet02SDSubjet2", SDAll02.getSubjets2());
      jetCollection JCAll04SD(SDAll04.doGrooming(JCAll04));
      JCAll04SD.addVector("AllJet04SDZG",      SDAll04.getZgs());
      JCAll04SD.addVector("AllJet04SDNBranch", SDAll04.getNDroppedSubjets());
      JCAll04SD.addVector("AllJet04SDDR12",    SDAll04.getDR12());
      JCAll04SD.addVector("AllJet04SDSubjet1", SDAll04.getSubjets1());
      JCAll04SD.addVector("AllJet04SDSubjet2", SDAll04.getSubjets2());
      jetCollection JCAll06SD(SDAll06.doGrooming(JCAll06));
      JCAll06SD.addVector("AllJet06SDZG",      SDAll06.getZgs());
      JCAll06SD.addVector("AllJet06SDNBranch", SDAll06.getNDroppedSubjets());
      JCAll06SD.addVector("AllJet06SDDR12",    SDAll06.getDR12());
      JCAll06SD.addVector("AllJet06SDSubjet1", SDAll06.getSubjets1());
      JCAll06SD.addVector("AllJet06SDSubjet2", SDAll06.getSubjets2());
      jetCollection JCAll08SD(SDAll08.doGrooming(JCAll08));
      JCAll08SD.addVector("AllJet08SDZG",      SDAll08.getZgs());
      JCAll08SD.addVector("AllJet08SDNBranch", SDAll08.getNDroppedSubjets());
      JCAll08SD.addVector("AllJet08SDDR12",    SDAll08.getDR12());
      JCAll08SD.addVector("AllJet08SDSubjet1", SDAll08.getSubjets1());
      JCAll08SD.addVector("AllJet08SDSubjet2", SDAll08.getSubjets2());
      jetCollection JCAll10SD(SDAll10.doGrooming(JCAll10));
      JCAll10SD.addVector("AllJet10SDZG",      SDAll10.getZgs());
      JCAll10SD.addVector("AllJet10SDNBranch", SDAll10.getNDroppedSubjets());
      JCAll10SD.addVector("AllJet10SDDR12",    SDAll10.getDR12());
      JCAll10SD.addVector("AllJet10SDSubjet1", SDAll10.getSubjets1());
      JCAll10SD.addVector("AllJet10SDSubjet2", SDAll10.getSubjets2());

      jetCollection JCAll02SDJewel(GetCorrectedJets(SDAll02.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All02SDJewel
         = GetCorrectedSubJets(SDAll02.getConstituents1(), SDAll02.getConstituents2(), ParticlesDummy);
      JCAll02SDJewel.addVector("AllJet02SDJewelZG",      CalculateZG(All02SDJewel));
      JCAll02SDJewel.addVector("AllJet02SDJewelDR12",    CalculateDR(All02SDJewel));
      JCAll02SDJewel.addVector("AllJet02SDJewelSubjet1", CalculateSubjet1(All02SDJewel));
      JCAll02SDJewel.addVector("AllJet02SDJewelSubjet2", CalculateSubjet2(All02SDJewel));
      jetCollection JCAll04SDJewel(GetCorrectedJets(SDAll04.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All04SDJewel
         = GetCorrectedSubJets(SDAll04.getConstituents1(), SDAll04.getConstituents2(), ParticlesDummy);
      JCAll04SDJewel.addVector("AllJet04SDJewelZG",      CalculateZG(All04SDJewel));
      JCAll04SDJewel.addVector("AllJet04SDJewelDR12",    CalculateDR(All04SDJewel));
      JCAll04SDJewel.addVector("AllJet04SDJewelSubjet1", CalculateSubjet1(All04SDJewel));
      JCAll04SDJewel.addVector("AllJet04SDJewelSubjet2", CalculateSubjet2(All04SDJewel));
      jetCollection JCAll06SDJewel(GetCorrectedJets(SDAll06.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All06SDJewel
         = GetCorrectedSubJets(SDAll06.getConstituents1(), SDAll06.getConstituents2(), ParticlesDummy);
      JCAll06SDJewel.addVector("AllJet06SDJewelZG",      CalculateZG(All06SDJewel));
      JCAll06SDJewel.addVector("AllJet06SDJewelDR12",    CalculateDR(All06SDJewel));
      JCAll06SDJewel.addVector("AllJet06SDJewelSubjet1", CalculateSubjet1(All06SDJewel));
      JCAll06SDJewel.addVector("AllJet06SDJewelSubjet2", CalculateSubjet2(All06SDJewel));
      jetCollection JCAll08SDJewel(GetCorrectedJets(SDAll08.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All08SDJewel
         = GetCorrectedSubJets(SDAll08.getConstituents1(), SDAll08.getConstituents2(), ParticlesDummy);
      JCAll08SDJewel.addVector("AllJet08SDJewelZG",      CalculateZG(All08SDJewel));
      JCAll08SDJewel.addVector("AllJet08SDJewelDR12",    CalculateDR(All08SDJewel));
      JCAll08SDJewel.addVector("AllJet08SDJewelSubjet1", CalculateSubjet1(All08SDJewel));
      JCAll08SDJewel.addVector("AllJet08SDJewelSubjet2", CalculateSubjet2(All08SDJewel));
      jetCollection JCAll10SDJewel(GetCorrectedJets(SDAll10.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> All10SDJewel
         = GetCorrectedSubJets(SDAll10.getConstituents1(), SDAll10.getConstituents2(), ParticlesDummy);
      JCAll10SDJewel.addVector("AllJet10SDJewelZG",      CalculateZG(All10SDJewel));
      JCAll10SDJewel.addVector("AllJet10SDJewelDR12",    CalculateDR(All10SDJewel));
      JCAll10SDJewel.addVector("AllJet10SDJewelSubjet1", CalculateSubjet1(All10SDJewel));
      JCAll10SDJewel.addVector("AllJet10SDJewelSubjet2", CalculateSubjet2(All10SDJewel));

      // XXX
      jetCollection WTAJCSignal02SD(WTASDSignal02.doGrooming(WTAJCSignal02));
      WTAJCSignal02SD.addVector("WTASignalJet02SDZG",      WTASDSignal02.getZgs());
      WTAJCSignal02SD.addVector("WTASignalJet02SDNBranch", WTASDSignal02.getNDroppedSubjets());
      WTAJCSignal02SD.addVector("WTASignalJet02SDDR12",    WTASDSignal02.getDR12());
      WTAJCSignal02SD.addVector("WTASignalJet02SDSubjet1", WTASDSignal02.getSubjets1());
      WTAJCSignal02SD.addVector("WTASignalJet02SDSubjet2", WTASDSignal02.getSubjets2());
      jetCollection WTAJCSignal04SD(WTASDSignal04.doGrooming(WTAJCSignal04));
      WTAJCSignal04SD.addVector("WTASignalJet04SDZG",      WTASDSignal04.getZgs());
      WTAJCSignal04SD.addVector("WTASignalJet04SDNBranch", WTASDSignal04.getNDroppedSubjets());
      WTAJCSignal04SD.addVector("WTASignalJet04SDDR12",    WTASDSignal04.getDR12());
      WTAJCSignal04SD.addVector("WTASignalJet04SDSubjet1", WTASDSignal04.getSubjets1());
      WTAJCSignal04SD.addVector("WTASignalJet04SDSubjet2", WTASDSignal04.getSubjets2());
      jetCollection WTAJCSignal06SD(WTASDSignal06.doGrooming(WTAJCSignal06));
      WTAJCSignal06SD.addVector("WTASignalJet06SDZG",      WTASDSignal06.getZgs());
      WTAJCSignal06SD.addVector("WTASignalJet06SDNBranch", WTASDSignal06.getNDroppedSubjets());
      WTAJCSignal06SD.addVector("WTASignalJet06SDDR12",    WTASDSignal06.getDR12());
      WTAJCSignal06SD.addVector("WTASignalJet06SDSubjet1", WTASDSignal06.getSubjets1());
      WTAJCSignal06SD.addVector("WTASignalJet06SDSubjet2", WTASDSignal06.getSubjets2());
      jetCollection WTAJCSignal08SD(WTASDSignal08.doGrooming(WTAJCSignal08));
      WTAJCSignal08SD.addVector("WTASignalJet08SDZG",      WTASDSignal08.getZgs());
      WTAJCSignal08SD.addVector("WTASignalJet08SDNBranch", WTASDSignal08.getNDroppedSubjets());
      WTAJCSignal08SD.addVector("WTASignalJet08SDDR12",    WTASDSignal08.getDR12());
      WTAJCSignal08SD.addVector("WTASignalJet08SDSubjet1", WTASDSignal08.getSubjets1());
      WTAJCSignal08SD.addVector("WTASignalJet08SDSubjet2", WTASDSignal08.getSubjets2());
      jetCollection WTAJCSignal10SD(WTASDSignal10.doGrooming(WTAJCSignal10));
      WTAJCSignal10SD.addVector("WTASignalJet10SDZG",      WTASDSignal10.getZgs());
      WTAJCSignal10SD.addVector("WTASignalJet10SDNBranch", WTASDSignal10.getNDroppedSubjets());
      WTAJCSignal10SD.addVector("WTASignalJet10SDDR12",    WTASDSignal10.getDR12());
      WTAJCSignal10SD.addVector("WTASignalJet10SDSubjet1", WTASDSignal10.getSubjets1());
      WTAJCSignal10SD.addVector("WTASignalJet10SDSubjet2", WTASDSignal10.getSubjets2());

      jetCollection WTAJCSignal02SDJewel(GetCorrectedJets(WTASDSignal02.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTASignal02SDJewel
         = GetCorrectedSubJets(WTASDSignal02.getConstituents1(), WTASDSignal02.getConstituents2(), ParticlesDummy);
      WTAJCSignal02SDJewel.addVector("WTASignalJet02SDJewelZG",      CalculateZG(WTASignal02SDJewel));
      WTAJCSignal02SDJewel.addVector("WTASignalJet02SDJewelDR12",    CalculateDR(WTASignal02SDJewel));
      WTAJCSignal02SDJewel.addVector("WTASignalJet02SDJewelSubjet1", CalculateSubjet1(WTASignal02SDJewel));
      WTAJCSignal02SDJewel.addVector("WTASignalJet02SDJewelSubjet2", CalculateSubjet2(WTASignal02SDJewel));
      jetCollection WTAJCSignal04SDJewel(GetCorrectedJets(WTASDSignal04.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTASignal04SDJewel
         = GetCorrectedSubJets(WTASDSignal04.getConstituents1(), WTASDSignal04.getConstituents2(), ParticlesDummy);
      WTAJCSignal04SDJewel.addVector("WTASignalJet04SDJewelZG",      CalculateZG(WTASignal04SDJewel));
      WTAJCSignal04SDJewel.addVector("WTASignalJet04SDJewelDR12",    CalculateDR(WTASignal04SDJewel));
      WTAJCSignal04SDJewel.addVector("WTASignalJet04SDJewelSubjet1", CalculateSubjet1(WTASignal04SDJewel));
      WTAJCSignal04SDJewel.addVector("WTASignalJet04SDJewelSubjet2", CalculateSubjet2(WTASignal04SDJewel));
      jetCollection WTAJCSignal06SDJewel(GetCorrectedJets(WTASDSignal06.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTASignal06SDJewel
         = GetCorrectedSubJets(WTASDSignal06.getConstituents1(), WTASDSignal06.getConstituents2(), ParticlesDummy);
      WTAJCSignal06SDJewel.addVector("WTASignalJet06SDJewelZG",      CalculateZG(WTASignal06SDJewel));
      WTAJCSignal06SDJewel.addVector("WTASignalJet06SDJewelDR12",    CalculateDR(WTASignal06SDJewel));
      WTAJCSignal06SDJewel.addVector("WTASignalJet06SDJewelSubjet1", CalculateSubjet1(WTASignal06SDJewel));
      WTAJCSignal06SDJewel.addVector("WTASignalJet06SDJewelSubjet2", CalculateSubjet2(WTASignal06SDJewel));
      jetCollection WTAJCSignal08SDJewel(GetCorrectedJets(WTASDSignal08.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTASignal08SDJewel
         = GetCorrectedSubJets(WTASDSignal08.getConstituents1(), WTASDSignal08.getConstituents2(), ParticlesDummy);
      WTAJCSignal08SDJewel.addVector("WTASignalJet08SDJewelZG",      CalculateZG(WTASignal08SDJewel));
      WTAJCSignal08SDJewel.addVector("WTASignalJet08SDJewelDR12",    CalculateDR(WTASignal08SDJewel));
      WTAJCSignal08SDJewel.addVector("WTASignalJet08SDJewelSubjet1", CalculateSubjet1(WTASignal08SDJewel));
      WTAJCSignal08SDJewel.addVector("WTASignalJet08SDJewelSubjet2", CalculateSubjet2(WTASignal08SDJewel));
      jetCollection WTAJCSignal10SDJewel(GetCorrectedJets(WTASDSignal10.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTASignal10SDJewel
         = GetCorrectedSubJets(WTASDSignal10.getConstituents1(), WTASDSignal10.getConstituents2(), ParticlesDummy);
      WTAJCSignal10SDJewel.addVector("WTASignalJet10SDJewelZG",      CalculateZG(WTASignal10SDJewel));
      WTAJCSignal10SDJewel.addVector("WTASignalJet10SDJewelDR12",    CalculateDR(WTASignal10SDJewel));
      WTAJCSignal10SDJewel.addVector("WTASignalJet10SDJewelSubjet1", CalculateSubjet1(WTASignal10SDJewel));
      WTAJCSignal10SDJewel.addVector("WTASignalJet10SDJewelSubjet2", CalculateSubjet2(WTASignal10SDJewel));

      jetCollection WTAJCAll02SD(WTASDAll02.doGrooming(WTAJCAll02));
      WTAJCAll02SD.addVector("WTAAllJet02SDZG",      WTASDAll02.getZgs());
      WTAJCAll02SD.addVector("WTAAllJet02SDNBranch", WTASDAll02.getNDroppedSubjets());
      WTAJCAll02SD.addVector("WTAAllJet02SDDR12",    WTASDAll02.getDR12());
      WTAJCAll02SD.addVector("WTAAllJet02SDSubjet1", WTASDAll02.getSubjets1());
      WTAJCAll02SD.addVector("WTAAllJet02SDSubjet2", WTASDAll02.getSubjets2());
      jetCollection WTAJCAll04SD(WTASDAll04.doGrooming(WTAJCAll04));
      WTAJCAll04SD.addVector("WTAAllJet04SDZG",      WTASDAll04.getZgs());
      WTAJCAll04SD.addVector("WTAAllJet04SDNBranch", WTASDAll04.getNDroppedSubjets());
      WTAJCAll04SD.addVector("WTAAllJet04SDDR12",    WTASDAll04.getDR12());
      WTAJCAll04SD.addVector("WTAAllJet04SDSubjet1", WTASDAll04.getSubjets1());
      WTAJCAll04SD.addVector("WTAAllJet04SDSubjet2", WTASDAll04.getSubjets2());
      jetCollection WTAJCAll06SD(WTASDAll06.doGrooming(WTAJCAll06));
      WTAJCAll06SD.addVector("WTAAllJet06SDZG",      WTASDAll06.getZgs());
      WTAJCAll06SD.addVector("WTAAllJet06SDNBranch", WTASDAll06.getNDroppedSubjets());
      WTAJCAll06SD.addVector("WTAAllJet06SDDR12",    WTASDAll06.getDR12());
      WTAJCAll06SD.addVector("WTAAllJet06SDSubjet1", WTASDAll06.getSubjets1());
      WTAJCAll06SD.addVector("WTAAllJet06SDSubjet2", WTASDAll06.getSubjets2());
      jetCollection WTAJCAll08SD(WTASDAll08.doGrooming(WTAJCAll08));
      WTAJCAll08SD.addVector("WTAAllJet08SDZG",      WTASDAll08.getZgs());
      WTAJCAll08SD.addVector("WTAAllJet08SDNBranch", WTASDAll08.getNDroppedSubjets());
      WTAJCAll08SD.addVector("WTAAllJet08SDDR12",    WTASDAll08.getDR12());
      WTAJCAll08SD.addVector("WTAAllJet08SDSubjet1", WTASDAll08.getSubjets1());
      WTAJCAll08SD.addVector("WTAAllJet08SDSubjet2", WTASDAll08.getSubjets2());
      jetCollection WTAJCAll10SD(WTASDAll10.doGrooming(WTAJCAll10));
      WTAJCAll10SD.addVector("WTAAllJet10SDZG",      WTASDAll10.getZgs());
      WTAJCAll10SD.addVector("WTAAllJet10SDNBranch", WTASDAll10.getNDroppedSubjets());
      WTAJCAll10SD.addVector("WTAAllJet10SDDR12",    WTASDAll10.getDR12());
      WTAJCAll10SD.addVector("WTAAllJet10SDSubjet1", WTASDAll10.getSubjets1());
      WTAJCAll10SD.addVector("WTAAllJet10SDSubjet2", WTASDAll10.getSubjets2());

      jetCollection WTAJCAll02SDJewel(GetCorrectedJets(WTASDAll02.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTAAll02SDJewel
         = GetCorrectedSubJets(WTASDAll02.getConstituents1(), WTASDAll02.getConstituents2(), ParticlesDummy);
      WTAJCAll02SDJewel.addVector("WTAAllJet02SDJewelZG",      CalculateZG(WTAAll02SDJewel));
      WTAJCAll02SDJewel.addVector("WTAAllJet02SDJewelDR12",    CalculateDR(WTAAll02SDJewel));
      WTAJCAll02SDJewel.addVector("WTAAllJet02SDJewelSubjet1", CalculateSubjet1(WTAAll02SDJewel));
      WTAJCAll02SDJewel.addVector("WTAAllJet02SDJewelSubjet2", CalculateSubjet2(WTAAll02SDJewel));
      jetCollection WTAJCAll04SDJewel(GetCorrectedJets(WTASDAll04.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTAAll04SDJewel
         = GetCorrectedSubJets(WTASDAll04.getConstituents1(), WTASDAll04.getConstituents2(), ParticlesDummy);
      WTAJCAll04SDJewel.addVector("WTAAllJet04SDJewelZG",      CalculateZG(WTAAll04SDJewel));
      WTAJCAll04SDJewel.addVector("WTAAllJet04SDJewelDR12",    CalculateDR(WTAAll04SDJewel));
      WTAJCAll04SDJewel.addVector("WTAAllJet04SDJewelSubjet1", CalculateSubjet1(WTAAll04SDJewel));
      WTAJCAll04SDJewel.addVector("WTAAllJet04SDJewelSubjet2", CalculateSubjet2(WTAAll04SDJewel));
      jetCollection WTAJCAll06SDJewel(GetCorrectedJets(WTASDAll06.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTAAll06SDJewel
         = GetCorrectedSubJets(WTASDAll06.getConstituents1(), WTASDAll06.getConstituents2(), ParticlesDummy);
      WTAJCAll06SDJewel.addVector("WTAAllJet06SDJewelZG",      CalculateZG(WTAAll06SDJewel));
      WTAJCAll06SDJewel.addVector("WTAAllJet06SDJewelDR12",    CalculateDR(WTAAll06SDJewel));
      WTAJCAll06SDJewel.addVector("WTAAllJet06SDJewelSubjet1", CalculateSubjet1(WTAAll06SDJewel));
      WTAJCAll06SDJewel.addVector("WTAAllJet06SDJewelSubjet2", CalculateSubjet2(WTAAll06SDJewel));
      jetCollection WTAJCAll08SDJewel(GetCorrectedJets(WTASDAll08.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTAAll08SDJewel
         = GetCorrectedSubJets(WTASDAll08.getConstituents1(), WTASDAll08.getConstituents2(), ParticlesDummy);
      WTAJCAll08SDJewel.addVector("WTAAllJet08SDJewelZG",      CalculateZG(WTAAll08SDJewel));
      WTAJCAll08SDJewel.addVector("WTAAllJet08SDJewelDR12",    CalculateDR(WTAAll08SDJewel));
      WTAJCAll08SDJewel.addVector("WTAAllJet08SDJewelSubjet1", CalculateSubjet1(WTAAll08SDJewel));
      WTAJCAll08SDJewel.addVector("WTAAllJet08SDJewelSubjet2", CalculateSubjet2(WTAAll08SDJewel));
      jetCollection WTAJCAll10SDJewel(GetCorrectedJets(WTASDAll10.getConstituents(), ParticlesDummy));
      vector<pair<PseudoJet, PseudoJet>> WTAAll10SDJewel
         = GetCorrectedSubJets(WTASDAll10.getConstituents1(), WTASDAll10.getConstituents2(), ParticlesDummy);
      WTAJCAll10SDJewel.addVector("WTAAllJet10SDJewelZG",      CalculateZG(WTAAll10SDJewel));
      WTAJCAll10SDJewel.addVector("WTAAllJet10SDJewelDR12",    CalculateDR(WTAAll10SDJewel));
      WTAJCAll10SDJewel.addVector("WTAAllJet10SDJewelSubjet1", CalculateSubjet1(WTAAll10SDJewel));
      WTAJCAll10SDJewel.addVector("WTAAllJet10SDJewelSubjet2", CalculateSubjet2(WTAAll10SDJewel));

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

      Writer.addCollection("WTASignalJet02",        WTAJCSignal02);
      Writer.addCollection("WTASignalJet02Jewel",   WTAJCSignal02Jewel);
      Writer.addCollection("WTASignalJet02SD",      WTAJCSignal02SD);
      Writer.addCollection("WTASignalJet02SDJewel", WTAJCSignal02SDJewel);
      Writer.addCollection("WTASignalJet04",        WTAJCSignal04);
      Writer.addCollection("WTASignalJet04Jewel",   WTAJCSignal04Jewel);
      Writer.addCollection("WTASignalJet04SD",      WTAJCSignal04SD);
      Writer.addCollection("WTASignalJet04SDJewel", WTAJCSignal04SDJewel);
      Writer.addCollection("WTASignalJet06",        WTAJCSignal06);
      Writer.addCollection("WTASignalJet06Jewel",   WTAJCSignal06Jewel);
      Writer.addCollection("WTASignalJet06SD",      WTAJCSignal06SD);
      Writer.addCollection("WTASignalJet06SDJewel", WTAJCSignal06SDJewel);
      Writer.addCollection("WTASignalJet08",        WTAJCSignal08);
      Writer.addCollection("WTASignalJet08Jewel",   WTAJCSignal08Jewel);
      Writer.addCollection("WTASignalJet08SD",      WTAJCSignal08SD);
      Writer.addCollection("WTASignalJet08SDJewel", WTAJCSignal08SDJewel);
      Writer.addCollection("WTASignalJet10",        WTAJCSignal10);
      Writer.addCollection("WTASignalJet10Jewel",   WTAJCSignal10Jewel);
      Writer.addCollection("WTASignalJet10SD",      WTAJCSignal10SD);
      Writer.addCollection("WTASignalJet10SDJewel", WTAJCSignal10SDJewel);

      Writer.addCollection("WTAAllJet02",           WTAJCAll02);
      Writer.addCollection("WTAAllJet02Jewel",      WTAJCAll02Jewel);
      Writer.addCollection("WTAAllJet02SD",         WTAJCAll02SD);
      Writer.addCollection("WTAAllJet02SDJewel",    WTAJCAll02SDJewel);
      Writer.addCollection("WTAAllJet04",           WTAJCAll04);
      Writer.addCollection("WTAAllJet04Jewel",      WTAJCAll04Jewel);
      Writer.addCollection("WTAAllJet04SD",         WTAJCAll04SD);
      Writer.addCollection("WTAAllJet04SDJewel",    WTAJCAll04SDJewel);
      Writer.addCollection("WTAAllJet06",           WTAJCAll06);
      Writer.addCollection("WTAAllJet06Jewel",      WTAJCAll06Jewel);
      Writer.addCollection("WTAAllJet06SD",         WTAJCAll06SD);
      Writer.addCollection("WTAAllJet06SDJewel",    WTAJCAll06SDJewel);
      Writer.addCollection("WTAAllJet08",           WTAJCAll08);
      Writer.addCollection("WTAAllJet08Jewel",      WTAJCAll08Jewel);
      Writer.addCollection("WTAAllJet08SD",         WTAJCAll08SD);
      Writer.addCollection("WTAAllJet08SDJewel",    WTAJCAll08SDJewel);
      Writer.addCollection("WTAAllJet10",           WTAJCAll10);
      Writer.addCollection("WTAAllJet10Jewel",      WTAJCAll10Jewel);
      Writer.addCollection("WTAAllJet10SD",         WTAJCAll10SD);
      Writer.addCollection("WTAAllJet10SDJewel",    WTAJCAll10SDJewel);

      Writer.addCollection("HemisphereSignal", HemisphereJetSignal);
      Writer.addCollection("HemisphereAll",    HemisphereJetAll);

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
