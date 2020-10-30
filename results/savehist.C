#include "uti.h"
#include <iostream>
#include <string.h>
// #include "pythiaAnalysis.h"

using namespace std;

Double_t BIN_MIN = 250.;
Double_t BIN_MAX = 0.;
Double_t BIN_NUM = 100.;

void savehist(TString inputdata, TString outputfolder, TString branchLabel)
{
    TString PYTHIA_INPUT_DATA="/data/kying/lundPlaneResults/pythia_10000.root"
    TString PYTHIA_THERMAL_INPUT_DATA="/data/kying/lundPlaneResults/pythia_thermal_10000.root"
    TString PYTHIA_INPUT_CS_DATA="/data/kying/lundPlaneResults/pythia_cs_10000.root"
    TString PYTHIA_THERMAL_CS_INPUT_DATA="/data/kying/lundPlaneResults/pythia_thermal_cs_10000.root"

    gStyle->SetTextSize(0.045);
    gStyle->SetTextFont(42);
    gStyle->SetPadRightMargin(0.043);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.145);
    gStyle->SetTitleX(0.5);

    TString xLabel = "pT";
    // TObjArray *tx = outputfolder.Tokenize("/");
    // auto dataSource = ((TObjString *)(tx->at(1)))->String();
    // cout << "DATA SOURCE " << tx << endl;

    // Open the input data file
    TFile* inf = new TFile(inputdata.Data());
    TTree* jetTree = (TTree*)inf->Get("JetTree");

    // Create a histogram and fill it with 'Dmass'
    TH1D* h = new TH1D("h", branchLabel,BIN_NUM,BIN_MIN,BIN_MAX);
    jetTree->Project("h", branchLabel);
    cout << "GOT TREE" << endl;

    TFile * outputfile = new TFile(outputfolder+"/"+branchLabel+".root","recreate");

    h->Sumw2();
    h->GetXaxis()->SetTitle( xLabel );
    h->GetYaxis()->SetTitle( "log(1/N dN/d"+ xLabel + ")" );

    // no scaling 3500
    // dN/dpT 1400
    // 1/N dN/dpT 0.045
    // this gives the 1/N dN/dpT scaling
    Double_t factor = 1.;
    h->Scale(factor/h->Integral("width"));

    // when factor is 1, this is dN/dpT
    // can divide factor by total to get 1/N dN/dpT
    // confirmed that this is the same as the integral thing above
    // Double_t factor = 1./total;
    // h->Scale(factor, "width");  // 1400
    cout << "DID SCALING STUFF" << endl;
    TCanvas* c = new TCanvas("c","",600,600);
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    // h->SetAxisRange(-1,h->GetMaximum()*1.5,"Y");
    h->GetXaxis()->SetTitleOffset(1.0);
    h->GetYaxis()->SetTitleOffset(1.5);
    h->GetXaxis()->SetLabelOffset(0.007);
    h->GetYaxis()->SetLabelOffset(0.007);
    h->SetMarkerSize(0.8);
    h->SetMarkerStyle(20);
    h->SetStats(0);
    h->Draw();
    cout << "DREW HIST" << endl;
    gPad->SetLogy();
    c->SaveAs("temp/" + branchLabel + ".jpg");
    cout << "SAVED PLOT" << endl;
    outputfile->Write();
    outputfile->Close();
    cout << "WROTE TO OUTPUT FILE :')" << endl;
}

