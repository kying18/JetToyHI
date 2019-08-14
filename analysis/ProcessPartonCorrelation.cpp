#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TGraph.h"
#include "TFile.h"

#include "CommandLine.h"
#include "SetStyle.h"

int main(int argc, char *argv[]);
double GetDR(double eta1, double phi1, double eta2, double phi2);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   int JetPTCut = CL.GetInt("JetPTCut", 350);
   string Algo = CL.Get("Algo", "KT");
   string Title = CL.Get("Title", "");
   string OutputBase = CL.Get("OutputBase", "Correlation");

   TFile File(CL.Get("InputFile", "../samples/Pythia.root").c_str());
   
   int DoZG = CL.GetInt("DoZG", 0);

   TTree *Tree = (TTree *)File.Get("JetTree");

   vector<double> *SignalJetPt = nullptr;
   vector<double> *SignalJetEta = nullptr;
   vector<double> *SignalJetPhi = nullptr;
   vector<vector<double>> *SignalJetSJ1sPt = nullptr;
   vector<vector<double>> *SignalJetSJ1sEta = nullptr;
   vector<vector<double>> *SignalJetSJ1sPhi = nullptr;
   vector<vector<double>> *SignalJetSJ2sPt = nullptr;
   vector<vector<double>> *SignalJetSJ2sEta = nullptr;
   vector<vector<double>> *SignalJetSJ2sPhi = nullptr;
   vector<double> *PartonPt = nullptr;
   vector<double> *PartonEta = nullptr;
   vector<double> *PartonPhi = nullptr;
   vector<double> *PartonSJ1sPt = nullptr;
   vector<double> *PartonSJ1sEta = nullptr;
   vector<double> *PartonSJ1sPhi = nullptr;
   vector<double> *PartonSJ2sPt = nullptr;
   vector<double> *PartonSJ2sEta = nullptr;
   vector<double> *PartonSJ2sPhi = nullptr;

   Tree->SetBranchAddress("SignalJetPt",      &SignalJetPt);
   Tree->SetBranchAddress("SignalJetEta",     &SignalJetEta);
   Tree->SetBranchAddress("SignalJetPhi",     &SignalJetPhi);
   Tree->SetBranchAddress("SignalJetSJ1sPt",  &SignalJetSJ1sPt);
   Tree->SetBranchAddress("SignalJetSJ1sEta", &SignalJetSJ1sEta);
   Tree->SetBranchAddress("SignalJetSJ1sPhi", &SignalJetSJ1sPhi);
   Tree->SetBranchAddress("SignalJetSJ2sPt",  &SignalJetSJ2sPt);
   Tree->SetBranchAddress("SignalJetSJ2sEta", &SignalJetSJ2sEta);
   Tree->SetBranchAddress("SignalJetSJ2sPhi", &SignalJetSJ2sPhi);
   Tree->SetBranchAddress("PartonPt",         &PartonPt);
   Tree->SetBranchAddress("PartonEta",        &PartonEta);
   Tree->SetBranchAddress("PartonPhi",        &PartonPhi);
   Tree->SetBranchAddress("PartonSJ1sPt",     &PartonSJ1sPt);
   Tree->SetBranchAddress("PartonSJ1sEta",    &PartonSJ1sEta);
   Tree->SetBranchAddress("PartonSJ1sPhi",    &PartonSJ1sPhi);
   Tree->SetBranchAddress("PartonSJ2sPt",     &PartonSJ2sPt);
   Tree->SetBranchAddress("PartonSJ2sEta",    &PartonSJ2sEta);
   Tree->SetBranchAddress("PartonSJ2sPhi",    &PartonSJ2sPhi);

   int EntryCount = Tree->GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree->GetEntry(iE);

      if(PartonPt->size() == 0)   // WTF
         continue;
      if(SignalJetPt->size() != 1)   // WTF
         continue;

      double PPT = (*PartonPt)[0];
      double PEta = (*PartonEta)[0];
      double PPhi = (*PartonPhi)[0];

      int BestJetIndex = 0;
      double BestDR = -1;
      for(int i = 0; i < (int)SignalJetPt->size(); i++)
      {
         double DR = GetDR(PEta, PPhi, (*SignalJetEta)[i], (*SignalJetPhi)[i]);
         if(BestDR < 0 || BestDR > DR)
         {
            BestJetIndex = i;
            BestDR = DR;
         }
      }

      double JPT = (*SignalJetPt)[BestJetIndex];
      double JEta = (*SignalJetEta)[BestJetIndex];
      double JPhi = (*SignalJetPhi)[BestJetIndex];
   }

   /*
   string TitleString = Form("%s  p_{T}^{jet} > %d GeV;Max ln(k_{T}), %s declustering;Max ln(k_{T}) partons", Title.c_str(), JetPTCut, Algo.c_str());
   if(DoZG > 0)
      TitleString = Form("%s  p_{T}^{jet} > %d GeV;Groomed ln(k_{T});Max ln(k_{T}) partons", Title.c_str(), JetPTCut);

   TH2D H("H", TitleString.c_str(),
      100, 0, 5, 100, -2, 6);

   if(DoZG == 0)
   {
      Tree->Draw(Form("log(Max$(PartonSJ2sPt*PartonDRs*(sqrt((SignalJetEta[0]-PartonEta)*(SignalJetEta[0]-PartonEta)+(acos(cos(SignalJetPhi[0]-PartonPhi)))*(acos(cos(SignalJetPhi[0]-PartonPhi))))<0.4))):log(Max$(SignalJet%sPT2s*SignalJet%sDRs))>>H", Algo.c_str(), Algo.c_str()),
            Form("SignalJetPt[0]>%d&&log(Max$(PartonSJ2sPt*PartonDRs*(sqrt((SignalJetEta[0]-PartonEta)*(SignalJetEta[0]-PartonEta)+(acos(cos(SignalJetPhi[0]-PartonPhi)))*(acos(cos(SignalJetPhi[0]-PartonPhi))))<0.4)))!=0", JetPTCut),
            "colz");
   }
   else
   {
      Tree->Draw(Form("log(Max$(PartonSJ2sPt*PartonDRs*(sqrt((SignalJetEta[0]-PartonEta)*(SignalJetEta[0]-PartonEta)+(acos(cos(SignalJetPhi[0]-PartonPhi)))*(acos(cos(SignalJetPhi[0]-PartonPhi))))<0.4))):log(Max$(SignalJetSD%dSubjet2Pt*SignalJetSD%dDR12))>>H", DoZG, DoZG),
            Form("SignalJetPt[0]>%d&&log(Max$(PartonSJ2sPt*PartonDRs*(sqrt((SignalJetEta[0]-PartonEta)*(SignalJetEta[0]-PartonEta)+(acos(cos(SignalJetPhi[0]-PartonPhi)))*(acos(cos(SignalJetPhi[0]-PartonPhi))))<0.4)))!=0", JetPTCut),
            "colz");
   }

   TCanvas Canvas;

   H.SetStats(0);
   H.Draw("colz");

   TGraph G;
   G.SetPoint(0, -10, -10);
   G.SetPoint(1, 10, 10);
   G.SetLineColor(kRed);
   G.SetLineWidth(3);
   // G.Draw("l");

   Canvas.SaveAs(Form("%s_%d_%s.png", OutputBase.c_str(), JetPTCut, Algo.c_str()));
   Canvas.SaveAs(Form("%s_%d_%s.pdf", OutputBase.c_str(), JetPTCut, Algo.c_str()));
   */

   return 0;
}

double GetDR(double eta1, double phi1, double eta2, double phi2)
{
   double deta = eta1 - eta2;
   double dphi = phi1 - phi2;

   if(dphi < -M_PI)   dphi = dphi + 2 * M_PI;
   if(dphi > +M_PI)   dphi = dphi - 2 * M_PI;

   return sqrt(deta * deta + dphi * dphi);
}


