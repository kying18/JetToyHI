#include <iostream>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "TLorentzVector.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("input", " ");
   string OutputBase = CL.Get("output");
   double MinPT = CL.GetDouble("minpt", 300);
   string title = CL.Get("title","");

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("ZJet Plane");

   TFile *outf = new TFile(Form("%s.root",OutputBase.c_str()),"recreate");

   TH1D HJetPT("HJetPT", Form("%s Jet PT (Jet Signal Only)",title.c_str()), 200, 0, 1000);
   TH1D HJetSDPT("HJetSDPT", Form("%s Jet SD PT (Jet Signal Only)",title.c_str()), 200, 0, 1000);
   TH1D HAllJetPT("HAllJetPT", Form ("%s Jet PT (Signal+Background)",title.c_str()), 200, 0, 1000);
   TH1D HAllJetSDPT("HAllJetSDPT", Form("%s Jet SD PT (Signal+Background)",title.c_str()), 200, 0, 1000);
   TH1D HZJetDPhi("HZJetDPhi", Form("%s All Hadrons;#Delta#Phi",title.c_str()), 100,0,3.1415926535897932384636);
   TH1D HZJetDPhiSub("HZJetDPhiSub", Form("%s All Hadrons Subtracted;#Delta#Phi",title.c_str()), 100,0,3.1415926535897932384636);
   TH2D HZJetDPhiDEta("HZJetDPhiDEta", Form("%s All Hadrons;#Delta#Phi;#Delta#eta",title.c_str()), 100,0,3.1415926535897932384636,100,0,5);
   TH2D HZJetDPhiDEtaSub("HZJetDPhiDEtaSub", Form("%s All Hadrons Subtracted;#Delta#Phi;#Delta#eta",title.c_str()), 100,0,3.1415926535897932384636,100,0,5);

   HZJetDPhi.SetStats(0);
   HZJetDPhiDEta.SetStats(0);
   HZJetDPhiSub.SetStats(0);
   HZJetDPhiDEtaSub.SetStats(0);
   HZJetDPhi.GetYaxis()->SetRange(0,10);
   HZJetDPhiSub.GetYaxis()->SetRange(0,10);

   double nZBosonSelected=0;

   for(string FileName : InputFileNames)
   {
      cout <<InputFileNames.size()<<endl;
      TFile File(FileName.c_str());
      cout <<FileName.c_str()<<endl;
      TTree *Tree = (TTree *)File.Get("JetTree");

      if(Tree == nullptr)
      {
         File.Close();
         continue;
      }

      vector<double> *SignalJetPt = nullptr;
      vector<double> *SignalJetSDPt = nullptr;
      vector<double> *SignalJetEta = nullptr;
      vector<double> *SignalJetPhi = nullptr;
      vector<double> *AllJetPt = nullptr;
      vector<double> *AllJetSDPt = nullptr;
      vector<double> *AllJetEta = nullptr;
      vector<double> *AllJetPhi = nullptr;
      vector<double> *ParticlesPt = nullptr;
      vector<double> *ParticlesEta = nullptr;
      vector<double> *ParticlesPhi = nullptr;
      vector<double> *ParticlesM = nullptr;
      vector<double> *LeadingZBosonPt = nullptr;
      vector<double> *LeadingZBosonEta = nullptr;
      vector<double> *LeadingZBosonPhi = nullptr;
      vector<double> *LeadingZBosonM = nullptr;
      vector<double> *DummyPt = nullptr;
      vector<double> *DummyEta = nullptr;
      vector<double> *DummyPhi = nullptr;
      vector<double> *DummyM = nullptr;
      vector<bool> *isHadron = nullptr;
      vector<vector<double>> *ConstituentPt = nullptr;
      vector<vector<double>> *ConstituentEta = nullptr;
      vector<vector<double>> *ConstituentPhi = nullptr;
      vector<vector<double>> *ConstituentM = nullptr;
      vector<vector<double>> *ConstituentIsHadron = nullptr;
      vector<vector<double>> *SDConstituentPt = nullptr;
      vector<vector<double>> *SDConstituentEta = nullptr;
      vector<vector<double>> *SDConstituentPhi = nullptr;
      vector<vector<double>> *SDConstituentM = nullptr;
      vector<vector<double>> *SDConstituentIsHadron = nullptr;
      vector<vector<double>> *AllConstituentPt = nullptr;
      vector<vector<double>> *AllConstituentEta = nullptr;
      vector<vector<double>> *AllConstituentPhi = nullptr;
      vector<vector<double>> *AllConstituentM = nullptr;
      vector<vector<double>> *AllConstituentIsHadron = nullptr;
      vector<vector<double>> *SDAllConstituentPt = nullptr;
      vector<vector<double>> *SDAllConstituentEta = nullptr;
      vector<vector<double>> *SDAllConstituentPhi = nullptr;
      vector<vector<double>> *SDAllConstituentM = nullptr;
      vector<vector<double>> *SDAllConstituentIsHadron = nullptr;
      vector<double> *EventWeight = nullptr;

      Tree->SetBranchAddress("SignalJet04JewelPt", &SignalJetPt);
      Tree->SetBranchAddress("SignalJet04SDJewelPt", &SignalJetSDPt);
      Tree->SetBranchAddress("SignalJet04JewelEta", &SignalJetEta);
      Tree->SetBranchAddress("SignalJet04JewelPhi", &SignalJetPhi);
      Tree->SetBranchAddress("AllJet04JewelPt", &AllJetPt);
      Tree->SetBranchAddress("AllJet04SDJewelPt", &AllJetSDPt);
      Tree->SetBranchAddress("AllJet04JewelEta", &AllJetEta);
      Tree->SetBranchAddress("AllJet04JewelPhi", &AllJetPhi);
      Tree->SetBranchAddress("ParticlesPt", &ParticlesPt);
      Tree->SetBranchAddress("ParticlesEta", &ParticlesEta);
      Tree->SetBranchAddress("ParticlesPhi", &ParticlesPhi);
      Tree->SetBranchAddress("ParticlesM", &ParticlesM);
      Tree->SetBranchAddress("LeadingZBosonPt", &LeadingZBosonPt);
      Tree->SetBranchAddress("LeadingZBosonEta", &LeadingZBosonEta);
      Tree->SetBranchAddress("LeadingZBosonPhi", &LeadingZBosonPhi);
      Tree->SetBranchAddress("LeadingZBosonM", &LeadingZBosonM);
      Tree->SetBranchAddress("DummyPt", &DummyPt);
      Tree->SetBranchAddress("DummyEta", &DummyEta);
      Tree->SetBranchAddress("DummyPhi", &DummyPhi);
      Tree->SetBranchAddress("DummyM", &DummyM);
      Tree->SetBranchAddress("isHadron", &isHadron);
      Tree->SetBranchAddress("SignalJet04ConstituentPt", &ConstituentPt);
      Tree->SetBranchAddress("SignalJet04ConstituentEta", &ConstituentEta);
      Tree->SetBranchAddress("SignalJet04ConstituentPhi", &ConstituentPhi);
      Tree->SetBranchAddress("SignalJet04ConstituentMass", &ConstituentM);
      Tree->SetBranchAddress("SignalJet04ConstituentIsHadron", &ConstituentIsHadron);
      Tree->SetBranchAddress("SignalJet04SDConstituentPt", &SDConstituentPt);
      Tree->SetBranchAddress("SignalJet04SDConstituentEta", &SDConstituentEta);
      Tree->SetBranchAddress("SignalJet04SDConstituentPhi", &SDConstituentPhi);
      Tree->SetBranchAddress("SignalJet04SDConstituentMass", &SDConstituentM);
      Tree->SetBranchAddress("SignalJet04SDConstituentIsHadron", &SDConstituentIsHadron);
      Tree->SetBranchAddress("AllJet04ConstituentPt", &AllConstituentPt);
      Tree->SetBranchAddress("AllJet04ConstituentEta", &AllConstituentEta);
      Tree->SetBranchAddress("AllJet04ConstituentPhi", &AllConstituentPhi);
      Tree->SetBranchAddress("AllJet04ConstituentMass", &AllConstituentM);
      Tree->SetBranchAddress("AllJet04ConstituentIsHadron", &AllConstituentIsHadron);
      Tree->SetBranchAddress("AllJet04SDConstituentPt", &SDAllConstituentPt);
      Tree->SetBranchAddress("AllJet04SDConstituentEta", &SDAllConstituentEta);
      Tree->SetBranchAddress("AllJet04SDConstituentPhi", &SDAllConstituentPhi);
      Tree->SetBranchAddress("AllJet04SDConstituentMass", &SDAllConstituentM);
      Tree->SetBranchAddress("AllJet04SDConstituentIsHadron", &SDAllConstituentIsHadron);
      Tree->SetBranchAddress("EventWeight", &EventWeight);

      int EntryCount = Tree->GetEntries();
      for(int iE = 0; iE < EntryCount; iE++)
      {
         Tree->GetEntry(iE);

         if(LeadingZBosonPt == nullptr)
            continue;

         int NZBoson = LeadingZBosonPt->size();
         for(int iJ = 0; iJ < NZBoson; iJ++)
         {
            HJetPT.Fill((*LeadingZBosonPt)[iJ],(*EventWeight)[0]);
            if((*LeadingZBosonEta)[iJ] < -2 || (*LeadingZBosonEta)[iJ] > 2)
               continue;
            if((*LeadingZBosonPt)[iJ] < MinPT)
               continue;
            nZBosonSelected+=(*EventWeight)[0];
            TLorentzVector j;
            j.SetPtEtaPhiM( (*LeadingZBosonPt)[iJ], (*LeadingZBosonEta)[iJ], (*LeadingZBosonPhi)[iJ], 0);
            for(int iS = 0; iS < (*ParticlesPt).size(); iS++)
            {
               if ((*isHadron)[iS]==0) continue;
               TLorentzVector p;
               p.SetPtEtaPhiM( (*ParticlesPt)[iS], (*ParticlesEta)[iS], (*ParticlesPhi)[iS], (*ParticlesM)[iS]);
               double W = 1*(*EventWeight)[0];
               if((*ParticlesPt)[iS] < 1e-5)
               {
                  for(int iD = 0; iD < (*DummyPt).size(); iD++)
                  {
                     TLorentzVector d;
                     d.SetPtEtaPhiM( (*DummyPt)[iS], (*DummyEta)[iS], (*DummyPhi)[iS], (*DummyM)[iS]);
                     if(p.DeltaR(d) < 1e-5)
                     {
                        p = d;
                        W = -1*(*EventWeight)[0];
                        break;
                     }
                  }
               }
               double dPhi = fabs(j.DeltaPhi(p));
               double dEta = fabs(j.Eta()-p.Eta());
               if (W>0) HZJetDPhi.Fill(dPhi, W);
               HZJetDPhiSub.Fill(dPhi, W);
               if (W>0) HZJetDPhiDEta.Fill(dPhi,dEta, W);
               HZJetDPhiDEtaSub.Fill(dPhi,dEta, W);
            }

         }
      }

      File.Close();
   }

   HZJetDPhi.Scale(1./(double)nZBosonSelected);
   HZJetDPhiSub.Scale(1./(double)nZBosonSelected);
   HZJetDPhiDEta.Scale(1./(double)nZBosonSelected);
   HZJetDPhiDEtaSub.Scale(1./(double)nZBosonSelected);
   
   PdfFile.AddPlot(HJetPT, "", true);
   PdfFile.AddPlot(HAllJetPT, "", true);
   PdfFile.AddPlot(HJetSDPT, "", true);
   PdfFile.AddPlot(HAllJetSDPT, "", true);
   PdfFile.AddPlot(HZJetDPhi, "",false);
   PdfFile.AddPlot(HZJetDPhiSub, "",false);
   PdfFile.AddPlot(HZJetDPhiDEta, "colz",false);
   PdfFile.AddPlot(HZJetDPhiDEtaSub, "colz",false);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();
   outf->cd();
   HZJetDPhi.Write();
   HZJetDPhiDEta.Write();
   HZJetDPhiSub.Write();
   HZJetDPhiDEtaSub.Write();
   return 0;
}




