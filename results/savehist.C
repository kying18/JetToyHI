#include "uti.h"
#include <iostream>
#include <string.h>
// #include "pythiaAnalysis.h"

using namespace std;

// Double_t BIN_MIN = 250.;
// Double_t BIN_MAX = 0.;
Double_t BIN_MAX = 250.;
Double_t BIN_MIN = 0.;
Double_t BIN_NUM = 100.;

void savehist(TString outputfolder, TString branchLabel)
{
    TString PYTHIA_INPUT_DATA="/data/kying/lundPlaneResults/pythia_10000.root";
    TString PYTHIA_THERMAL_INPUT_DATA="/data/kying/lundPlaneResults/pythia_thermal_10000.root";
    TString PYTHIA_INPUT_CS_DATA="/data/kying/lundPlaneResults/pythia_cs_10000.root";
    TString PYTHIA_THERMAL_CS_INPUT_DATA="/data/kying/lundPlaneResults/pythia_thermal_cs_10000.root";
    // TString inputs[4] = {PYTHIA_INPUT_DATA, PYTHIA_THERMAL_INPUT_DATA, PYTHIA_INPUT_CS_DATA, PYTHIA_THERMAL_CS_INPUT_DATA};
    TString inputs[4] = {PYTHIA_INPUT_CS_DATA, PYTHIA_THERMAL_CS_INPUT_DATA, PYTHIA_INPUT_DATA, PYTHIA_THERMAL_INPUT_DATA};
    // TString inputs[1] = {PYTHIA_INPUT_CS_DATA};

    gStyle->SetTextSize(0.045);
    gStyle->SetTextFont(42);
    gStyle->SetPadRightMargin(0.043);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadTopMargin(0.15);
    gStyle->SetPadBottomMargin(0.145);
    gStyle->SetTitleX(0.5);

    TCanvas* c = new TCanvas("c","",1400,1200);
    TLegend* legend = new TLegend(0.67,0.7,0.97,0.87);

    TFile * outputfile = new TFile(outputfolder+"/"+branchLabel+".root","recreate");

    int colorIdx = 1;
    cout << "STARTING LOOP" << endl;
    for (TString inputdata:inputs) {
        // TCanvas* c1 = new TCanvas("c1","",1400,1200);
        cout << "INPUT: " << inputdata << endl;
        // Open the input data file
        TFile* inf = new TFile(inputdata.Data());
        TTree* jetTree = (TTree*)inf->Get("JetTree");

        // Create a histogram and fill it with 'Dmass'
        TH1D* h = new TH1D("h", branchLabel + " Spectra",//+ " Spectra (PYTHIA dijet, pthat=50, s=5.02TeV, UE on, Tun4C) (Thermal background: EventsMult=100, PtAv=0.85)", 
                            BIN_NUM,BIN_MIN,BIN_MAX);
        // h->SetTitle(Form("#splitline{%s}{#scale[0.4]{%s}}",h->GetTitle(),"Input data: PYTHIA dijet, pthat=50, s=5.02TeV, UE on, Tun4C"));
        // h->SetTitle(Form("#splitline{%s}{#scale[0.4]{%s}}",h->GetTitle(),"Pileup: Thermal, EventsMult=100, PtAv=0.85"));
        // jetTree->Project("h", branchLabel, Form("%s>17.49",branchLabel.Data()));
        jetTree->Project("h", branchLabel);
        cout << "GOT TREE" << endl;

        h->Sumw2();

        // TString label;
        // label = branchLabel.Contains("DR12") ? "DR12" : branchLabel.Contains("ZG") ? "ZG" : "Pt";
        h->GetXaxis()->SetTitle( branchLabel );
        // h->GetYaxis()->SetTitle( "1/N dN/d"+ branchLabel );
        h->GetYaxis()->SetTitle( "N" );

        // no scaling 3500
        // dN/dpT 1400
        // 1/N dN/dpT 0.045
        // this gives the 1/N dN/dpT scaling
        Double_t factor = 1.;
        // h->Scale(factor/h->Integral("width"));

        // when factor is 1, this is dN/dpT
        // can divide factor by total to get 1/N dN/dpT
        // confirmed that this is the same as the integral thing above
        // Double_t total = h->GetEntries();
        // cout << total << endl;
        // Double_t factor = 1./total;
        h->Scale(factor, "width");  // 1400


        h->GetXaxis()->CenterTitle();
        h->GetYaxis()->CenterTitle();
        // h->SetAxisRange(-1,h->GetMaximum()*1.5,"Y");
        h->GetXaxis()->SetTitleOffset(1.0);
        h->GetYaxis()->SetTitleOffset(1.5);
        h->GetXaxis()->SetLabelOffset(0.007);
        h->GetYaxis()->SetLabelOffset(0.007);
        h->SetMarkerStyle(20);
        h->SetMarkerSize(0.9);
        h->SetMarkerColor(colorIdx);
        h->SetLineColor(colorIdx);
        h->SetStats(0);
        h->Draw("SAME");
        cout << "DREW HIST" << endl;
        colorIdx+=1;

        TString legendText;
        if (inputdata == PYTHIA_INPUT_DATA){
            legendText="Pythia";
        } else if (inputdata == PYTHIA_THERMAL_INPUT_DATA) {
            legendText="Pythia + Thermal";
        } else if (inputdata == PYTHIA_INPUT_CS_DATA) {
            legendText="Pythia + CS";
        } else if (inputdata == PYTHIA_THERMAL_CS_INPUT_DATA) {
            legendText="Pythia + Thermal + CS";
        }
        legend->AddEntry(h, legendText, "p");
        // c1->SaveAs("temp/" + branchLabel + legendText + ".jpg");
    }
    // cout << "DREW HIST" << endl;

    gPad->SetLogy();
    // TODO: remove the removal of log

    legend->SetTextSize(0.028);
    legend->SetFillColor(0);
    legend->SetBorderSize(1);
    legend->Draw("");

    c->Update();
    c->SaveAs("temp/" + branchLabel + ".jpg");
    cout << "SAVED PLOT" << endl;
    outputfile->Write();
    outputfile->Close();
    cout << "WROTE TO OUTPUT FILE :')" << endl;
}

