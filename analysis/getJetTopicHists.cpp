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
// #include "LabelHelper.h"

// g++ getJetTopicHists.cpp $(root-config --cflags --libs) -O2 -o "getJetTopicHists.exe"
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/ppZJet80" -minpt 0 -output ./80_1_2.8.2021/80_1_zjet_thstack_pt0
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp80,/data/kying/jetTopicsData/PbPbWide80_0_10,/data/kying/jetTopicsData/PbPb80_0_10" -minpt 100 -output ./80_1_2.8.2021/80_1_dijet_thstack
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp80,/data/kying/jetTopicsData/PbPbWide80_0_10,/data/kying/jetTopicsData/PbPb80_0_10" -minpt 0 -output ./80_1_2.8.2021/80_1_dijet_thstack

// with thermal!!
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp80,/data/kying/jetTopicsData/PbPbWide80_0_10,/data/kying/jetTopicsData/PbPb80_0_10,/data/kying/jetTopicsData/pp80_7000thermal,/data/kying/jetTopicsData/PbPbWide80_0_10_7000thermal,/data/kying/jetTopicsData/PbPb80_0_10_7000thermal" -minpt 100 -output ./80_1_7000thermal/80_1_dijet_thstack
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp80,/data/kying/jetTopicsData/PbPbWide80_0_10,/data/kying/jetTopicsData/PbPb80_0_10,/data/kying/jetTopicsData/pp80_7000thermal,/data/kying/jetTopicsData/PbPbWide80_0_10_7000thermal,/data/kying/jetTopicsData/PbPb80_0_10_7000thermal" -minpt 0 -output ./80_1_7000thermal/80_1_dijet_thstack
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp80_7000thermal,/data/kying/jetTopicsData/PbPbWide80_0_10_7000thermal,/data/kying/jetTopicsData/PbPb80_0_10_7000thermal" -minpt 0 -output ./80_1_7000thermal/80_1_dijet_thstack

// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/ppZJet80,/data/kying/jetTopicsData/PbPbWideZJet80_0_10,/data/kying/jetTopicsData/ppPhotonJet80,/data/kying/jetTopicsData/PbPbWidePhotonJet80_0_10,/data/kying/jetTopicsData/pp80,/data/kying/jetTopicsData/PbPb80_0_10,/data/kying/jetTopicsData/PbPbWide80_0_10" -minpt 0 -output "./80_1_all/80_1_dijetzjetphotonjet"
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/ppPhotonJet80,/data/kying/jetTopicsData/PbPbWidePhotonJet80_0_10" -minpt 0 -output "./80_photon/80_1_photonjet"
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/test" -minpt 0 -output "./80_photon/80_1_photonjet" // photon jet
// run -input "/data/kying/jetTopicsData/ppZJet80" -minpt 0 -output "test"

tuple<string, string, TString, int> getDataLabel(string folder, vector<int> &colors, vector<int> &colorsLight) {
    // given the folder, return how to modify the title, the label of the output file, and the label in the legend
    int colorIdx;
    string titleMod,label;
    TString legendLabel;
    if (folder.find("7000thermal") != string::npos) {
        if (folder.find("ppZJet") != string::npos){
            titleMod = " (pp z-jet with thermal) (Data: pyquen ppZJet80)";
            label = "pp80_zjet_thermal";
            legendLabel = "ppZJet80 w/ thermal";
            colorIdx = kGreen;
        } else if (folder.find("PbPbWideZJet") != string::npos) {
            titleMod = " (PbPb wide z-jet with thermal) (Data: pyquen PbPbWideZJet80)";
            label = "pbpb80_0_10_wide_zjet_thermal";
            legendLabel = "PbPbWideZJet80 w/ thermal";
            colorIdx = kCyan-3;
        } else if (folder.find("ppPhotonJet") != string::npos) {
            titleMod = " (pp photon jet with thermal) (Data: pyquen ppPhotonJet80)";
            label = "pp80_photonjet_thermal";
            legendLabel = "ppPhotonJet80 w/ thermal";
            colorIdx = kBlue;
        } else if (folder.find("PbPbWidePhotonJet") != string::npos) {
            titleMod = " (PbPb wide photon jet with thermal) (Data: pyquen PbPbWidePhotonJet80)";
            label = "pbpb80_0_10_wide_photonjet_thermal";
            legendLabel = "PbPbWidePhotonJet80 w/ thermal";
            colorIdx = kMagenta-7;
        } else if (folder.find("PbPbWide") != string::npos){
            titleMod = " (PbPb wide with thermal) (Data: pyquen PbPbWide80_0_10)";
            label = "pbpb80_0_10_wide_thermal";
            legendLabel = "PbPbWide80 w/ thermal";
            colorIdx = kRed;
        } else if (folder.find("PbPb") != string::npos){
            titleMod = " (PbPb with thermal) (Data: pyquen PbPb80_0_10)";
            label = "pbpb80_0_10_thermal";
            legendLabel = "PbPb80 w/ thermal";
            colorIdx = kOrange+1;
        } else if (folder.find("pp") != string::npos){
            titleMod = " (pp dijet with thermal) (Data: pyquen pp80)";
            label = "pp80_thermal";
            legendLabel = "pp80 w/ thermal";
            colorIdx = kViolet-6;
        }
    } else {
        if (folder.find("ppZJet") != string::npos){
            titleMod = " (pp z-jet) (Data: pyquen ppZJet80)";
            label = "pp80_zjet";
            legendLabel = "ppZJet80";
            // colorIdx = kGreen;
            colorIdx = colors[0];
        } else if (folder.find("PbPbWideZJet") != string::npos) {
            titleMod = " (PbPb wide z-jet) (Data: pyquen PbPbWideZJet80)";
            label = "pbpb80_0_10_wide_zjet";
            legendLabel = "PbPbWideZJet80";
            // colorIdx = kCyan-3;
            colorIdx = colorsLight[0];
        } else if (folder.find("ppPhotonJet") != string::npos) {
            titleMod = " (pp photon jet) (Data: pyquen ppPhotonJet80)";
            label = "pp80_photonjet";
            legendLabel = "ppPhotonJet80";
            // colorIdx = kBlue;
            colorIdx = colors[2];
        } else if (folder.find("PbPbWidePhotonJet") != string::npos) {
            titleMod = " (PbPb wide photon jet) (Data: pyquen PbPbWidePhotonJet80)";
            label = "pbpb80_0_10_wide_photonjet";
            legendLabel = "PbPbWidePhotonJet80";
            // colorIdx = kMagenta-7;
            colorIdx = colorsLight[2];
        } else if (folder.find("PbPbWide") != string::npos){
            titleMod = " (PbPb wide) (Data: pyquen PbPbWide80_0_10)";
            label = "pbpb80_0_10_wide";
            legendLabel = "PbPbWide80";
            // colorIdx = kRed;
            colorIdx = colors[4];
        } else if (folder.find("PbPb") != string::npos){
            titleMod = " (PbPb) (Data: pyquen PbPb80_0_10)";
            label = "pbpb80_0_10";
            legendLabel = "PbPb80";
            // colorIdx = kOrange+1;
            colorIdx = colorsLight[4];
        } else if (folder.find("pp") != string::npos){
            titleMod = " (pp dijet) (Data: pyquen pp80)";
            label = "pp80";
            legendLabel = "pp80";
            // colorIdx = kViolet-6;
            colorIdx = colors[5];
        }
    }

    return make_tuple(titleMod, label, legendLabel, colorIdx);
}

bool oppositeHemisphere(double particlePhi, double jetPhi) {
    if(particlePhi >= 0) {
        double dPhi = particlePhi - jetPhi;
        if(dPhi < -M_PI)   dPhi = dPhi + 2 * M_PI;
        if(dPhi > +M_PI)   dPhi = dPhi - 2 * M_PI;

        if(dPhi < - M_PI / 2 || dPhi > M_PI / 2) {
            return true;
        }
        return false;
    }
    // todo: what if there is no leading Z?
    return false; // false for now.. don't count it
}

TCanvas * plotQGHistograms(TH1D *hQuark, TH1D *hGluon, THStack *hStack, bool log, vector<int> &colors, vector<int> &colorsLight){
    TLegend* qglegend = new TLegend(0.6,0.8,0.97,0.92);

    // hQuark->Sumw2();
    hQuark->SetFillColor(colors[3]);
    hQuark->SetMarkerColor(colors[3]);
    hQuark->SetLineColor(colorsLight[3]);
    hQuark->SetFillStyle(1001);
    hStack->Add(hQuark);
    qglegend->AddEntry(hQuark, "Quark-like", "p"); // we only need to add this once
    // hGluon->Sumw2();
    hGluon->SetFillColor(colors[6]);
    hGluon->SetMarkerColor(colors[6]);
    hGluon->SetLineColor(colorsLight[6]);
    hGluon->SetFillStyle(1001);
    hStack->Add(hGluon);
    qglegend->AddEntry(hGluon, "Gluon-like", "p"); // we only need to add this once

    TCanvas* cqg = new TCanvas("cqg","",1200,900);
    hStack->Draw();
    qglegend->SetTextSize(0.028);
    qglegend->SetFillColor(0);
    qglegend->SetBorderSize(1);
    qglegend->Draw("");

    if (log) {
        cqg->SetLogy();
    }

    cqg->Update();
    return cqg;
}

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileFolders = CL.GetStringVector("input");
   string OutputBase = CL.Get("output");
   
   double MinPT = CL.GetDouble("minpt", 0);
   double MaxPT = CL.GetDouble("maxpt", 500);
   string Title = CL.Get("title", "");
   string SoftDrop = CL.Get("sd", ""); // ex: -sd SD1
   double MinTrackPT = CL.GetDouble("mintrkpt", 0);
   double MinEta = CL.GetDouble("mineta", 0);
   double MaxEta = CL.GetDouble("maxeta", 1);

//    string outputLabel = OutputBase + "_pt" + to_string((int) MinPT) + "_trkpt" + to_string((int)MinTrackPT) + "_" + SoftDrop;
   string outputLabel = OutputBase + "pt" + to_string((int) MinPT) + to_string((int) MaxPT);
//    string outputLabel = OutputBase + "pt" + to_string((int) MinPT);

   PdfFileHelper PdfFile(outputLabel + ".pdf");
   PdfFile.AddTextPage("Jet Topics Data Plots (" + to_string((int) MinPT) + " GeV" + " < pT < " + to_string((int) MaxPT) + " GeV)");
   vector<string> lines;
   lines.push_back("Pyquen data source location:");
   lines.push_back("/data/yjlee/pyquen/pp80/pp_*.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPb80_0_10/PbPb_0_10_*.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPbWide80_0_10/PbPbWide_0_10_*.pu14");

   lines.push_back("/data/yjlee/pyquen/ppPhotonJet80/pp_*.pu14");
   lines.push_back("/data/yjlee/pyquen/ppZJet80/pp_*.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPbWidePhotonJet80_0_10/PbPbWide_0_10_*.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPbWideZJet80_0_10/PbPbWide_0_10_*.pu14");

   lines.push_back("Thermal pileup location:");
   lines.push_back("/data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz");
   PdfFile.AddTextPage(lines, 0.1, 0.9, 0.03);

    vector<int> colors = GetDifferentiableColors();
    vector<int> colorsLight = GetDifferentiableColorsLight();

    // stuff for the output csv
    ofstream outFile(outputLabel + "histogram.csv", ios_base::app);
    // ostream_iterator<std::string> output_iterator(outFile, "\n");

    // ofstream outFileMass(outputLabel + "mass.csv", ios_base::app);
    // // ostream_iterator<std::string> output_iterator_2(outFileMass, "\n");
    // outFileMass << "sample,gluon,quark" << endl;

    THStack* hNStack = new THStack("hNStack",(SoftDrop + " Constituent Multiplicity (pyquen samples);" + SoftDrop + " Constituent Multiplicity;1/N dN/d " + SoftDrop + " Constituent Multiplicity").c_str());
    THStack* hPTStack = new THStack("hPTStack","pT (pyquen samples);pT;1/N dpT");
    // THStack* hPTStack2 = new THStack("hPTStack2","pT (pyquen samples);pT;1/N dpT");
    THStack* hNLeadingStack = new THStack("hNLeadingStack","NConstituent (leading jet(s));Constitutent Multiplicity;1/N dN");
    // THStack* hNQuarkStack = new THStack("hNQuarkStack","NConstituent (leading jet(s), quark-like);Constitutent Multiplicity;1/N dN");
    // THStack* hNGluonStack = new THStack("hNGluonStack","NConstituent (leading jet(s), gluon-like);Constitutent Multiplicity;1/N dN");
    THStack* hPTTrackStack = new THStack("hPTTrackStack","Particle pT (pyquen samples);pT;1/N dpT");
    TLegend* legend = new TLegend(0.6,0.7,0.97,0.92);
    // TCanvas* cn = new TCanvas("cn","",1200,900);
    // TCanvas* cpt = new TCanvas("cpt","",1200,900);

    double jetCount = 0;

    const int NUM_GRAPHS = 6;
    
   for(string folder : InputFileFolders)
   {
        // int numMassEmptyQ = 0;
        // int numMassEmptyG = 0;
        bool zJet = false;
        bool photonJet = false;
        bool dijet = false;

        if (folder.find("ZJet") != string::npos){
            zJet = true;
        } else if (folder.find("PhotonJet") != string::npos){
            photonJet = true;
        } else {
            dijet = true;
        }

        // some tools to define the plots
        string titleMod, label;
        TString legendLabel;
        int colorIdx;
        
        struct dirent *de; // to get files in path
        DIR *dr = opendir(folder.c_str()); 
        if (dr == NULL)  // opendir returns NULL if couldn't open directory 
        { 
            cout << "Could not open directory: " << folder << endl;
            return 0;
        }

        tie(titleMod, label, legendLabel, colorIdx) = getDataLabel(folder, colors, colorsLight);
        
        cout << "Label of folder: " << legendLabel << endl;

        THStack* hNQuarkGluonStack = new THStack("hNQuarkGluonStack",("NConstituent (leading jet(s))" + titleMod + ";Constitutent Multiplicity;N").c_str());
        THStack* hDRStack = new THStack("hDRStack",("dR (between Jet and Quark/Gluon)" + titleMod + ";dR;N").c_str());
        THStack* hPtStack = new THStack("hPtStack",("Matrix Element pT" + titleMod + ";pT;N").c_str());
        THStack* hPtStack2 = new THStack("hPtStack2",("Matrix Element pT" + titleMod + ";pT;N").c_str());
        THStack* hMassStack = new THStack("hMassStack",("Matrix Element Mass" + titleMod + ";Mass;N").c_str());
        // TLegend* qglegend = new TLegend(0.6,0.8,0.97,0.92);
        
        TH1D* HJetN = new TH1D("HJetN", (SoftDrop + " Constituent Multiplicity" + titleMod + ";" + SoftDrop + " Constitutent Multiplicity;1/N dN").c_str(), 120, 0, 120);
        TH1D* HJetPTTrack = new TH1D("HJetPTTrack", ("Particle PT" + titleMod + ";pT;1/N dpT").c_str(), 200, 0, 200);
        TH1D* HJetNLeading = new TH1D("HJetNLeading", ("NConstituent (leading jet(s))" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 120, 0, 120);
        TH1D* HJetNQuarkTruth = new TH1D("HJetNQuarkTruth", ("NConstituent (leading jet(s))" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 120, 0, 120);
        TH1D* HJetNGluonTruth = new TH1D("HJetNGluonTruth", ("NConstituent (leading jet(s))" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 120, 0, 120);
        TH1D* HJetNQuark = new TH1D("HJetNQuark", ("NConstituent (leading jet(s), quark-like)" + titleMod + ";Constitutent Multiplicity;N").c_str(), 120, 0, 120);
        TH1D* HJetNGluon = new TH1D("HJetNGluon", ("NConstituent (leading jet(s), gluon-like)" + titleMod + ";Constitutent Multiplicity;N").c_str(), 120, 0, 120);
        TH1D* HJetDRQuark = new TH1D("HJetDRQuark", ("dR" + titleMod + ";dR;N").c_str(), 100, 0, 5);
        TH1D* HJetDRGluon = new TH1D("HJetDRGluon", ("dR" + titleMod + ";dR;N").c_str(), 100, 0, 5);
        TH1D* HJetPtQuark = new TH1D("HJetPtQuark", ("Matrix Element pT" + titleMod + ";pT;N").c_str(), 100, 0, 100);
        TH1D* HJetPtGluon = new TH1D("HJetPtGluon", ("Matrix Element pT" + titleMod + ";pT;N").c_str(), 100, 0, 100);
        TH1D* HJetPtQuark2 = new TH1D("HJetPtQuark", ("Matrix Element pT" + titleMod + ";pT;N").c_str(), 400, 0, 400);
        TH1D* HJetPtGluon2 = new TH1D("HJetPtGluon", ("Matrix Element pT" + titleMod + ";pT;N").c_str(), 400, 0, 400);
        TH1D* HJetMassQuark = new TH1D("HJetMassQuark", ("Matrix Element Mass" + titleMod + ";Mass;N").c_str(), 50, 0, 5);
        TH1D* HJetMassGluon = new TH1D("HJetMassGluon", ("Matrix Element Mass" + titleMod + ";Mass;N").c_str(), 50, 0, 5);
        TH1D* HJetPT = new TH1D("HJetPT", ("Jet PT" + titleMod + ";pT;1/N dpT").c_str(), 100, MinPT, 500);
        TH2D * h2 = new TH2D("h2", ("pT vs " + SoftDrop + " Constituent Multiplicity" + titleMod + ";" + SoftDrop + " Constituent Multiplicity;pT").c_str(), 120, 0, 120, 100, MinPT, 500);
        TH2D * hMatElem = new TH2D("hMatElem", ("Constituent Multiplicity vs Matrix Element pT" + titleMod + ";Matrix Element pT;Constituent Multiplicity").c_str(), 50, 0, 50, 20, 0, 20);
        

        int fileCount = 0; // REMOVE
        while ((de = readdir(dr)) != NULL) {
            if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ) continue;
            // if (fileCount >= 20) continue; // REMOVE
            string fileName = de->d_name;
            TFile File((folder + "/" + fileName).c_str());

            // cout << "Filename: " << fileName << endl;

            TTree *Tree = (TTree *)File.Get("JetTree");

            if(Tree == nullptr)
            {
                File.Close();
                continue;
            }

            fileCount += 1; // REMOVE

            vector<double> *SignalJetNConstituent = nullptr;
            vector<double> *SignalJetMatrixElem = nullptr;
            vector<double> *SignalJetMatrixElemDR = nullptr;
            vector<double> *SignalJetMatrixElemPt = nullptr;
            vector<double> *SignalJetMatrixElemPx = nullptr;
            vector<double> *SignalJetMatrixElemPy = nullptr;
            vector<double> *SignalJetMatrixElemMass = nullptr;
            vector<double> *SignalJetPt = nullptr;
            vector<double> *Weight = nullptr;
            vector<double> *SignalJetPhi = nullptr;
            vector<double> *SignalJetEta = nullptr;
            vector <double> *LeadingZPhi = nullptr;
            vector <double> *LeadingPhotonPhi = nullptr;
            vector<vector<double>> *SignalJetConstituentPt = nullptr;
            vector<vector<double>> *SignalJetConstituentPid = nullptr;

            Tree->SetBranchAddress(("SignalJet" + SoftDrop + "NConstituent").c_str(), &SignalJetNConstituent);
            Tree->SetBranchAddress("SignalJetMatrixElem", &SignalJetMatrixElem);
            Tree->SetBranchAddress("SignalJetMatrixElemDR", &SignalJetMatrixElemDR);
            Tree->SetBranchAddress("SignalJetMatrixElemPt", &SignalJetMatrixElemPt);
            Tree->SetBranchAddress("SignalJetMatrixElemPx", &SignalJetMatrixElemPx);
            Tree->SetBranchAddress("SignalJetMatrixElemPy", &SignalJetMatrixElemPy);
            Tree->SetBranchAddress("SignalJetMatrixElemMass", &SignalJetMatrixElemMass);
            Tree->SetBranchAddress("SignalJetPt", &SignalJetPt);
            Tree->SetBranchAddress("SignalJetPhi", &SignalJetPhi);
            Tree->SetBranchAddress("SignalJetEta", &SignalJetEta);
            Tree->SetBranchAddress("LeadingZPhi", &LeadingZPhi);
            Tree->SetBranchAddress("LeadingPhotonPhi", &LeadingPhotonPhi);
            Tree->SetBranchAddress("EventWeight", &Weight);
            Tree->SetBranchAddress("SignalJetConstituentPt", &SignalJetConstituentPt);
            Tree->SetBranchAddress("SignalJetConstituentPid", &SignalJetConstituentPid);

            int EntryCount = Tree->GetEntries();
            // cout << "Entry count... " << to_string(EntryCount) << endl;
            for(int iE = 0; iE < EntryCount; iE++)
            {
                Tree->GetEntry(iE);

                if(SignalJetPt == nullptr)
                    continue;

                int NJet = SignalJetPt->size();
                double MaxJetPT = 0; // to find max pt jet in event
                int MaxJetPTEventIndex = -1;
                double MaxJetPT2 = 0; // to find second max pt jet in event
                int MaxJetPTEventIndex2 = -1;

                for(int iJ = 0; iJ < NJet; iJ++)
                {
                    if((*SignalJetPt)[iJ] < MinPT || (*SignalJetPt)[iJ] > MaxPT) continue;
                    // if((*SignalJetPt)[iJ] < MinPT) continue;
                    if((*SignalJetMatrixElem)[iJ] <= 0) continue; // if the matrix element is undefined then continue
                    if(fabs((*SignalJetEta)[iJ]) < MinEta || fabs((*SignalJetEta)[iJ]) > MaxEta) continue;  // eta cut here
                    // if((*SignalJetMatrixElemDR)[iJ] > 0.4) continue;
                    if(zJet && !oppositeHemisphere((*LeadingZPhi)[0], (*SignalJetPhi)[iJ])) continue; // if its a z jet and not in the opposite hemisphere according to leading z phi
                    if(photonJet && !oppositeHemisphere((*LeadingPhotonPhi)[0], (*SignalJetPhi)[iJ])) continue; // same logic but photons

                    // now let's try to find the leading 2 jets! (we'll find leading 2 even tho there may just be 1.. or zjet/photon jet only needs 1)
                    if ((*SignalJetPt)[iJ] > MaxJetPT) {
                        // there's probably a more clever way of using arrays and shifting values but yolo life's too short
                        // if new pt is > max pt, then replace 2nd max with the og max, and make the current the max
                        MaxJetPTEventIndex2 = MaxJetPTEventIndex;
                        MaxJetPT2 = MaxJetPT;
                        MaxJetPTEventIndex = iJ;
                        MaxJetPT = (*SignalJetPt)[iJ];
                    } else if ((*SignalJetPt)[iJ] > MaxJetPT2) {
                        // ow if new pt > 2nd max, then just replace 2nd max
                        MaxJetPTEventIndex2 = iJ;
                        MaxJetPT2 = (*SignalJetPt)[iJ];
                    }

                    
                    int NParticles = (*SignalJetConstituentPt)[iJ].size();

                    for (int k = 0; k < NParticles; k++){
                        HJetPTTrack->Fill((*SignalJetConstituentPt)[iJ][k], (*Weight)[0]);
                    }

                    HJetN->Fill((*SignalJetNConstituent)[iJ], (*Weight)[0]);

                    // debugging purposes: find constituent multiplicity < x and highlight those events
                    // if ((*SignalJetNConstituent)[iJ] < 5) {
                    //     cout << "Entry " << iE << ", jet " << iJ << ", const mult " << (*SignalJetNConstituent)[iJ] << endl;
                    // }

                    HJetPT->Fill((*SignalJetPt)[iJ], (*Weight)[0]);
                    h2->Fill((*SignalJetNConstituent)[iJ], (*SignalJetPt)[iJ]);

                    jetCount = jetCount + (*Weight)[0];
                }

                // now put these into the appropriate plots
                // for dijet/zjet we just want leading jet
                if (MaxJetPTEventIndex >=0) {
                    // get the largest photon pt fraction and cut if larger than 0.8
                    int NParticles = (*SignalJetConstituentPid)[MaxJetPTEventIndex].size();
                    double MaxPhotonPT = 0;
                    double PT;
                    for (int iP = 0; iP < NParticles; iP++) {
                        if ((*SignalJetConstituentPid)[MaxJetPTEventIndex][iP] == 22) {
                            PT = (*SignalJetConstituentPt)[MaxJetPTEventIndex][iP];
                            if (PT > MaxPhotonPT) {
                                MaxPhotonPT = PT;
                            }
                        }
                    }
                    double largestPhotonPtFrac = MaxPhotonPT / MaxJetPT;
                    if (largestPhotonPtFrac > 0.8) continue;

                    // if the sample survives then fill in plots and stuff
                    HJetNLeading->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
                    hMatElem->Fill((*SignalJetMatrixElemPt)[MaxJetPTEventIndex], (*SignalJetNConstituent)[MaxJetPTEventIndex]);
                    if ((*SignalJetMatrixElem)[MaxJetPTEventIndex] == 21) {
                        // gluon-like
                        HJetNGluon->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
                        HJetDRGluon->Fill((*SignalJetMatrixElemDR)[MaxJetPTEventIndex], (*Weight)[0]);
                        HJetPtGluon->Fill((*SignalJetMatrixElemPt)[MaxJetPTEventIndex], (*Weight)[0]);
                        HJetPtGluon2->Fill((*SignalJetMatrixElemPt)[MaxJetPTEventIndex], (*Weight)[0]);
                        HJetMassGluon->Fill((*SignalJetMatrixElemMass)[MaxJetPTEventIndex], (*Weight)[0]);
                        if ((*SignalJetMatrixElemDR)[MaxJetPTEventIndex] <= 0.4) {
                            HJetNGluonTruth->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
                        }
                    } else {
                        // quark-like
                        HJetNQuark->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
                        HJetDRQuark->Fill((*SignalJetMatrixElemDR)[MaxJetPTEventIndex], (*Weight)[0]);
                        HJetPtQuark->Fill((*SignalJetMatrixElemPt)[MaxJetPTEventIndex], (*Weight)[0]);
                        HJetPtQuark2->Fill((*SignalJetMatrixElemPt)[MaxJetPTEventIndex], (*Weight)[0]);
                        HJetMassQuark->Fill((*SignalJetMatrixElemMass)[MaxJetPTEventIndex], (*Weight)[0]);
                        if ((*SignalJetMatrixElemDR)[MaxJetPTEventIndex] <= 0.4) {
                            HJetNQuarkTruth->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
                        }
                    }
                    if (dijet && MaxJetPTEventIndex2 >= 0) {
                        HJetNLeading->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
                        if ((*SignalJetMatrixElem)[MaxJetPTEventIndex2] == 21) {
                            // gluon-like
                            HJetNGluon->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
                            HJetDRGluon->Fill((*SignalJetMatrixElemDR)[MaxJetPTEventIndex2], (*Weight)[0]);
                            HJetPtGluon->Fill((*SignalJetMatrixElemPt)[MaxJetPTEventIndex2], (*Weight)[0]);
                            HJetMassGluon->Fill((*SignalJetMatrixElemMass)[MaxJetPTEventIndex2], (*Weight)[0]);
                            if ((*SignalJetMatrixElemDR)[MaxJetPTEventIndex2] <= 0.4) {
                                HJetNGluonTruth->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
                            }
                        } else {
                            // quark-like
                            HJetNQuark->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
                            HJetDRQuark->Fill((*SignalJetMatrixElemDR)[MaxJetPTEventIndex2], (*Weight)[0]);
                            HJetPtQuark->Fill((*SignalJetMatrixElemPt)[MaxJetPTEventIndex2], (*Weight)[0]);
                            HJetMassQuark->Fill((*SignalJetMatrixElemMass)[MaxJetPTEventIndex2], (*Weight)[0]);
                            if ((*SignalJetMatrixElemDR)[MaxJetPTEventIndex2] <= 0.4) {
                                HJetNQuarkTruth->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
                            }
                        }
                    }
                }
            }
            File.Close();
        }

        cout << "total files looked at: " << fileCount << endl;
        
        std::vector<double> v, v_error, v_quark, v_quark_error, v_gluon, v_gluon_error, v_quark_truth, v_quark_truth_error, v_gluon_truth, v_gluon_truth_error;
        for (int i = 0; i < HJetNLeading->GetNbinsX(); i++) {
            v.push_back(HJetNLeading->GetBinContent(i + 1)); 
            v_error.push_back(HJetNLeading->GetBinError(i + 1));
            v_quark.push_back(HJetNQuark->GetBinContent(i + 1));
            v_quark_error.push_back(HJetNQuark->GetBinError(i + 1));
            v_gluon.push_back(HJetNGluon->GetBinContent(i + 1));
            v_gluon_error.push_back(HJetNGluon->GetBinError(i + 1));
            v_quark_truth.push_back(HJetNQuarkTruth->GetBinContent(i+1));
            v_quark_truth_error.push_back(HJetNQuarkTruth->GetBinError(i+1));
            v_gluon_truth.push_back(HJetNGluonTruth->GetBinContent(i+1));
            v_gluon_truth_error.push_back(HJetNGluonTruth->GetBinError(i+1));
        }

        // put csv lines in csv
        // copy(v.begin(), v.end(), output_iterator);
        outFile << label << ",";
        for (const auto &e : v) outFile << e << ",";
        outFile << endl;
        outFile << label << "_error" << ",";
        for (const auto &e : v_error) outFile << e << ",";
        outFile << endl;
        outFile << label << "_quark" << ",";
        for (const auto &e : v_quark) outFile << e << ",";
        outFile << endl;
        outFile << label << "_quark_error" << ",";
        for (const auto &e : v_quark_error) outFile << e << ",";
        outFile << endl;
        outFile << label << "_gluon" << ",";
        for (const auto &e : v_gluon) outFile << e << ",";
        outFile << endl;
        outFile << label << "_gluon_error" << ",";
        for (const auto &e : v_gluon_error) outFile << e << ",";
        outFile << endl;
        // if (photonJet) {
        outFile << label << "_quark_truth" << ",";
        for (const auto &e : v_quark_truth) outFile << e << ",";
        outFile << endl;
        outFile << label << "_quark_truth_error" << ",";
        for (const auto &e : v_quark_truth_error) outFile << e << ",";
        outFile << endl;
        outFile << label << "_gluon_truth" << ",";
        for (const auto &e : v_gluon_truth) outFile << e << ",";
        outFile << endl;
        outFile << label << "_gluon_truth_error" << ",";
        for (const auto &e : v_gluon_truth_error) outFile << e << ",";
        outFile << endl;
        // }

        // outFileMass << legendLabel << "," << numMassEmptyG << "," << numMassEmptyQ << endl;

        // TH1D* hists[3] = {HJetN, HJetPT, HJetNLeading};
        TH1D* hists[4] = {HJetN, HJetPT, HJetPTTrack, HJetNLeading};
        // TH1D* hists[NUM_GRAPHS] = {HJetN, HJetPT, HJetPTTrack, HJetNLeading, HJetNQuark, HJetNGluon};
        for (int i=0; i < 4; i++) {
            TH1D* h = hists[i];
            // gStyle->SetTextSize(0.045);
            // gStyle->SetTextFont(42);
            // gStyle->SetPadRightMargin(0.05);
            // gStyle->SetPadLeftMargin(0.15);
            // gStyle->SetPadTopMargin(0.15);
            // gStyle->SetPadBottomMargin(0.145);
            // gStyle->SetTitleX(0.5);
            
            h->GetYaxis()->SetTitleOffset(1.6);
            h->Sumw2();
            h->Scale(1./h->Integral());
            // h->Scale(1./h->GetEntries(), "width");
            // Double_t scale = h->GetXaxis()->GetBinWidth(1)/(h->GetEntries());
            // h->Scale(scale);
            h->SetMarkerStyle(20);
            h->SetMarkerColor(colorIdx);
            h->SetLineColor(colorIdx);
            h->SetStats(0);
            if (i == 0) {
                hNStack->Add(h);
                legend->AddEntry(h, legendLabel, "p"); // we only need to add this once
            } else if (i == 1) {
                hPTStack->Add(h);
            } else if (i == 2) {
                hPTTrackStack->Add(h);
            } else if (i == 3) {
                hNLeadingStack->Add(h);
            }
        }

        TCanvas* cqg = plotQGHistograms(HJetNQuark, HJetNGluon, hNQuarkGluonStack, false, colors, colorsLight);
        PdfFile.AddCanvas(cqg);
        TCanvas* cqgDR = plotQGHistograms(HJetDRQuark, HJetDRGluon, hDRStack, true, colors, colorsLight);
        PdfFile.AddCanvas(cqgDR);
        TCanvas* cqgPt = plotQGHistograms(HJetPtQuark, HJetPtGluon, hPtStack, false, colors, colorsLight);
        PdfFile.AddCanvas(cqgPt);
        TCanvas* cqgPt2 = plotQGHistograms(HJetPtQuark2, HJetPtGluon2, hPtStack2, false, colors, colorsLight);
        PdfFile.AddCanvas(cqgPt2);
        TCanvas* cqgMass = plotQGHistograms(HJetMassQuark, HJetMassGluon, hMassStack, false, colors, colorsLight);
        PdfFile.AddCanvas(cqgMass);

        TCanvas* cscatter = new TCanvas("cscatter", "", 1200, 900);
        // cscatter->SetLogy();
        h2->Scale(1./h2->GetEntries(), "width");
        h2->Draw("COLZ");
        cscatter->Update();
        // cscatter->SaveAs((TString) (outputLabel + label + "scatter.jpg"));
        PdfFile.AddCanvas(cscatter);

        TCanvas* cMatElem = new TCanvas("cMatElem", "", 1200, 900);
        hMatElem->Draw("COLZ");
        cMatElem->Update();
        PdfFile.AddCanvas(cMatElem);

        TCanvas* cMatElem2 = new TCanvas("cMatElem2", "", 1200, 900);
        hMatElem->Draw("LEGO1");
        cMatElem2->Update();
        PdfFile.AddCanvas(cMatElem2);

        TCanvas* cz = new TCanvas("cz", "", 1200, 900);
        HJetNLeading->Draw();
        // cz->SetLogy();
        // cz->SaveAs((TString) (outputLabel + label + "nconst_opphem.jpg"));
   }

    THStack* hStacks[4] = {hNStack, hPTStack, hNLeadingStack, hPTTrackStack};
    for (int i = 0; i < 4; i ++ ){
        TCanvas* c = new TCanvas("c","",1200,900);
        hStacks[i]->Draw("nostackb");
        legend->SetTextSize(0.028);
        legend->SetFillColor(0);
        legend->SetBorderSize(1);
        legend->Draw("");
        if (i == 1 || i == 3) c->SetLogy();
        c->Update();
        // c->SaveAs((TString) (outputLabel + to_string(i) + ".jpg"));
        PdfFile.AddCanvas(c);
    }

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




