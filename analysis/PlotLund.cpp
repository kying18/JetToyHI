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
   double MaxEta = CL.GetDouble("maxeta", 2);
   double MinZ = CL.GetDouble("minz", 1e-4);
   double MaxZ = CL.GetDouble("maxz", 0.02);
   string Title = CL.Get("title", "");

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("Lund Plane");

   TH1D HJetRawPT("HJetRawPT", "Jet PT (no subtraction)", 100, 0, 500);
   TH1D HJetPT("HJetPT", "Jet PT", 100, 0, 500);
   TH2D HLundCA("HLundCA", ";-ln(#Delta R);ln(k_{T})", 100, 0, 8, 100, -3, 5);
   TH2D HLundAK("HLundAK", ";-ln(#Delta R);ln(k_{T})", 100, 0, 8, 100, -3, 5);
   TH2D HLundKT("HLundKT", ";-ln(#Delta R);ln(k_{T})", 100, 0, 8, 100, -3, 5);

   HLundCA.SetTitle(Title.c_str());
   HLundAK.SetTitle(Title.c_str());
   HLundKT.SetTitle(Title.c_str());
      
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
      vector<vector<double>> *SignalJetCAPT2s = nullptr;
      vector<vector<double>> *SignalJetCAEta2s = nullptr;
      vector<vector<double>> *SignalJetCAPhi2s = nullptr;
      vector<vector<double>> *SignalJetAKZGs = nullptr;
      vector<vector<double>> *SignalJetAKDRs = nullptr;
      vector<vector<double>> *SignalJetAKPT2s = nullptr;
      vector<vector<double>> *SignalJetAKEta2s = nullptr;
      vector<vector<double>> *SignalJetAKPhi2s = nullptr;
      vector<vector<double>> *SignalJetKTZGs = nullptr;
      vector<vector<double>> *SignalJetKTDRs = nullptr;
      vector<vector<double>> *SignalJetKTPT2s = nullptr;
      vector<vector<double>> *SignalJetKTEta2s = nullptr;
      vector<vector<double>> *SignalJetKTPhi2s = nullptr;
      vector<double> *Weight = nullptr;

      Tree->SetBranchAddress("SignalJetPt", &SignalJetRawPt);
      Tree->SetBranchAddress("SignalJetJewelPt", &SignalJetPt);
      Tree->SetBranchAddress("SignalJetEta", &SignalJetEta);
      Tree->SetBranchAddress("SignalJetPhi", &SignalJetPhi);
      Tree->SetBranchAddress("SignalJetCAZGs", &SignalJetCAZGs);
      Tree->SetBranchAddress("SignalJetCADRs", &SignalJetCADRs);
      Tree->SetBranchAddress("SignalJetCAPT2s", &SignalJetCAPT2s);
      Tree->SetBranchAddress("SignalJetCAEta2s", &SignalJetCAEta2s);
      Tree->SetBranchAddress("SignalJetCAPhi2s", &SignalJetCAPhi2s);
      Tree->SetBranchAddress("SignalJetAKZGs", &SignalJetAKZGs);
      Tree->SetBranchAddress("SignalJetAKDRs", &SignalJetAKDRs);
      Tree->SetBranchAddress("SignalJetAKPT2s", &SignalJetAKPT2s);
      Tree->SetBranchAddress("SignalJetAKEta2s", &SignalJetAKEta2s);
      Tree->SetBranchAddress("SignalJetAKPhi2s", &SignalJetAKPhi2s);
      Tree->SetBranchAddress("SignalJetKTZGs", &SignalJetKTZGs);
      Tree->SetBranchAddress("SignalJetKTDRs", &SignalJetKTDRs);
      Tree->SetBranchAddress("SignalJetKTPT2s", &SignalJetKTPT2s);
      Tree->SetBranchAddress("SignalJetKTEta2s", &SignalJetKTEta2s);
      Tree->SetBranchAddress("SignalJetKTPhi2s", &SignalJetKTPhi2s);
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

            if((*SignalJetEta)[iJ] < -MaxEta || (*SignalJetEta)[iJ] > MaxEta)
               continue;
            if((*SignalJetPt)[iJ] < MinPT)
               continue;

            double JetPT = (*SignalJetPt)[iJ];
            JetCount = JetCount + (*Weight)[0];

            for(int iS = 0; iS < (*SignalJetCAZGs)[iJ].size(); iS++)
            {
               double ZG = (*SignalJetCAZGs)[iJ][iS];
               double DR = (*SignalJetCADRs)[iJ][iS];
               double PT2 = (*SignalJetCAPT2s)[iJ][iS];
               HLundCA.Fill(-log(DR), log(PT2 * DR), (*Weight)[0]);
            }
            for(int iS = 0; iS < (*SignalJetAKZGs)[iJ].size(); iS++)
            {
               double ZG = (*SignalJetAKZGs)[iJ][iS];
               double DR = (*SignalJetAKDRs)[iJ][iS];
               double PT2 = (*SignalJetAKPT2s)[iJ][iS];
               HLundAK.Fill(-log(DR), log(PT2 * DR), (*Weight)[0]);
            }
            for(int iS = 0; iS < (*SignalJetKTZGs)[iJ].size(); iS++)
            {
               double ZG = (*SignalJetKTZGs)[iJ][iS];
               double DR = (*SignalJetKTDRs)[iJ][iS];
               double PT2 = (*SignalJetKTPT2s)[iJ][iS];
               HLundKT.Fill(-log(DR), log(PT2 * DR), (*Weight)[0]);
            }
         }
      }

      File.Close();
   }

   HLundCA.Scale(1 / JetCount);
   HLundAK.Scale(1 / JetCount);
   HLundKT.Scale(1 / JetCount);

   HLundCA.SetMinimum(MinZ);
   HLundAK.SetMinimum(MinZ);
   HLundKT.SetMinimum(MinZ);
   HLundCA.SetMaximum(MaxZ);
   HLundAK.SetMaximum(MaxZ);
   HLundKT.SetMaximum(MaxZ);

   // PdfFile.AddPlot(HJetRawPT, "", true);
   // PdfFile.AddPlot(HJetPT, "", true);
   PdfFile.AddPlot(HLundCA, "colz");
   PdfFile.AddPlot(HLundAK, "colz");
   PdfFile.AddPlot(HLundKT, "colz");
   PdfFile.AddPlot(HLundCA, "colz", false, true);
   PdfFile.AddPlot(HLundAK, "colz", false, true);
   PdfFile.AddPlot(HLundKT, "colz", false, true);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




