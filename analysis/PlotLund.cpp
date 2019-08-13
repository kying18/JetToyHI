#include <iostream>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("input");
   string OutputBase = CL.Get("output");
   double MinPT = CL.GetDouble("minpt", 200);

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("Lund Plane");

   TH1D HJetRawPT("HJetRawPT", "Jet PT (no subtraction)", 100, 0, 500);
   TH1D HJetPT("HJetPT", "Jet PT", 100, 0, 500);
   TH2D HLundCA("HLundCA", ";-ln(#theta);ln(z#theta)", 100, 0, 5, 100, -10, 0);
   TH2D HLundAK("HLundAK", ";-ln(#theta);ln(z#theta)", 100, 0, 5, 100, -10, 0);
   TH2D HLundKT("HLundKT", ";-ln(#theta);ln(z#theta)", 100, 0, 5, 100, -10, 0);
      
   double JetCount = 0;

   for(string FileName : InputFileNames)
   {
      TFile File(FileName.c_str());

      TTree *Tree = (TTree *)File.Get("JetTree");

      if(Tree == nullptr)
      {
         File.Close();
         continue;
      }

      vector<double> *SignalJetRawPt = nullptr;
      vector<double> *SignalJetPt = nullptr;
      vector<double> *SignalJetEta = nullptr;
      vector<double> *SignalJetPhi = nullptr;
      vector<vector<double>> *SignalJetCAZGs = nullptr;
      vector<vector<double>> *SignalJetCADRs = nullptr;
      vector<vector<double>> *SignalJetAKZGs = nullptr;
      vector<vector<double>> *SignalJetAKDRs = nullptr;
      vector<vector<double>> *SignalJetKTZGs = nullptr;
      vector<vector<double>> *SignalJetKTDRs = nullptr;
      vector<double> *Weight = nullptr;

      Tree->SetBranchAddress("SignalJetPt", &SignalJetRawPt);
      Tree->SetBranchAddress("SignalJetJewelPt", &SignalJetPt);
      Tree->SetBranchAddress("SignalJetEta", &SignalJetEta);
      Tree->SetBranchAddress("SignalJetPhi", &SignalJetPhi);
      Tree->SetBranchAddress("SignalJetCAZGs", &SignalJetCAZGs);
      Tree->SetBranchAddress("SignalJetCADRs", &SignalJetCADRs);
      Tree->SetBranchAddress("SignalJetAKZGs", &SignalJetAKZGs);
      Tree->SetBranchAddress("SignalJetAKDRs", &SignalJetAKDRs);
      Tree->SetBranchAddress("SignalJetKTZGs", &SignalJetKTZGs);
      Tree->SetBranchAddress("SignalJetKTDRs", &SignalJetKTDRs);
      Tree->SetBranchAddress("EventWeight", &Weight);

      int EntryCount = Tree->GetEntries();

      for(int iE = 0; iE < EntryCount; iE++)
      {
         Tree->GetEntry(iE);

         if(SignalJetPt == nullptr)
            continue;

         int NJet = SignalJetPt->size();
         for(int iJ = 0; iJ < NJet; iJ++)
         {
            HJetRawPT.Fill((*SignalJetRawPt)[iJ], (*Weight)[0]);
            HJetPT.Fill((*SignalJetPt)[iJ], (*Weight)[0]);

            if((*SignalJetEta)[iJ] < -2 || (*SignalJetEta)[iJ] > 2)
               continue;
            if((*SignalJetPt)[iJ] < MinPT)
               continue;

            JetCount = JetCount + (*Weight)[0];

            for(int iS = 0; iS < (*SignalJetCAZGs)[iJ].size(); iS++)
            {
               double ZG = (*SignalJetCAZGs)[iJ][iS];
               double Theta = (*SignalJetCADRs)[iJ][iS] / 0.4;
               HLundCA.Fill(-log(Theta), log(ZG * Theta), (*Weight)[0]);
            }
            for(int iS = 0; iS < (*SignalJetAKZGs)[iJ].size(); iS++)
            {
               double ZG = (*SignalJetAKZGs)[iJ][iS];
               double Theta = (*SignalJetAKDRs)[iJ][iS] / 0.4;
               HLundAK.Fill(-log(Theta), log(ZG * Theta), (*Weight)[0]);
            }
            for(int iS = 0; iS < (*SignalJetKTZGs)[iJ].size(); iS++)
            {
               double ZG = (*SignalJetKTZGs)[iJ][iS];
               double Theta = (*SignalJetKTDRs)[iJ][iS] / 0.4;
               HLundKT.Fill(-log(Theta), log(ZG * Theta), (*Weight)[0]);
            }
         }
      }

      File.Close();
   }

   HLundCA.Scale(1 / JetCount);
   HLundAK.Scale(1 / JetCount);
   HLundKT.Scale(1 / JetCount);

   // PdfFile.AddPlot(HJetRawPT, "", true);
   // PdfFile.AddPlot(HJetPT, "", true);
   PdfFile.AddPlot(HLundCA, "colz");
   PdfFile.AddPlot(HLundAK, "colz");
   PdfFile.AddPlot(HLundKT, "colz");

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




