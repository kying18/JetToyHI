#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("input");
   string OutputBase = CL.Get("output");
   double MinPT = CL.GetDouble("minpt", 200);
   string Title = CL.Get("title", "");

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("Lund Plane (pT > " + to_string((int) MinPT) + " GeV)");

    // stuff for the output csv
    ofstream outFile("./example.csv");
    // ostream_iterator<std::string> output_iterator(output_file, "\n");

    double jetCount = 0;
    
   for(string FileName : InputFileNames)
   {
        outFile << "\n" ;
        string titleMod;
        string label;
        // string data_label;

        if (FileName.find("ZJet") != string::npos){
            titleMod = " (ZJet) (Data: pyquen ppZJet150)";
            label = "pp150_1_zjet";
            // data_label = "Data using pyquen pp150 samples (pp_1)";
        } else {
            titleMod = " (Dijet) (Data: pyquen pp150)";
            label = "pp150_1";
            // data_label = "Data using pyquen ppZJet150 samples (pp_1)";
        }

        gStyle->SetTextSize(0.045);
        gStyle->SetTextFont(42);
        gStyle->SetPadRightMargin(0.05);
        gStyle->SetPadLeftMargin(0.15);
        gStyle->SetPadTopMargin(0.15);
        gStyle->SetPadBottomMargin(0.145);
        gStyle->SetTitleX(0.5);

        TCanvas* c = new TCanvas("c","",1200,900);
        
        TH1D HJetN("HJetN", ("Constituent Multiplicity" + titleMod + ";Constitutent Multiplicity;N").c_str(), 100, 0, 100);
        TH1D HJetPT("HJetPT", ("Jet PT" + titleMod + ";pT;N").c_str(), 100, MinPT, 500);
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

        Tree->SetBranchAddress("SignalJetNConstituent", &SignalJetNConstituent);
        Tree->SetBranchAddress("SignalJetPt", &SignalJetPt);
        Tree->SetBranchAddress("EventWeight", &Weight);

        int EntryCount = Tree->GetEntries();

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
                
                HJetN.Fill((*SignalJetNConstituent)[iJ], (*Weight)[0]);
                HJetPT.Fill((*SignalJetPt)[iJ], (*Weight)[0]);

                jetCount = jetCount + (*Weight)[0];
            }
        }
        // HJetN.SaveAs((exportTitle + ".C").c_str());

        std::vector<double> v; 
        for (int i = 0; i < HJetN.GetNbinsX(); i++) {
            v.push_back(HJetN.GetBinContent(i + 1)); 
        }

        // copy(v.begin(), v.end(), output_iterator);
        outFile << label << ",";
        for (const auto &e : v) outFile << e << ",";

        PdfFile.AddPlot(HJetN, "", true);
        PdfFile.AddPlot(HJetPT, "", true);

        File.Close();
        
   }

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




