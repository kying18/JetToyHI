#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "uti.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("input");
   string OutputBase = CL.Get("output");
   double MinPT = CL.GetDouble("minpt", 200);
   string Title = CL.Get("title", "");

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("Jet Topics Data Plots (pT > " + to_string((int) MinPT) + " GeV)");

    // stuff for the output csv
    // ofstream outFile("./example.csv");
    // ostream_iterator<std::string> output_iterator(output_file, "\n");

    // THStack* hNStack = new THStack("hNStack","Constituent Multiplicity;Constituent Multiplicity;1/N dN/d Constituent Multiplicity");
    // THStack* hPTStack = new THStack("hs","pT;pT;1/N dN/dpT");
    // TCanvas* cn = new TCanvas("cn","",1200,900);
    // TCanvas* cpt = new TCanvas("cpt","",1200,900);

    double jetCount = 0;
    

   for(string FileName : InputFileNames)
   {
        // outFile << "\n" ;

        // some tools to define the plots
        string titleMod;
        string label;
        // string data_label;

        int colorIdx;
        if (FileName.find("ZJet") != string::npos){
            titleMod = " (ZJet) (Data: pyquen ppZJet150)";
            label = "pp150_1_zjet";
            colorIdx = 1;
            // data_label = "Data using pyquen pp150 samples (pp_1)";
        } else if (FileName.find("PbPbWide") != string::npos){
            titleMod = " (PbPb wide) (Data: pyquen PbPbWide150_0_10)";
            label = "pbpb150_0_10_1_wide";
            colorIdx = 3;
            // data_label = "Data using pyquen pp150 samples (pp_1)";
        } else if (FileName.find("PbPb") != string::npos){
            titleMod = " (PbPb) (Data: pyquen PbPb150_0_10)";
            label = "pbpb150_0_10_1";
            colorIdx = 4;
            // data_label = "Data using pyquen pp150 samples (pp_1)";
        } else {
            titleMod = " (Dijet) (Data: pyquen pp150)";
            label = "pp150_1";
            colorIdx = 2;
            // data_label = "Data using pyquen ppZJet150 samples (pp_1)";
        }
        
        TH1D* HJetN = new TH1D("HJetN", ("Constituent Multiplicity" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 100, 0, 100);
        TH1D* HJetPT = new TH1D("HJetPT", ("Jet PT" + titleMod + ";pT;1/N dN/dpT").c_str(), 100, MinPT, 500);
        TFile File(FileName.c_str());

        TTree *Tree = (TTree *)File.Get("JetTree");

        if(Tree == nullptr)
        {
            File.Close();
            continue;
        }

        vector<double> *SignalJetNConstituent = nullptr;
        vector<double> *SignalJetPt = nullptr;
        vector<double> *Weight = nullptr;

        Tree->SetBranchAddress("SignalJet04NConstituent", &SignalJetNConstituent);
        Tree->SetBranchAddress("SignalJet04Pt", &SignalJetPt);
        Tree->SetBranchAddress("EventWeight", &Weight);

        int EntryCount = Tree->GetEntries();
        vector<double> ptsFiltered;
        vector<double> nsFiltered;
        for(int iE = 0; iE < EntryCount; iE++)
        {
            Tree->GetEntry(iE);

            if(SignalJetPt == nullptr)
                continue;

            int NJet = SignalJetPt->size();
            for(int iJ = 0; iJ < NJet; iJ++)
            {
                if((*SignalJetPt)[iJ] < MinPT)
                continue;
                
                HJetN->Fill((*SignalJetNConstituent)[iJ], (*Weight)[0]);
                HJetPT->Fill((*SignalJetPt)[iJ], (*Weight)[0]);

                nsFiltered.push_back((*SignalJetNConstituent)[iJ]);
                ptsFiltered.push_back((*SignalJetPt)[iJ]);

                jetCount = jetCount + (*Weight)[0];
            }
        }

        TH1D* hists[2] = {HJetN, HJetPT};
        // TH1D* hists[1] = {HJetN};
        for (auto h : hists) {
            gStyle->SetTextSize(0.045);
            gStyle->SetTextFont(42);
            gStyle->SetPadRightMargin(0.05);
            gStyle->SetPadLeftMargin(0.15);
            gStyle->SetPadTopMargin(0.15);
            gStyle->SetPadBottomMargin(0.145);
            gStyle->SetTitleX(0.5);
            TCanvas* c = new TCanvas("c","",1200,900);
            h->GetYaxis()->SetTitleOffset(1.6);
            h->Sumw2();
            h->Scale(1./EntryCount, "width");
            h->SetMarkerStyle(20);
            h->SetMarkerSize(0.9);
            h->SetMarkerColor(colorIdx);
            h->SetLineColor(colorIdx);
            h->SetStats(0);
            h->Draw();
            c->Update();
            c->SaveAs("test.jpg");
            PdfFile.AddCanvas(c);
        }

        TCanvas* cscatter = new TCanvas("cscatter", "", 1200, 900);
        Int_t n = nsFiltered.size();

        Double_t x[n], y[n];
        for (Int_t i=0; i<n; i++) {
            x[i] = nsFiltered.at(i);
            y[i] = ptsFiltered.at(i);
        }
        
        TGraph *gr = new TGraph(n, x, y);
        // gr->SetLineColor(2);
        // gr->SetLineWidth(4);
        gr->SetMarkerColor(colorIdx);
        gr->SetMarkerSize(0.8);
        // gr->SetMarkerStyle(21);
        gr->SetTitle(("pT vs Constituent Multiplicity" + titleMod).c_str());
        gr->GetXaxis()->SetTitle("Constituent Multiplicity");
        gr->GetYaxis()->SetTitle("pT");
        gr->Draw("ap");
        cscatter->Update();
        cscatter->SaveAs("test_scatter.jpg");
        PdfFile.AddCanvas(cscatter);

        // HJetN->Sumw2();
        // HJetN->Scale(1./EntryCount, "width");
        // HJetN->SetMarkerStyle(20);
        // HJetN->SetMarkerSize(0.9);
        // HJetN->SetStats(0);

        // hNStack->Add(HJetN);
        // hPTStack->Add(HJetPT);


        // std::vector<double> v; 
        // for (int i = 0; i < HJetN.GetNbinsX(); i++) {
        //     v.push_back(HJetN.GetBinContent(i + 1)); 
        // }

        // // copy(v.begin(), v.end(), output_iterator);
        // outFile << label << ",";
        // for (const auto &e : v) outFile << e << ",";

        // PdfFile.AddPlot(HJetN, "", true);
        // PdfFile.AddPlot(HJetPT, "", true);

        File.Close();
        
   }

    // gStyle->SetHistTopMargin(0.25);
    // hNStack->Draw("nostackb");
    // cn->Update();
    // cn->SaveAs("stack_test.jpg");
    // hPTStack->Draw("nostackb");

    // cout << firstBin << endl;
    // hs->GetXaxis()->SetRange(firstBin-2, lastBin+2);
    // cout << maxY << endl;
    // hs->Draw("nostackb");

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




