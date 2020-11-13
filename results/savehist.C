#include "uti.h"
#include <iostream>
#include <string.h>
#include <map>
#include "../analysis/PlotHelper4.h"
// #include "pythiaAnalysis.h"

using namespace std;

Double_t BIN_MIN;
Double_t BIN_MAX;
Double_t BIN_NUM = 100.;

bool WRITE_TO_PDF = false;

map<string, string> SOFT_DROP_PT_MAP = {
    {"", "SignalJetPt"},
    {"soft drop 1", "SignalJetSD1Pt"},
    {"soft drop 2", "SignalJetSD2Pt"},
    {"soft drop 3", "SignalJetSD3Pt"},
    {"soft drop 4", "SignalJetSD4Pt"},
    {"soft drop 5", "SignalJetSD5Pt"},
    {"soft drop 6", "SignalJetSD6Pt"},
};

map<string, string> SOFT_DROP_R12_MAP = {
    {"soft drop 1", "SignalJetSD1DR12"},
    {"soft drop 2", "SignalJetSD2DR12"},
    {"soft drop 3", "SignalJetSD3DR12"},
    {"soft drop 4", "SignalJetSD4DR12"},
    {"soft drop 5", "SignalJetSD5DR12"},
    {"soft drop 6", "SignalJetSD6DR12"},
};

TString getSoftDrop(TString branchLabel) {
    TString sd = "";
    if(branchLabel.Contains("SD1")) {
        sd = "soft drop 1";
    } else if(branchLabel.Contains("SD2")) {
        sd = "soft drop 2";
    } else if(branchLabel.Contains("SD3")) {
        sd = "soft drop 3";
    } else if(branchLabel.Contains("SD4")) {
        sd = "soft drop 4";
    } else if(branchLabel.Contains("SD5")) {
        sd = "soft drop 5";
    } else if(branchLabel.Contains("SD6")) {
        sd = "soft drop 6";
    } 
    cout << "soft drop setting: " << sd << endl;
    return sd;
}

int getMinXBin(TH1D* hist){
    int minXbin = 1;
    for (int i=1; i <= hist->GetNbinsX(); i++) {
        if (hist->GetBinContent(i)) {
            minXbin = i;
            break;
        }
    }
    return minXbin;
}
int getMaxXBin(TH1D* hist){
    int maxXbin = hist->GetNbinsX();
    for (int i=hist->GetNbinsX(); i >= 1; i--) {
        if (hist->GetBinContent(i)) {
            maxXbin = i;
            break;
        }
    }
    return maxXbin;
}

void savehist()
{
    PdfFileHelper PdfFile("plots/7k_pt50cutoff/plotspt50_v2.pdf");
    if (WRITE_TO_PDF) {
        PdfFile.AddTextPage("Pythia (+ thermal) spectra (pT > 50 GeV)");
    }

    // TString branchLabels[7] = {"SignalJetPt", "SignalJetSD1DR12", "SignalJetSD1Pt", "SignalJetSD1ZG", "SignalJetSD2DR12", "SignalJetSD2Pt", "SignalJetSD2ZG"};
    // TString branchLabels[4] = {"SignalJetSD1DR12", "SignalJetSD1ZG", "SignalJetSD2DR12", "SignalJetSD2ZG"};
    TString branchLabels[1] = {"SignalJetSD5DR12"};
    for (auto branchLabel: branchLabels) {

        TString xLabel;
        if(branchLabel.Contains("Pt")) {
            xLabel = "p_{T}";
            BIN_MAX = 250.5;
            BIN_MIN = 50.0;
        } else if(branchLabel.Contains("DR12") ) {
            xLabel = "dR_{12}";
            BIN_MAX = 0.7;
            BIN_MIN = 0;
        } else if(branchLabel.Contains("ZG")) {
            xLabel = "z_{G}";
            BIN_MAX = 0.6;
            BIN_MIN = 0;
        }
        
        TString PYTHIA_INPUT_DATA="/data/kying/lundPlaneResults/pythia_10000.root";
        TString PYTHIA_THERMAL_INPUT_DATA="/data/kying/lundPlaneResults/pythia_thermal7k_10000.root";
        TString PYTHIA_INPUT_CS_DATA="/data/kying/lundPlaneResults/pythia_cs_10000.root";
        TString PYTHIA_THERMAL_CS_INPUT_DATA="/data/kying/lundPlaneResults/pythia_thermal7k_cs_10000.root";
        // TString inputs[4] = {PYTHIA_THERMAL_INPUT_DATA, PYTHIA_INPUT_DATA, PYTHIA_INPUT_CS_DATA, PYTHIA_THERMAL_CS_INPUT_DATA};
        TString inputs[4] = {PYTHIA_INPUT_DATA, PYTHIA_THERMAL_INPUT_DATA, PYTHIA_INPUT_CS_DATA, PYTHIA_THERMAL_CS_INPUT_DATA};
        // TString inputs[4] = {PYTHIA_INPUT_CS_DATA, PYTHIA_THERMAL_CS_INPUT_DATA, PYTHIA_INPUT_DATA, PYTHIA_THERMAL_INPUT_DATA};
        // TString inputs[1] = {PYTHIA_THERMAL_CS_INPUT_DATA};

        double fracFailed[4];

        gStyle->SetTextSize(0.045);
        gStyle->SetTextFont(42);
        gStyle->SetPadRightMargin(0.05);
        gStyle->SetPadLeftMargin(0.15);
        gStyle->SetPadTopMargin(0.15);
        gStyle->SetPadBottomMargin(0.145);
        gStyle->SetTitleX(0.5);

        TCanvas* c = new TCanvas("c","",1200,900);
        TLegend* legend = new TLegend(0.67,0.75,0.97,0.92);

        TFile * outputfile = new TFile("outputfiles/"+branchLabel+".root","recreate");

        int colorIdx = 1;
        cout << "STARTING LOOP" << endl;

        TString sd = getSoftDrop(branchLabel);

        THStack hs("hs",xLabel + " spectra" + (sd != "" ? " (" + sd + ")" : "") +  ";" + xLabel + ";1/N dN/d" + xLabel);

        for (int i=0; i < 4; i++) {
            TString inputdata = inputs[i];
            cout << "INPUT: " << inputdata << endl;
            // Open the input data file
            TFile* inf = new TFile(inputdata.Data());
            TTree* jetTree = (TTree*)inf->Get("JetTree");

            // getting fraction of jets that are < 0 (ie softdrop did not work)
            // we only want to do this if we need soft drop
            if (sd != "") {
                TH1D* H = new TH1D("H", "H", 1, -1, 0);
                jetTree->Project("H", SOFT_DROP_R12_MAP[sd.Data()].c_str(), Form("%s>50", SOFT_DROP_PT_MAP[sd.Data()].c_str()));
                double frac = H->GetBinContent(1) / H->GetEntries();
                fracFailed[i] = frac;
            }

            // Create a histogram and fill it with 'Dmass'
            TH1D* h = new TH1D("h", xLabel + " spectra" + (sd != "" ? " (" + sd + ")" : ""),//+ " Spectra (PYTHIA dijet, pthat=50, s=5.02TeV, UE on, Tun4C) (Thermal background: EventsMult=100, PtAv=0.85)", 
                                BIN_NUM,BIN_MIN,BIN_MAX);
            jetTree->Project("h", branchLabel, Form("%s>50", SOFT_DROP_PT_MAP[sd.Data()].c_str()));

            cout << "GOT TREE" << endl;

            h->Sumw2();
            // normalization 
            Double_t total = jetTree->GetEntries();
            Double_t factor = 1./total;
            h->Scale(factor, "width");

            h->GetXaxis()->CenterTitle();
            h->GetYaxis()->CenterTitle();

            // getting max bin thats filled
            h->GetXaxis()->SetRange(getMinXBin(h), getMaxXBin(h));
            h->GetXaxis()->SetTitleOffset(1.0);
            h->GetYaxis()->SetTitleOffset(1.5);
            h->SetMarkerStyle(20);
            h->SetMarkerSize(0.9);
            h->SetMarkerColor(colorIdx);
            h->SetLineColor(colorIdx);
            h->SetStats(0);
            // h->Draw("SAME");
            hs.Add(h);
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
        }
        hs.Draw("nostack");
        

        TPaveText *t = new TPaveText(-0.01, 0.0, 0.5, 0.09, "NB NDC");
        t->SetFillColor(0);
        t->SetTextSize(0.02);
        t->SetTextAlign(12);
        t->AddText("Input data: PYTHIA dijet, pthat=50, s=5.02TeV, UE on, Tune 4C");
        t->AddText("Background data: Thermal, EventsMult=7000, PtAv=1.20");
        t->AddText("p_{T}>50 GeV");
        if (sd != "") {
            t->AddText(
                Form("Fraction of jets that failed Soft Drop: %f (Pythia), %f (Pythia + Thermal),  %f (Pythia + CS), %f (Pythia + Thermal + CS)",
                fracFailed[0], fracFailed[1], fracFailed[2], fracFailed[3])
            );
        }
        t->Draw();
    
        if(branchLabel.Contains("Pt")) {
            gPad->SetLogy();
        }

        legend->SetTextSize(0.028);
        legend->SetFillColor(0);
        legend->SetBorderSize(1);
        legend->Draw("");

        c->Update();
        // c->SaveAs("plots/7k_pt50cutoff/" + branchLabel + "_v2.jpg");
        c->SaveAs("plots/test.jpg");

        if (WRITE_TO_PDF) {
            PdfFile.AddCanvas(c);
        }

        cout << "SAVED PLOT" << endl;
        outputfile->Write();
        outputfile->Close();
        cout << "WROTE TO OUTPUT FILE :')" << endl;
    }
    if (WRITE_TO_PDF) {
        PdfFile.AddTimeStampPage(); 
        PdfFile.Close(); 
    }
}

