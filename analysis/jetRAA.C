#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TLine.h>


void jetRAA(string algo="KT",string title="HYBRID")
{
   TFile *inf = new TFile("Lund_Hybrid_kappa0p404-0.root");
//   TFile *inf = new TFile("Lund_qpythia_dijet50_qhat2-0.root");
//   TFile *inf = new TFile("Lund_jewel-emmi_5TeV_hydro_jj_0-0_worec-0.root");
   TTree *tPbPb = (TTree*)inf->Get("JetTree");
   TFile *inf2 = new TFile("Lund_Hybrid_noquench-0.root");
//   TFile *inf2 = new TFile("Lund_qpythia_vac-0.root");
//   TFile *inf2 = new TFile("Lund_jewel-emmi_5TeV_pp_jj_xxxx-0.root");
   TTree *tPP = (TTree*)inf2->Get("JetTree");

   float bins[9]= {350,370,400,450,500,550,600,700,800};
//   float bins[9]= {70,80,90,100,150,200,300,400,500};
   TH1D *hPbPb = new TH1D("hPbPb","",8,bins);
   TH1D *hPP = new TH1D("hPP","",8, bins);
   TH1D *hPbPb2 = new TH1D("hPbPb2","",8,bins);
   TH1D *hPP2 = new TH1D("hPP2","",8, bins);
   TH1D *hPbPb3 = new TH1D("hPbPb3","",8,bins);
   TH1D *hPP3 = new TH1D("hPP3","",8, bins);
   
   TCanvas *c = new TCanvas("c","",600,600);  
   tPbPb->Project("hPbPb","SignalJetPt");
   tPP->Project("hPP","SignalJetPt");
   tPbPb->Project("hPbPb2","SignalJetPt",Form("log(Max$(SignalJet%sDRs*SignalJet%sPT2s))>3",algo.c_str(),algo.c_str()));
   tPP->Project("hPP2","SignalJetPt",Form("log(Max$(SignalJet%sDRs*SignalJet%sPT2s))>3",algo.c_str(),algo.c_str()));
   tPbPb->Project("hPbPb3","SignalJetPt",Form("log(Max$(SignalJet%sDRs*SignalJet%sPT2s))<3",algo.c_str(),algo.c_str()));
   tPP->Project("hPP3","SignalJetPt",Form("log(Max$(SignalJet%sDRs*SignalJet%sPT2s))<3",algo.c_str(),algo.c_str()));

   hPbPb->Sumw2();
   hPP->Sumw2();
   hPbPb->Scale(1./tPbPb->GetEntries());
   hPP->Scale(1./tPP->GetEntries());

   hPbPb2->Sumw2();
   hPP2->Sumw2();
   hPbPb2->Scale(1./tPbPb->GetEntries());
   hPP2->Scale(1./tPP->GetEntries());

   hPbPb3->Sumw2();
   hPP3->Sumw2();
   hPbPb3->Scale(1./tPbPb->GetEntries());
   hPP3->Scale(1./tPP->GetEntries());

   hPbPb->Divide(hPP);
   hPbPb2->Divide(hPP2);
   hPbPb3->Divide(hPP3);


   hPbPb->SetAxisRange(0,1.5,"Y");
   hPbPb->SetXTitle("anti-k_{T} R=0.4 Jet p_{T} GeV");
   hPbPb->SetYTitle("R_{AA}");
   hPbPb->SetMarkerStyle(24);
   hPbPb->Draw();
   hPbPb2->SetLineColor(2);
   hPbPb2->SetMarkerColor(2);
   hPbPb2->Draw("same");
   hPbPb3->SetLineColor(4);
   hPbPb3->SetMarkerColor(4);
   hPbPb3->Draw("same");
   

   TLegend *leg= new TLegend(0.2,0.7,0.9,0.9);
   leg->SetBorderSize(0);
   leg->SetFillStyle(0);
   leg->AddEntry("hPbPb",Form("%s Model #sqrt{s_{NN}}=5.02 TeV",title.c_str()),"t");
   leg->AddEntry("hPbPb",Form("%s Declustering",algo.c_str()),"t");
   leg->AddEntry("hPbPb","Inclusive","pl");
   leg->AddEntry("hPbPb2","ln(Max KT)>3","pl");
   leg->AddEntry("hPbPb3","ln(Max KT)<3","pl");
   leg->Draw(); 
   TLine *l = new TLine(350,1,800,1);
   l->SetLineStyle(2);
   l->Draw();
   
   c->SaveAs(Form("JetRAA_%s_%s.png",title.c_str(),algo.c_str()));   
}
