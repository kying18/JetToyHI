#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TLine.h>


void jetRAA()
{
   TFile *inf = new TFile("Lund_Hybrid_kappa0p404-0.root");
   TTree *tPbPb = (TTree*)inf->Get("JetTree");
   TFile *inf2 = new TFile("Lund_Hybrid_noquench-0.root");
   TTree *tPP = (TTree*)inf2->Get("JetTree");


   float bins[6]= {350,370,400,500,600,800};
   TH1D *hPbPb = new TH1D("hPbPb","",5,bins);
   TH1D *hPP = new TH1D("hPP","",5, bins);
   TH1D *hPbPb2 = new TH1D("hPbPb2","",5,bins);
   TH1D *hPP2 = new TH1D("hPP2","",5, bins);
   TH1D *hPbPb3 = new TH1D("hPbPb3","",5,bins);
   TH1D *hPP3 = new TH1D("hPP3","",5, bins);
   
   TCanvas *c = new TCanvas("c","",600,600);  
   tPbPb->Project("hPbPb","SignalJetPt");
   tPP->Project("hPP","SignalJetPt");
   tPbPb->Project("hPbPb2","SignalJetPt","log(Max$(SignalJetCADRs*SignalJetCAPT2s))>3");
   tPP->Project("hPP2","SignalJetPt","log(Max$(SignalJetCADRs*SignalJetCAPT2s))>3");
   tPbPb->Project("hPbPb3","SignalJetPt","log(Max$(SignalJetCADRs*SignalJetCAPT2s))<3");
   tPP->Project("hPP3","SignalJetPt","log(Max$(SignalJetCADRs*SignalJetCAPT2s))<3");

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


   hPbPb->SetAxisRange(0,2,"Y");
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
   leg->AddEntry("hPbPb","HYBRID Model #sqrt{s_{NN}}=5.02 TeV","t");
   leg->AddEntry("hPbPb","CA Declustering","t");
   leg->AddEntry("hPbPb","Inclusive","pl");
   leg->AddEntry("hPbPb2","ln(Max K_T)>3","pl");
   leg->AddEntry("hPbPb3","ln(Max KT)<3","pl");
   leg->Draw(); 
   TLine *l = new TLine(350,1,800,1);
   l->SetLineStyle(2);
   l->Draw();
   
}
