#define pythiaAnalysis_cxx
#include "pythiaAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include "uti.h"


using namespace std;

void pythiaAnalysis::Loop()
{
//   In a ROOT session, you can do:
//      root> .L pythiaAnalysis.C
//      root> pythiaAnalysis t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   TString name = "SignalJetPt";
   TString type = "temp";
   TString xLabel = "pT";
   
   Double_t BIN_MIN = 1.;
   Double_t BIN_MAX = 0.;
   Double_t BIN_NUM = 100.;


   if (fChain == 0) return;
   TFile * outputfile = new TFile("outputfiles/test.root","recreate");
   TH1D * h = new TH1D(name, name + " (" + type + ")", BIN_NUM, BIN_MIN, BIN_MAX);
   // TCanvas* c = new TCanvas("c","",600,600);
   h->Sumw2();
   h->GetXaxis()->SetTitle( xLabel );
   h->GetYaxis()->SetTitle( "1/N dN/d"+ xLabel );

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      // nb = fChain->GetEntry(jentry);   nbytes += nb;
      nb = b_SignalJetPt->GetEntry(ientry); nbytes += nb;
      // nb = b_SignalJetSD2Pt->GetEntry(ientry); nbytes += nb;
      for (auto pt = SignalJetPt->begin(); pt < SignalJetPt->end(); pt++) {
         // cout << *pt << endl;
         h->Fill(*pt);
         // total ++;
      }
   }

   // no scaling 3500
   // dN/dpT 1400
   // 1/N dN/dpT 0.045
   // this gives the 1/N dN/dpT scaling

   //TODO: undo this
   // Double_t factor = 1.;
   // h->Scale(factor/h->Integral("width"));

   // h->Draw("H"); gPad->SetLogy();
   // c->SaveAs("plots/"+type+"/"+xLabel+".jpg");

   // when factor is 1, this is dN/dpT
   // can divide factor by total to get 1/N dN/dpT
   // confirmed that this is the same as the integral thing above
   // Double_t factor = 1./total;
   // h->Scale(factor, "width");  // 1400

   outputfile->Write();
   outputfile->Close();
}

// to plot in log scale, use in root
// root [1] SignalJetPt->Draw("H"); gPad->SetLogy();


int main(int argc, char *argv[])
{
   pythiaAnalysis * ana = new pythiaAnalysis();
   ana->Loop();
   return 0;
}