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

   vector<string> InputFileNames = CL.GetStringVector("input");
   string OutputBase = CL.Get("output");
   double MinPT = CL.GetDouble("minpt", 100);

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("EMMI Plane");

   TH1D HJetPT("HJetPT", "Jet PT (Jet Signal Only)", 200, 0, 500);
   TH1D HJetSDPT("HJetSDPT", "Jet SD PT (Jet Signal Only)", 200, 0, 500);
   TH1D HAllJetPT("HAllJetPT", "Jet PT (Signal+Background)", 200, 0, 500);
   TH1D HAllJetSDPT("HAllJetSDPT", "Jet SD PT (Signal+Background)", 200, 0, 500);
   TH2D HEMMI("HEMMI", "All Hadrons;#Delta#Theta;ln(z)", 100, 0, 1, 100, -10, 0);
   TH2D HEMMIConstituent("HEMMIConstituent", "Constituent Hadrons (Jet Signal Only);#Delta#Theta;ln(z)", 100, 0, 1, 100, -10, 0);
   TH2D HEMMISDConstituent("HEMMISDConstituent", "SD Constituent Hadrons (Jet Signal Only);#Delta#Theta;ln(z)", 100, 0, 1, 100, -10, 0);
   TH2D HEMMIAllConstituent("HEMMIAllConstituent", "Constituent Hadrons (Signal+Background);#Delta#Theta;ln(z)", 100, 0, 1, 100, -10, 0);
   TH2D HEMMISDAllConstituent("HEMMISDAllConstituent", "SD Constituent Hadrons (Signal+Background;#Delta#Theta;ln(z)", 100, 0, 1, 100, -10, 0);
   
   HEMMI.SetStats(0);
   HEMMIConstituent.SetStats(0);
   HEMMISDConstituent.SetStats(0);
   HEMMIAllConstituent.SetStats(0);
   HEMMISDAllConstituent.SetStats(0);

   for(string FileName : InputFileNames)
   {
      TFile File(FileName.c_str());

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

      Tree->SetBranchAddress("SignalJet04Pt", &SignalJetPt);
      Tree->SetBranchAddress("SignalJet04SDPt", &SignalJetSDPt);
      Tree->SetBranchAddress("SignalJet04Eta", &SignalJetEta);
      Tree->SetBranchAddress("SignalJet04Phi", &SignalJetPhi);
      Tree->SetBranchAddress("AllJet04Pt", &AllJetPt);
      Tree->SetBranchAddress("AllJet04SDPt", &AllJetSDPt);
      Tree->SetBranchAddress("AllJet04Eta", &AllJetEta);
      Tree->SetBranchAddress("AllJet04Phi", &AllJetPhi);
      Tree->SetBranchAddress("ParticlesPt", &ParticlesPt);
      Tree->SetBranchAddress("ParticlesEta", &ParticlesEta);
      Tree->SetBranchAddress("ParticlesPhi", &ParticlesPhi);
      Tree->SetBranchAddress("ParticlesM", &ParticlesM);
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

      int EntryCount = Tree->GetEntries();

      for(int iE = 0; iE < EntryCount; iE++)
      {
         Tree->GetEntry(iE);

         if(SignalJetPt == nullptr)
            continue;

         int NJet = SignalJetPt->size();
         for(int iJ = 0; iJ < NJet; iJ++)
         {
            HJetPT.Fill((*SignalJetPt)[iJ]);
            HJetSDPT.Fill((*SignalJetSDPt)[iJ]);
            if((*SignalJetEta)[iJ] < -2 || (*SignalJetEta)[iJ] > 2)
               continue;
            if((*SignalJetPt)[iJ] < MinPT)
               continue;
            TLorentzVector j;
	    j.SetPtEtaPhiM( (*SignalJetPt)[iJ], (*SignalJetEta)[iJ], (*SignalJetPhi)[iJ], 0);
            for(int iS = 0; iS < (*ParticlesPt).size(); iS++)
            {
	       if ((*isHadron)[iS]==0) continue;
	       TLorentzVector p;
	       p.SetPtEtaPhiM( (*ParticlesPt)[iS], (*ParticlesEta)[iS], (*ParticlesPhi)[iS], (*ParticlesM)[iS]);
               double ZG = p.P()/j.P();
	       double dTheta = fabs(j.DeltaPhi(p));
               HEMMI.Fill(dTheta, log(ZG));
            }

            for(int iS = 0; iS < (*ConstituentPt)[iJ].size(); iS++)
            {
	       if ((*ConstituentIsHadron)[iJ][iS]==0) continue;
	       TLorentzVector p;
	       p.SetPtEtaPhiM( (*ConstituentPt)[iJ][iS], (*ConstituentEta)[iJ][iS], (*ConstituentPhi)[iJ][iS], (*ConstituentM)[iJ][iS]);
	       double ZG = p.P()/j.P();
	       double dTheta = fabs(j.DeltaPhi(p));
               HEMMIConstituent.Fill(dTheta, log(ZG));
            }

            for(int iS = 0; iS < (*SDConstituentPt)[iJ].size(); iS++)
            {
	       if ((*SDConstituentIsHadron)[iJ][iS]==0) continue;
	       TLorentzVector p;
	       p.SetPtEtaPhiM( (*SDConstituentPt)[iJ][iS], (*SDConstituentEta)[iJ][iS], (*SDConstituentPhi)[iJ][iS], (*SDConstituentM)[iJ][iS]);
	       double ZG = p.P()/j.P();
	       double dTheta = fabs(j.DeltaPhi(p));
               HEMMISDConstituent.Fill(dTheta, log(ZG));
            }
        }
	int NAllJet = AllJetPt->size();
        for(int iJ = 0; iJ < NAllJet; iJ++)
	{
            HAllJetPT.Fill((*AllJetPt)[iJ]);
            HAllJetSDPT.Fill((*AllJetSDPt)[iJ]);
            if((*AllJetEta)[iJ] < -2 || (*AllJetEta)[iJ] > 2)
               continue;
            if((*AllJetPt)[iJ] < MinPT)
               continue;
            TLorentzVector j;
	    j.SetPtEtaPhiM( (*AllJetPt)[iJ], (*AllJetEta)[iJ], (*AllJetPhi)[iJ], 0);
         
            for(int iS = 0; iS < (*AllConstituentPt)[iJ].size(); iS++)
            {
	       if ((*AllConstituentIsHadron)[iJ][iS]==0) continue;
	       TLorentzVector p;
	       p.SetPtEtaPhiM( (*AllConstituentPt)[iJ][iS], (*AllConstituentEta)[iJ][iS], (*AllConstituentPhi)[iJ][iS], (*AllConstituentM)[iJ][iS]);
	       double ZG = p.P()/j.P();
	       double dTheta = fabs(j.DeltaPhi(p));
               HEMMIAllConstituent.Fill(dTheta, log(ZG));
            }

            for(int iS = 0; iS < (*SDAllConstituentPt)[iJ].size(); iS++)
            {
	       if ((*SDAllConstituentIsHadron)[iJ][iS]==0) continue;
	       TLorentzVector p;
	       p.SetPtEtaPhiM( (*SDAllConstituentPt)[iJ][iS], (*SDAllConstituentEta)[iJ][iS], (*SDAllConstituentPhi)[iJ][iS], (*SDAllConstituentM)[iJ][iS]);
	       double ZG = p.P()/j.P();
	       double dTheta = fabs(j.DeltaPhi(p));
               HEMMISDAllConstituent.Fill(dTheta, log(ZG));
            }


         }
      }

      File.Close();
   }

   PdfFile.AddPlot(HJetPT, "", true);
   PdfFile.AddPlot(HAllJetPT, "", true);
   PdfFile.AddPlot(HJetSDPT, "", true);
   PdfFile.AddPlot(HAllJetSDPT, "", true);
   PdfFile.AddPlot(HEMMI, "colz");
   PdfFile.AddPlot(HEMMIConstituent, "colz");
   PdfFile.AddPlot(HEMMISDConstituent, "colz");
   PdfFile.AddPlot(HEMMIAllConstituent, "colz");
   PdfFile.AddPlot(HEMMISDAllConstituent, "colz");

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




