#include <iostream>
#include <vector>
using namespace std;

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TGraph.h"
#include "TFile.h"
#include "TChain.h"

#include "CommandLine.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   int JetPTCut = CL.GetInt("JetPTCut", 350);
   string Algo = CL.Get("Algo", "KT");
   string Title = CL.Get("Title", "");
   string OutputBase = CL.Get("OutputBase", "Correlation");

   vector<string> FileNames = CL.GetStringVector("InputFiles");

   TChain Tree("JetTree", "JetTree");

   for(string File : FileNames)
      Tree.AddFile(File.c_str());

   int DoZG = CL.GetInt("DoZG", 0);

   string TitleString = Form("%s  p_{T}^{jet} > %d GeV;Max ln(k_{T}), %s declustering;Max ln(k_{T}) partons", Title.c_str(), JetPTCut, Algo.c_str());
   if(DoZG > 0)
      TitleString = Form("%s  p_{T}^{jet} > %d GeV;Groomed ln(k_{T});Max ln(k_{T}) partons", Title.c_str(), JetPTCut);

   TH2D H("H", TitleString.c_str(),
      100, 0, 5, 100, -2, 6);

   Tree.SetAlias("JPDR", "(sqrt((SignalJetEta[0]-PartonEta)*(SignalJetEta[0]-PartonEta)+(acos(cos(SignalJetPhi[0]-PartonPhi)))*(acos(cos(SignalJetPhi[0]-PartonPhi)))))");

   if(DoZG == 0)
   {
      Tree.Draw(Form("log(Max$(PartonSJ2sPt*PartonDRs*(JPDR<0.4)*(PartonDRs<0.4))):log(Max$(SignalJet%sPT2s*SignalJet%sDRs))>>H", Algo.c_str(), Algo.c_str()),
            Form("SignalJetPt[0]>%d&&log(Max$(PartonSJ2sPt*PartonDRs*(JPDR<0.4)*(PartonDRs<0.4)))!=0", JetPTCut),
            "colz");
   }
   else
   {
      Tree.Draw(Form("log(Max$(PartonSJ2sPt*PartonDRs*(JPDR<0.4)*(PartonDRs<0.4))):log(Max$(SignalJetSD%dSubjet2Pt*SignalJetSD%dDR12))>>H", DoZG, DoZG),
            Form("SignalJetPt[0]>%d&&log(Max$(PartonSJ2sPt*PartonDRs*(JPDR<0.4)*(PartonDRs<0.4)))!=0", JetPTCut),
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

   return 0;
}
