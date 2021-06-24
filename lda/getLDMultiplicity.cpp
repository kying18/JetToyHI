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
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodLD.h"
#include "TMVA/DataSetInfo.h"
#include "TMVA/Factory.h"


// using namespace TMVA;


void getLDMultiplicityVector(int maxTrackPt, vector<double>& jetConstituentPt, ofstream outFileQuark, ofstream outFileGluon, int matrixElem) {
    vector<int> v, vError;
    TH1D* LDHist = new TH1D("LDHist", ("Track pT Multiplicity Histogram;Track pT;N").c_str(), maxTrackPt, 0, maxTrackPt); 
    int nParticles = jetConstituentPt.size(); // todo might have & * error here
    for (int i=0; i < nParticles; i++) {
        // put track pt into histogram
        LDHist->Fill(jetConstituentPt[i]); // track pt, weight is 1
    }
    for (int i=0; i < LDHist->GetNbinsX(); i++) {
        v.push_back(LDHist->GetBinContent(i+1));
        vError.push_back(LDHist->GetBinError(i+1));
    }
    
    if (matrixElem == 21) {
        for (const auto &e : v) outFileGluon << e << " ";
        outFileGluon << endl;
    } else {
        for (const auto &e : v) outFileQuark << e << " ";
        outFileQuark << endl;
    }

}


int main(int argc, char *argv[])
{
    SetThesisStyle();
    CommandLine CL(argc, argv);

    vector<string> InputFileFolders = CL.GetStringVector("input");
    string OutputBase = CL.Get("output");
    
    double MinPT = CL.GetDouble("minpt", 0);
    double MaxPT = CL.GetDouble("maxpt", 500);
    double MaxTrackPT = CL.GetInt("maxtrkpt", 0); // this needs to be int like 200 or something

    string outputLabel = OutputBase + "pt" + to_string((int) MinPT) + to_string((int) MaxPT) + "_maxtrkpt"+ to_string((int) MaxTrackPT);

    ofstream outFileQuark(outputLabel + "_quark.txt", ios_base::app);
    ofstream outFileGluon(outputLabel + "_gluon.txt", ios_base::app);

    for(string folder : inputFileFolders)
    {
        struct dirent *de; // to get files in path
        DIR *dr = opendir(folder.c_str()); 
        if (dr == NULL)  // opendir returns NULL if couldn't open directory 
        {
            cout << "Could not open directory: " << folder << endl;
            return 0;
        }

        int fileCount = 0; // REMOVE
        while ((de = readdir(dr)) != NULL) {
            if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ) continue;
            if (fileCount >= 20) continue; // REMOVE
            string fileName = de->d_name;
            TFile File((folder + "/" + fileName).c_str());

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
                    // if((*SignalJetPt)[iJ] < MinPT) continue;
                    if((*SignalJetMatrixElem)[iJ] <= 0) continue; // if the matrix element is undefined then continue
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
                }
            
                if (MaxJetPTEventIndex >=0 && MaxJetPT >= MinPT && MaxJetPT <= MaxPT) {
                    getLDMultiplicityVector(MaxTrackPT, (*SignalJetConstituentPt)[MaxJetPTEventIndex], outFileQuark, outFileGluon, (*SignalJetMatrixElem)[MaxJetPTEventIndex]);
                }
                if (dijet && MaxJetPTEventIndex2 >=0 && MaxJetPT2 >= MinPT && MaxJetPT2 <= MaxPT) {
                    getLDMultiplicityVector(MaxTrackPT, (*SignalJetConstituentPt)[MaxJetPTEventIndex2], outFileQuark, outFileGluon, (*SignalJetMatrixElem)[MaxJetPTEventIndex2]);
                }
            }

                    
            // int NParticles = (*SignalJetConstituentPt)[iJ].size();
        }
    }

    // // setting up
    // TMVA::DataSetInfo *dsi = new TMVA::DataSetInfo();
    // TString jobName = "temp job";
    // TString methodTitle = "temp title";
    // TMVA::MethodLD *LDA = new TMVA::MethodLD(jobName, methodTitle, *dsi);

    return 0;
}

// g++ getLDMultiplicity.cpp $(root-config --cflags --libs) -O2 -o "getLDMultiplicity.exe"
// ./getJetTopicHistsCsv.exe -input "/data/kying/final80/ppPhotonJet80,/data/kying/final80/pp80,/data/kying/final80/PbPbWidePhotonJet80_0_10,/data/kying/final80/PbPbWide80_0_10" -minpt 80 -maxpt 100 -output ./final80_etabuckets/

// the difference between this version and the previous is that this one will be used for csv generation, and we won't do much plotting here :)

// tuple<string, string, TString> getDataLabel(string folder) {
//     // given the folder, return how to modify the title, the label of the output file, and the label in the legend
//     string titleMod,label;
//     TString legendLabel;
//     if (folder.find("7000thermal") != string::npos) {
//         if (folder.find("ppZJet") != string::npos){
//             titleMod = " (pp z-jet with thermal) (Data: pyquen ppZJet80)";
//             label = "pp80_zjet_thermal";
//             legendLabel = "ppZJet80 w/ thermal";
//         } else if (folder.find("PbPbWideZJet") != string::npos) {
//             titleMod = " (PbPb wide z-jet with thermal) (Data: pyquen PbPbWideZJet80)";
//             label = "pbpb80_0_10_wide_zjet_thermal";
//             legendLabel = "PbPbWideZJet80 w/ thermal";
//         } else if (folder.find("ppPhotonJet") != string::npos) {
//             titleMod = " (pp photon jet with thermal) (Data: pyquen ppPhotonJet80)";
//             label = "pp80_photonjet_thermal";
//             legendLabel = "ppPhotonJet80 w/ thermal";
//         } else if (folder.find("PbPbWidePhotonJet") != string::npos) {
//             titleMod = " (PbPb wide photon jet with thermal) (Data: pyquen PbPbWidePhotonJet80)";
//             label = "pbpb80_0_10_wide_photonjet_thermal";
//             legendLabel = "PbPbWidePhotonJet80 w/ thermal";
//         } else if (folder.find("PbPbWide") != string::npos){
//             titleMod = " (PbPb wide with thermal) (Data: pyquen PbPbWide80_0_10)";
//             label = "pbpb80_0_10_wide_thermal";
//             legendLabel = "PbPbWide80 w/ thermal";
//         } else if (folder.find("PbPb") != string::npos){
//             titleMod = " (PbPb with thermal) (Data: pyquen PbPb80_0_10)";
//             label = "pbpb80_0_10_thermal";
//             legendLabel = "PbPb80 w/ thermal";
//         } else if (folder.find("pp") != string::npos){
//             titleMod = " (pp dijet with thermal) (Data: pyquen pp80)";
//             label = "pp80_thermal";
//             legendLabel = "pp80 w/ thermal";
//         }
//     } else {
//         if (folder.find("ppZJet") != string::npos){
//             titleMod = " (pp z-jet) (Data: pyquen ppZJet80)";
//             label = "pp80_zjet";
//             legendLabel = "ppZJet80";
//         } else if (folder.find("PbPbWideZJet") != string::npos) {
//             titleMod = " (PbPb wide z-jet) (Data: pyquen PbPbWideZJet80)";
//             label = "pbpb80_0_10_wide_zjet";
//             legendLabel = "PbPbWideZJet80";
//         } else if (folder.find("ppPhotonJet") != string::npos) {
//             titleMod = " (pp photon jet) (Data: pyquen ppPhotonJet80)";
//             label = "pp80_photonjet";
//             legendLabel = "ppPhotonJet80";
//         } else if (folder.find("PbPbWidePhotonJet") != string::npos) {
//             titleMod = " (PbPb wide photon jet) (Data: pyquen PbPbWidePhotonJet80)";
//             label = "pbpb80_0_10_wide_photonjet";
//             legendLabel = "PbPbWidePhotonJet80";
//         } else if (folder.find("PbPbWide") != string::npos){
//             titleMod = " (PbPb wide) (Data: pyquen PbPbWide80_0_10)";
//             label = "pbpb80_0_10_wide";
//             legendLabel = "PbPbWide80";
//         } else if (folder.find("PbPb") != string::npos){
//             titleMod = " (PbPb) (Data: pyquen PbPb80_0_10)";
//             label = "pbpb80_0_10";
//             legendLabel = "PbPb80";
//         } else if (folder.find("pp") != string::npos){
//             titleMod = " (pp dijet) (Data: pyquen pp80)";
//             label = "pp80";
//             legendLabel = "pp80";
//         }
//     }

//     return make_tuple(titleMod, label, legendLabel);
// }

// bool oppositeHemisphere(double particlePhi, double jetPhi) {
//     if(particlePhi >= 0) {
//         double dPhi = particlePhi - jetPhi;
//         if(dPhi < -M_PI)   dPhi = dPhi + 2 * M_PI;
//         if(dPhi > +M_PI)   dPhi = dPhi - 2 * M_PI;

//         if(dPhi < - M_PI / 2 || dPhi > M_PI / 2) {
//             return true;
//         }
//         return false;
//     }
//     // todo: what if there is no leading Z?
//     return false; // false for now.. don't count it
// }


// int main(int argc, char *argv[])
// {
//    SetThesisStyle();

//    CommandLine CL(argc, argv);

//    vector<string> InputFileFolders = CL.GetStringVector("input");
//    string OutputBase = CL.Get("output");
   
//    double MinPT = CL.GetDouble("minpt", 0);
//    double MaxPT = CL.GetDouble("maxpt", 500);
//    string Title = CL.Get("title", "");

//    string outputLabel = OutputBase + "pt" + to_string((int) MinPT) + to_string((int) MaxPT);

//     // stuff for the output csv
//     ofstream outFile(outputLabel + "histogram.csv", ios_base::app);

//     TLegend* legend = new TLegend(0.6,0.7,0.97,0.92);

//     double jetCount = 0;
    
//    for(string folder : InputFileFolders)
//    {
//         bool zJet = false;
//         bool photonJet = false;
//         bool dijet = false;

//         if (folder.find("ZJet") != string::npos){
//             zJet = true;
//         } else if (folder.find("PhotonJet") != string::npos){
//             photonJet = true;
//         } else {
//             dijet = true;
//         }

//         // some tools to define the plots
//         string titleMod, label;
//         TString legendLabel;
//         int colorIdx;
        
//         struct dirent *de; // to get files in path
//         DIR *dr = opendir(folder.c_str()); 
//         if (dr == NULL)  // opendir returns NULL if couldn't open directory 
//         { 
//             cout << "Could not open directory: " << folder << endl;
//             return 0;
//         }

//         tie(titleMod, label, legendLabel) = getDataLabel(folder);
        
//         cout << "Label of folder: " << legendLabel << endl;

//         TH1D* hists [3]; // 3 histograms to take care of the eta cuts
//         TH1D* histsQuark [3];
//         TH1D* histsGluon [3];
//         TH1D* histsQuarkTruth [3];
//         TH1D* histsGluonTruth [3];

//         for (int i=0; i < 3; i++) {
//             hists[i] = new TH1D("HJetN", ("Constituent Multiplicity" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 120, 0, 120); 
//             histsQuark[i] = new TH1D("HJetNQuark", ("NConstituent (leading jet(s), quark-like)" + titleMod + ";Constitutent Multiplicity;N").c_str(), 120, 0, 120);
//             histsGluon[i] = new TH1D("HJetNGluon", ("NConstituent (leading jet(s), gluon-like)" + titleMod + ";Constitutent Multiplicity;N").c_str(), 120, 0, 120);
//             histsQuarkTruth[i] = new TH1D("HJetNQuarkTruth", ("NConstituent (leading jet(s))" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 120, 0, 120);
//             histsGluonTruth[i] = new TH1D("HJetNGluonTruth", ("NConstituent (leading jet(s))" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 120, 0, 120);
//         }

//         int fileCount = 0; // REMOVE
//         while ((de = readdir(dr)) != NULL) {
//             if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ) continue;
//             // if (fileCount >= 20) continue; // REMOVE
//             string fileName = de->d_name;
//             TFile File((folder + "/" + fileName).c_str());

//             TTree *Tree = (TTree *)File.Get("JetTree");

//             if(Tree == nullptr)
//             {
//                 File.Close();
//                 continue;
//             }

//             fileCount += 1; // REMOVE

//             vector<double> *SignalJetNConstituent = nullptr;
//             vector<double> *SignalJetMatrixElem = nullptr;
//             vector<double> *SignalJetMatrixElemDR = nullptr;
//             vector<double> *SignalJetMatrixElemPt = nullptr;
//             vector<double> *SignalJetMatrixElemPx = nullptr;
//             vector<double> *SignalJetMatrixElemPy = nullptr;
//             vector<double> *SignalJetMatrixElemMass = nullptr;
//             vector<double> *SignalJetPt = nullptr;
//             vector<double> *Weight = nullptr;
//             vector<double> *SignalJetPhi = nullptr;
//             vector<double> *SignalJetEta = nullptr;
//             vector <double> *LeadingZPhi = nullptr;
//             vector <double> *LeadingPhotonPhi = nullptr;
//             vector<vector<double>> *SignalJetConstituentPt = nullptr;
//             vector<vector<double>> *SignalJetConstituentPid = nullptr;

//             Tree->SetBranchAddress("SignalJetNConstituent", &SignalJetNConstituent);
//             Tree->SetBranchAddress("SignalJetMatrixElem", &SignalJetMatrixElem);
//             Tree->SetBranchAddress("SignalJetMatrixElemDR", &SignalJetMatrixElemDR);
//             Tree->SetBranchAddress("SignalJetMatrixElemPt", &SignalJetMatrixElemPt);
//             Tree->SetBranchAddress("SignalJetMatrixElemPx", &SignalJetMatrixElemPx);
//             Tree->SetBranchAddress("SignalJetMatrixElemPy", &SignalJetMatrixElemPy);
//             Tree->SetBranchAddress("SignalJetMatrixElemMass", &SignalJetMatrixElemMass);
//             Tree->SetBranchAddress("SignalJetPt", &SignalJetPt);
//             Tree->SetBranchAddress("SignalJetPhi", &SignalJetPhi);
//             Tree->SetBranchAddress("SignalJetEta", &SignalJetEta);
//             Tree->SetBranchAddress("LeadingZPhi", &LeadingZPhi);
//             Tree->SetBranchAddress("LeadingPhotonPhi", &LeadingPhotonPhi);
//             Tree->SetBranchAddress("EventWeight", &Weight);
//             Tree->SetBranchAddress("SignalJetConstituentPt", &SignalJetConstituentPt);
//             Tree->SetBranchAddress("SignalJetConstituentPid", &SignalJetConstituentPid);

//             int EntryCount = Tree->GetEntries();
//             // cout << "Entry count... " << to_string(EntryCount) << endl;
//             for(int iE = 0; iE < EntryCount; iE++)
//             {
//                 Tree->GetEntry(iE);

//                 if(SignalJetPt == nullptr)
//                     continue;

//                 int NJet = SignalJetPt->size();
//                 double MaxJetPT = 0; // to find max pt jet in event
//                 int MaxJetPTEventIndex = -1;
//                 double MaxJetPT2 = 0; // to find second max pt jet in event
//                 int MaxJetPTEventIndex2 = -1;

//                 for(int iJ = 0; iJ < NJet; iJ++)
//                 {
//                     // if((*SignalJetPt)[iJ] < MinPT || (*SignalJetPt)[iJ] > MaxPT) continue;
//                     // if((*SignalJetPt)[iJ] < MinPT) continue;
//                     if((*SignalJetMatrixElem)[iJ] <= 0) continue; // if the matrix element is undefined then continue
//                     // if((*SignalJetMatrixElemDR)[iJ] > 0.4) continue;
//                     if(zJet && !oppositeHemisphere((*LeadingZPhi)[0], (*SignalJetPhi)[iJ])) continue; // if its a z jet and not in the opposite hemisphere according to leading z phi
//                     if(photonJet && !oppositeHemisphere((*LeadingPhotonPhi)[0], (*SignalJetPhi)[iJ])) continue; // same logic but photons

//                     // now let's try to find the leading 2 jets! (we'll find leading 2 even tho there may just be 1.. or zjet/photon jet only needs 1)
//                     if ((*SignalJetPt)[iJ] > MaxJetPT) {
//                         // there's probably a more clever way of using arrays and shifting values but yolo life's too short
//                         // if new pt is > max pt, then replace 2nd max with the og max, and make the current the max
//                         MaxJetPTEventIndex2 = MaxJetPTEventIndex;
//                         MaxJetPT2 = MaxJetPT;
//                         MaxJetPTEventIndex = iJ;
//                         MaxJetPT = (*SignalJetPt)[iJ];
//                     } else if ((*SignalJetPt)[iJ] > MaxJetPT2) {
//                         // ow if new pt > 2nd max, then just replace 2nd max
//                         MaxJetPTEventIndex2 = iJ;
//                         MaxJetPT2 = (*SignalJetPt)[iJ];
//                     }
//                     jetCount = jetCount + (*Weight)[0];
//                 }


//                 // now put these into the appropriate plots
//                 // for dijet/zjet we just want leading jet
//                 if (MaxJetPTEventIndex >=0 && MaxJetPT >= MinPT && MaxJetPT <= MaxPT) {
//                     // get the largest photon pt fraction and cut if larger than 0.8
//                     int NParticles = (*SignalJetConstituentPid)[MaxJetPTEventIndex].size();
//                     double MaxPhotonPT = 0;
//                     double PT;
//                     for (int iP = 0; iP < NParticles; iP++) {
//                         if ((*SignalJetConstituentPid)[MaxJetPTEventIndex][iP] == 22) {
//                             PT = (*SignalJetConstituentPt)[MaxJetPTEventIndex][iP];
//                             if (PT > MaxPhotonPT) {
//                                 MaxPhotonPT = PT;
//                             }
//                         }
//                     }
//                     double largestPhotonPtFrac = MaxPhotonPT / MaxJetPT;
//                     if (largestPhotonPtFrac > 0.8) continue;

//                     int histIdx;
//                     if (fabs((*SignalJetEta)[MaxJetPTEventIndex]) < 0.8) { histIdx = 0; }
//                     else if (fabs((*SignalJetEta)[MaxJetPTEventIndex]) < 1.6) {histIdx = 1;}
//                     else {histIdx = 2;}

//                     // if the sample survives then fill in plots and stuff
//                     hists[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
//                     if ((*SignalJetMatrixElem)[MaxJetPTEventIndex] == 21) {
//                         // gluon-like
//                         histsGluon[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
//                         if ((*SignalJetMatrixElemDR)[MaxJetPTEventIndex] <= 0.4) {
//                             histsGluonTruth[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
//                         }
//                     } else {
//                         // quark-like
//                         histsQuark[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
//                         if ((*SignalJetMatrixElemDR)[MaxJetPTEventIndex] <= 0.4) {
//                             histsQuarkTruth[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex], (*Weight)[0]);
//                         }
//                     }
//                     if (dijet && MaxJetPTEventIndex2 >= 0 && MaxJetPT2 >= MinPT && MaxJetPT2 <= MaxPT) {
//                         int histIdx;
//                         if (fabs((*SignalJetEta)[MaxJetPTEventIndex2]) < 0.8) { histIdx = 0; }
//                         else if (fabs((*SignalJetEta)[MaxJetPTEventIndex2]) < 1.6) {histIdx = 1;}
//                         else {histIdx = 2;}

//                         hists[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
//                         if ((*SignalJetMatrixElem)[MaxJetPTEventIndex2] == 21) {
//                             // gluon-like
//                             histsGluon[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
//                             if ((*SignalJetMatrixElemDR)[MaxJetPTEventIndex2] <= 0.4) {
//                                 histsGluonTruth[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
//                             }
//                         } else {
//                             // quark-like
//                             histsQuark[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
//                             if ((*SignalJetMatrixElemDR)[MaxJetPTEventIndex2] <= 0.4) {
//                                 histsQuarkTruth[histIdx]->Fill((*SignalJetNConstituent)[MaxJetPTEventIndex2], (*Weight)[0]);
//                             }
//                         }
//                     }
//                 }
//             }
//             File.Close();
//         }

//         cout << "total files looked at: " << fileCount << endl;
        
//         for (int etaIdx = 0; etaIdx < 3; etaIdx++) {
//             std::vector<double> v, v_error, v_quark, v_quark_error, v_gluon, v_gluon_error, v_quark_truth, v_quark_truth_error, v_gluon_truth, v_gluon_truth_error;
//             for (int i = 0; i < hists[0]->GetNbinsX(); i++) {
//                 v.push_back(hists[etaIdx]->GetBinContent(i + 1)); 
//                 v_error.push_back(hists[etaIdx]->GetBinError(i + 1));
//                 v_quark.push_back(histsQuark[etaIdx]->GetBinContent(i + 1));
//                 v_quark_error.push_back(histsQuark[etaIdx]->GetBinError(i + 1));
//                 v_gluon.push_back(histsGluon[etaIdx]->GetBinContent(i + 1));
//                 v_gluon_error.push_back(histsGluon[etaIdx]->GetBinError(i + 1));
//                 v_quark_truth.push_back(histsQuarkTruth[etaIdx]->GetBinContent(i+1));
//                 v_quark_truth_error.push_back(histsQuarkTruth[etaIdx]->GetBinError(i+1));
//                 v_gluon_truth.push_back(histsGluonTruth[etaIdx]->GetBinContent(i+1));
//                 v_gluon_truth_error.push_back(histsGluonTruth[etaIdx]->GetBinError(i+1));
//             }

//             // put csv lines in csv
//             outFile << label << "_eta" << etaIdx << ",";
//             for (const auto &e : v) outFile << e << ",";
//             outFile << endl;
//             outFile << label << "_eta" << etaIdx << "_error" << ",";
//             for (const auto &e : v_error) outFile << e << ",";
//             outFile << endl;
//             outFile << label << "_eta" << etaIdx << "_quark" << ",";
//             for (const auto &e : v_quark) outFile << e << ",";
//             outFile << endl;
//             outFile << label << "_eta" << etaIdx << "_quark_error" << ",";
//             for (const auto &e : v_quark_error) outFile << e << ",";
//             outFile << endl;
//             outFile << label << "_eta" << etaIdx << "_gluon" << ",";
//             for (const auto &e : v_gluon) outFile << e << ",";
//             outFile << endl;
//             outFile << label << "_eta" << etaIdx << "_gluon_error" << ",";
//             for (const auto &e : v_gluon_error) outFile << e << ",";
//             outFile << endl;
//             // if (photonJet) {
//             outFile << label << "_eta" << etaIdx << "_quark_truth" << ",";
//             for (const auto &e : v_quark_truth) outFile << e << ",";
//             outFile << endl;
//             outFile << label << "_eta" << etaIdx << "_quark_truth_error" << ",";
//             for (const auto &e : v_quark_truth_error) outFile << e << ",";
//             outFile << endl;
//             outFile << label << "_eta" << etaIdx << "_gluon_truth" << ",";
//             for (const auto &e : v_gluon_truth) outFile << e << ",";
//             outFile << endl;
//             outFile << label << "_eta" << etaIdx << "_gluon_truth_error" << ",";
//             for (const auto &e : v_gluon_truth_error) outFile << e << ",";
//             outFile << endl;
//         }
//     }

//    return 0;
// }




