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
   double MinPT = CL.GetDouble("minpt", 200);

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("EMMI Plane");

   TH1D HJetPT("HJetPT", "Jet PT", 100, 0, 500);
   TH2D HEMMICA("HEMMICA", ";#Delta#Theta;ln(z)", 100, 0, 0.5, 100, -10, 0);
   
   HEMMICA.SetStats(0);

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
      vector<double> *SignalJetEta = nullptr;
      vector<double> *SignalJetPhi = nullptr;
      vector<double> *ParticlesPt = nullptr;
      vector<double> *ParticlesEta = nullptr;
      vector<double> *ParticlesPhi = nullptr;
      vector<double> *ParticlesM = nullptr;
      vector<bool> *isHadron = nullptr;

      Tree->SetBranchAddress("SignalJet04Pt", &SignalJetPt);
      Tree->SetBranchAddress("SignalJet04Eta", &SignalJetEta);
      Tree->SetBranchAddress("SignalJet04Phi", &SignalJetPhi);
      Tree->SetBranchAddress("ParticlesPt", &ParticlesPt);
      Tree->SetBranchAddress("ParticlesEta", &ParticlesEta);
      Tree->SetBranchAddress("ParticlesPhi", &ParticlesPhi);
      Tree->SetBranchAddress("ParticlesM", &ParticlesM);
      Tree->SetBranchAddress("isHadron", &isHadron);

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
            if((*SignalJetEta)[iJ] < -2 || (*SignalJetEta)[iJ] > 2)
               continue;
            if((*SignalJetPt)[iJ] < MinPT)
               continue;
            cout <<ParticlesPt<<endl;
            for(int iS = 0; iS < (*ParticlesPt).size(); iS++)
            {
	       if ((*isHadron)[iS]==0) continue;
	       TLorentzVector p;
	       p.SetPtEtaPhiM( (*ParticlesPt)[iS], (*ParticlesEta)[iS], (*ParticlesPhi)[iS], (*ParticlesM)[iS]);
	       TLorentzVector j;
	       j.SetPtEtaPhiM( (*SignalJetPt)[iJ], (*SignalJetEta)[iJ], (*SignalJetPhi)[iJ], 0);
               double ZG = p.P()/j.P();
	       double dTheta = fabs(j.DeltaPhi(p));
	       //cout <<ZG<<" "<<dTheta<<endl;
               //double Theta = (*SignalJetCADRs)[iJ][iS] / 0.4;
               HEMMICA.Fill(dTheta, log(ZG));
            }

         }
      }

      File.Close();
   }

   PdfFile.AddPlot(HJetPT, "", true);
   PdfFile.AddPlot(HEMMICA, "colz");

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




