#include <iostream>
#include <fstream>
#include <iterator>
#include <stdio.h>
#include <dirent.h>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "uti.h"

// g++ getJetTopicsHists.cpp $(root-config --cflags --libs) -O2 -o "getJetTopicHists.exe"
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/ppZJet150" -minpt 0 -output ./150_1_1.14.2020/150_1_zjet_thstack_pt0
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150,/data/kying/EMMIResults/PbPbWide150_0_10,/data/kying/EMMIResults/PbPb150_0_10" -minpt 100 -output ./150_1_1.14.2020/150_1_dijet_thstack_pt100
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150,/data/kying/EMMIResults/PbPbWide150_0_10,/data/kying/EMMIResults/PbPb150_0_10" -minpt 0 -output ./150_1_1.14.2020/150_1_dijet_thstack_pt0
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150.root,/data/kying/EMMIResults/PbPbWide150_0_10_1.root,/data/kying/EMMIResults/PbPb150_0_10_1.root" -minpt 0 -output ./150_1_1.14.2020/150_1_dijet_thstack_pt0
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150.root,/data/kying/EMMIResults/PbPbWide150_0_10_1.root,/data/kying/EMMIResults/PbPb150_0_10_1.root" -minpt 50 -output ./150_1_1.14.2020/150_1_dijet_thstack_pt50
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150.root,/data/kying/EMMIResults/PbPbWide150_0_10_1.root,/data/kying/EMMIResults/PbPb150_0_10_1.root" -minpt 100 -output ./150_1_1.14.2020/150_1_dijet_thstack_pt100
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150.root,/data/kying/EMMIResults/PbPbWide150_0_10_1.root,/data/kying/EMMIResults/PbPb150_0_10_1.root" -minpt 200 -output ./150_1_1.14.2020/150_1_dijet_thstack_pt200
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150.root,/data/kying/EMMIResults/ppZJet150.root" -minpt 0 -output ./150_1_1.14.2020/150_1_pp_thstack_pt0
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150.root,/data/kying/EMMIResults/ppZJet150.root" -minpt 50 -output ./150_1_1.14.2020/150_1_pp_thstack_pt50
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150.root,/data/kying/EMMIResults/ppZJet150.root" -minpt 100 -output ./150_1_1.14.2020/150_1_pp_thstack_pt100
// ./getJetTopicHists.exe -input "/data/kying/EMMIResults/pp150.root,/data/kying/EMMIResults/ppZJet150.root" -minpt 200 -output ./150_1_1.14.2020/150_1_pp_thstack_pt200

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileFolders = CL.GetStringVector("input");
   string OutputBase = CL.Get("output");
   double MinPT = CL.GetDouble("minpt", 200);
   string Title = CL.Get("title", "");

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("Jet Topics Data Plots (pT > " + to_string((int) MinPT) + " GeV)");
   vector<string> lines;
   lines.push_back("Pyquen data source location:");
   lines.push_back("/data/yjlee/pyquen/pp150/pp_1.pu14");
//    lines.push_back("/data/yjlee/pyquen/ppZJet150/pp_1.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPb150_0_10/PbPb_0_10_1.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPbWide150_0_10/PbPbWide_0_10_1.pu14");
   PdfFile.AddTextPage(lines, 0.16, 0.6, 0.03);


    // stuff for the output csv
    ofstream outFile("./150_1_histograms_pt" + to_string((int) MinPT) + ".csv", ios_base::app);
    ostream_iterator<std::string> output_iterator(outFile, "\n");

    THStack* hNStack = new THStack("hNStack","Constituent Multiplicity (pyquen dijet samples);Constituent Multiplicity;1/N dN/d Constituent Multiplicity");
    THStack* hPTStack = new THStack("hPTStack","pT (pyquen dijet samples);pT;1/N dN/dpT");
    TLegend* legend = new TLegend(0.67,0.75,0.97,0.92);
    // TCanvas* cn = new TCanvas("cn","",1200,900);
    // TCanvas* cpt = new TCanvas("cpt","",1200,900);

    double jetCount = 0;
    
   for(string folder : InputFileFolders)
   {
        // outFile << "\n" ;

        // some tools to define the plots
        string titleMod;
        string label;
        TString legendLabel;
        // string data_label;
        struct dirent *de; // to get files in path
        DIR *dr = opendir(folder.c_str()); 
        if (dr == NULL)  // opendir returns NULL if couldn't open directory 
        { 
            cout << "Could not open directory: " << folder << endl;
            return 0; 
        } 

        int colorIdx;
        if (folder.find("ZJet") != string::npos){
            titleMod = " (pp z-jet) (Data: pyquen ppZJet150)";
            label = "pp150_zjet";
            legendLabel = "ppZJet150";
            colorIdx = 1;
            // data_label = "Data using pyquen pp150 samples (pp_1)";
        } else if (folder.find("PbPbWide") != string::npos){
            titleMod = " (PbPb wide) (Data: pyquen PbPbWide150_0_10)";
            label = "pbpb150_0_10_wide";
            legendLabel = "PbPbWide150";
            colorIdx = 3;
            // data_label = "Data using pyquen pp150 samples (pp_1)";
        } else if (folder.find("PbPb") != string::npos){
            titleMod = " (PbPb) (Data: pyquen PbPb150_0_10)";
            label = "pbpb150_0_10";
            legendLabel = "PbPb150";
            colorIdx = 4;
            // data_label = "Data using pyquen pp150 samples (pp_1)";
        } else {
            titleMod = " (pp dijet) (Data: pyquen pp150)";
            label = "pp150";
            legendLabel = "pp150";
            colorIdx = 2;
            // data_label = "Data using pyquen ppZJet150 samples (pp_1)";
        }
        
        TH1D* HJetN = new TH1D("HJetN", ("Constituent Multiplicity" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 100, 0, 100);
        TH1D* HJetNZJet = new TH1D("HJetNZJet", ("NConstituent (leading jet, opp hemisphere)" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 100, 0, 100);
        TH1D* HJetPT = new TH1D("HJetPT", ("Jet PT" + titleMod + ";pT;1/N dN/dpT").c_str(), 100, MinPT, 500);
        TH2D * h2 = new TH2D("h2", ("pT vs Constituent Multiplicity" + titleMod + ";Constituent Multiplicity;pT").c_str(), 100, 0, 100, 100, MinPT, 500);
        // for each file in the folder, combine the statistics
        // for (const auto & entry : boost::filesystem::directory_iterator(folder)){
        while ((de = readdir(dr)) != NULL) {
            if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ) continue;
            string fileName = de->d_name;
            TFile File((folder + "/" + fileName).c_str());

            TTree *Tree = (TTree *)File.Get("JetTree");

            if(Tree == nullptr)
            {
                File.Close();
                continue;
            }

            vector<double> *SignalJetNConstituent = nullptr;
            vector<double> *SignalJetPt = nullptr;
            vector<double> *Weight = nullptr;
            vector<double> *SignalJetPhi = nullptr;
            vector <double> *LeadingZPhi = nullptr;

            Tree->SetBranchAddress("SignalJetNConstituent", &SignalJetNConstituent);
            Tree->SetBranchAddress("SignalJetPt", &SignalJetPt);
            Tree->SetBranchAddress("SignalJetPhi", &SignalJetPhi);
            Tree->SetBranchAddress("LeadingZPhi", &LeadingZPhi);
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
                double MaxPT = 0; // to find max pt jet in event
                int MaxPTEventIndex = -1;
                double ZBosonPhi = (*LeadingZPhi)[0];
                double JetPhi;

                for(int iJ = 0; iJ < NJet; iJ++)
                {
                    if((*SignalJetPt)[iJ] < MinPT) continue;

                    JetPhi = (*SignalJetPhi)[iJ];
                    // TODO: might need to change zbosonphi < 0 part dep on if theres no leading Z,
                    // do we skip the jet or ..
                    if(ZBosonPhi >= 0) {
                        double DPhi = ZBosonPhi - JetPhi;

                        if(DPhi < -M_PI)   DPhi = DPhi + 2 * M_PI;
                        if(DPhi > +M_PI)   DPhi = DPhi - 2 * M_PI;

                        if(DPhi < - M_PI / 2 || DPhi > M_PI / 2) {
                            if((*SignalJetPt)[iJ] > MaxPT) {
                                MaxPTEventIndex = iJ;
                                MaxPT = (*SignalJetPt)[iJ];
                            }
                        }
                    }
                    
                    HJetN->Fill((*SignalJetNConstituent)[iJ], (*Weight)[0]);
                    HJetPT->Fill((*SignalJetPt)[iJ], (*Weight)[0]);
                    h2->Fill((*SignalJetNConstituent)[iJ], (*SignalJetPt)[iJ]);

                    nsFiltered.push_back((*SignalJetNConstituent)[iJ]);
                    ptsFiltered.push_back((*SignalJetPt)[iJ]);

                    jetCount = jetCount + (*Weight)[0];
                }
                if (MaxPTEventIndex >=0) {
                    HJetNZJet->Fill((*SignalJetNConstituent)[MaxPTEventIndex], (*Weight)[0]);
                }
            }
            File.Close();
        }

        std::vector<double> v; 
        std::vector<double> v_error;
        for (int i = 0; i < HJetN->GetNbinsX(); i++) {
            // cout << "line: " << HJetN->GetBinContent(i + 1) << ", " << HJetN->GetBinContent(i + 1) * HJetN->GetEntries() << endl;
            v.push_back(HJetN->GetBinContent(i + 1)); 
            v_error.push_back(HJetN->GetBinError(i + 1));
        }

        // put csv lines in csv
        // copy(v.begin(), v.end(), output_iterator);
        outFile << label << ",";
        for (const auto &e : v) outFile << e << ",";
        outFile << endl;
        outFile << label << "_error" << ",";
        for (const auto &e : v_error) outFile << e << ",";
        outFile << endl;

        TH1D* hists[3] = {HJetN, HJetPT, HJetNZJet};
        // TH1D* hists[1] = {HJetN};
        for (int i=0; i < 3; i++) {
            TH1D* h = hists[i];
            gStyle->SetTextSize(0.045);
            gStyle->SetTextFont(42);
            gStyle->SetPadRightMargin(0.05);
            gStyle->SetPadLeftMargin(0.15);
            gStyle->SetPadTopMargin(0.15);
            gStyle->SetPadBottomMargin(0.145);
            gStyle->SetTitleX(0.5);
            // TCanvas* c = new TCanvas("c","",1200,900);
            h->GetYaxis()->SetTitleOffset(1.6);
            h->Sumw2();
            h->Scale(1./h->GetEntries(), "width");
            h->SetMarkerStyle(20);
            h->SetMarkerSize(0.9);
            h->SetMarkerColor(colorIdx);
            h->SetLineColor(colorIdx);
            h->SetStats(0);
            if (i == 0) {
                hNStack->Add(h);
                legend->AddEntry(h, legendLabel, "p"); // we only need to add this once
            } else if (i == 1) {
                hPTStack->Add(h);
            }
        }

        TCanvas* cscatter = new TCanvas("cscatter", "", 1200, 900);

        // cscatter->SetLogy();
        h2->Scale(1./h2->GetEntries(), "width");
        h2->Draw("COLZ");
        cscatter->Update();
        cscatter->SaveAs((TString) (OutputBase + label + "scatter.jpg"));
        PdfFile.AddCanvas(cscatter);

        TCanvas* cz = new TCanvas("cz", "", 1200, 900);
        HJetNZJet->Draw();
        cz->SetLogy();
        cz->SaveAs((TString) (OutputBase + "nconst_opphem.jpg"));
   }

    TCanvas* cn = new TCanvas("cn","",1200,900);
    // int yMaxN = hNStack->GetMaximum();
    // if (yMaxN < 35) {
    //     hNStack->SetMaximum(3*yMaxN);
    // }
    hNStack->Draw("nostackb");
    legend->SetTextSize(0.028);
    legend->SetFillColor(0);
    legend->SetBorderSize(1);
    legend->Draw("");
    cn->SetLogy();
    cn->SaveAs((TString) (OutputBase + "nstack.jpg"));
    PdfFile.AddCanvas(cn);

    TCanvas* cpt = new TCanvas("cpt","",1200,900);
    // int yMaxPT = hPTStack->GetMaximum();
    // cout << yMaxPT << endl;
    // if (yMaxPT < 35) {
    //     hNStack->SetMaximum(3*yMaxPT);
    // }
    // hPTStack->SetMaximum(max(60, yMaxPT));
    hPTStack->Draw("nostackb");
    legend->SetTextSize(0.028);
    legend->SetFillColor(0);
    legend->SetBorderSize(1);
    legend->Draw("");
    cpt->SetLogy();
    cpt->Update();
    cpt->SaveAs((TString) (OutputBase + "ptstack.jpg"));
    PdfFile.AddCanvas(cpt);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




