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
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/ppZJet150" -minpt 0 -output ./150_1_2.8.2021/150_1_zjet_thstack_pt0
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp150,/data/kying/jetTopicsData/PbPbWide150_0_10,/data/kying/jetTopicsData/PbPb150_0_10" -minpt 100 -output ./150_1_2.8.2021/150_1_dijet_thstack
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp150,/data/kying/jetTopicsData/PbPbWide150_0_10,/data/kying/jetTopicsData/PbPb150_0_10" -minpt 0 -output ./150_1_2.8.2021/150_1_dijet_thstack

// with thermal!!
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp150,/data/kying/jetTopicsData/PbPbWide150_0_10,/data/kying/jetTopicsData/PbPb150_0_10,/data/kying/jetTopicsData/pp150_7000thermal,/data/kying/jetTopicsData/PbPbWide150_0_10_7000thermal,/data/kying/jetTopicsData/PbPb150_0_10_7000thermal" -minpt 100 -output ./150_1_7000thermal/150_1_dijet_thstack
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp150,/data/kying/jetTopicsData/PbPbWide150_0_10,/data/kying/jetTopicsData/PbPb150_0_10,/data/kying/jetTopicsData/pp150_7000thermal,/data/kying/jetTopicsData/PbPbWide150_0_10_7000thermal,/data/kying/jetTopicsData/PbPb150_0_10_7000thermal" -minpt 0 -output ./150_1_7000thermal/150_1_dijet_thstack
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp150_7000thermal,/data/kying/jetTopicsData/PbPbWide150_0_10_7000thermal,/data/kying/jetTopicsData/PbPb150_0_10_7000thermal" -minpt 0 -output ./150_1_7000thermal/150_1_dijet_thstack

// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/ppZJet150,/data/kying/jetTopicsData/PbPbWideZJet150_0_10,/data/kying/jetTopicsData/ppPhotonJet150,/data/kying/jetTopicsData/PbPbWidePhotonJet150_0_10,/data/kying/jetTopicsData/pp150,/data/kying/jetTopicsData/PbPb150_0_10,/data/kying/jetTopicsData/PbPbWide150_0_10" -minpt 0 -output "./150_1_all/150_1_dijetzjetphotonjet"
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/ppPhotonJet150,/data/kying/jetTopicsData/PbPbWidePhotonJet150_0_10" -minpt 0 -output "./150_photon/150_1_photonjet"
// ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/test" -minpt 0 -output "./150_photon/150_1_photonjet" // photon jet
// run -input "/data/kying/jetTopicsData/ppZJet150" -minpt 0 -output "test"

tuple<string, string, TString, int> getDataLabel(string folder) {
    // given the folder, return how to modify the title, the label of the output file, and the label in the legend
    int colorIdx;
    string titleMod,label;
    TString legendLabel;
    if (folder.find("7000thermal") != string::npos) {
        if (folder.find("ppZJet") != string::npos){
            titleMod = " (pp z-jet with thermal) (Data: pyquen ppZJet150)";
            label = "pp150_zjet_thermal";
            legendLabel = "ppZJet150 w/ thermal";
            colorIdx = kGreen;
        } else if (folder.find("PbPbWideZJet") != string::npos) {
            titleMod = " (PbPb wide z-jet with thermal) (Data: pyquen PbPbWideZJet150)";
            label = "pbpb150_0_10_wide_zjet_thermal";
            legendLabel = "PbPbWideZJet150 w/ thermal";
            colorIdx = kCyan-3;
        } else if (folder.find("ppPhotonJet") != string::npos) {
            titleMod = " (pp photon jet with thermal) (Data: pyquen ppPhotonJet150)";
            label = "pp150_photonjet_thermal";
            legendLabel = "ppPhotonJet150 w/ thermal";
            colorIdx = kBlue;
        } else if (folder.find("PbPbWidePhotonJet") != string::npos) {
            titleMod = " (PbPb wide photon jet with thermal) (Data: pyquen PbPbWidePhotonJet150)";
            label = "pbpb150_0_10_wide_photonjet_thermal";
            legendLabel = "PbPbWidePhotonJet150 w/ thermal";
            colorIdx = kMagenta-7;
        } else if (folder.find("PbPbWide") != string::npos){
            titleMod = " (PbPb wide with thermal) (Data: pyquen PbPbWide150_0_10)";
            label = "pbpb150_0_10_wide_thermal";
            legendLabel = "PbPbWide150 w/ thermal";
            colorIdx = kRed;
        } else if (folder.find("PbPb") != string::npos){
            titleMod = " (PbPb with thermal) (Data: pyquen PbPb150_0_10)";
            label = "pbpb150_0_10_thermal";
            legendLabel = "PbPb150 w/ thermal";
            colorIdx = kOrange+1;
        } else if (folder.find("pp") != string::npos){
            titleMod = " (pp dijet with thermal) (Data: pyquen pp150)";
            label = "pp150_thermal";
            legendLabel = "pp150 w/ thermal";
            colorIdx = kViolet-6;
        }
    } else {
        if (folder.find("ppZJet") != string::npos){
            titleMod = " (pp z-jet) (Data: pyquen ppZJet150)";
            label = "pp150_zjet";
            legendLabel = "ppZJet150";
            colorIdx = kGreen;
        } else if (folder.find("PbPbWideZJet") != string::npos) {
            titleMod = " (PbPb wide z-jet) (Data: pyquen PbPbWideZJet150)";
            label = "pbpb150_0_10_wide_zjet";
            legendLabel = "PbPbWideZJet150";
            colorIdx = kCyan-3;
        } else if (folder.find("ppPhotonJet") != string::npos) {
            titleMod = " (pp photon jet) (Data: pyquen ppPhotonJet150)";
            label = "pp150_photonjet";
            legendLabel = "ppPhotonJet150";
            colorIdx = kBlue;
        } else if (folder.find("PbPbWidePhotonJet") != string::npos) {
            titleMod = " (PbPb wide photon jet) (Data: pyquen PbPbWidePhotonJet150)";
            label = "pbpb150_0_10_wide_photonjet";
            legendLabel = "PbPbWidePhotonJet150";
            colorIdx = kMagenta-7;
        } else if (folder.find("PbPbWide") != string::npos){
            titleMod = " (PbPb wide) (Data: pyquen PbPbWide150_0_10)";
            label = "pbpb150_0_10_wide";
            legendLabel = "PbPbWide150";
            colorIdx = kRed;
        } else if (folder.find("PbPb") != string::npos){
            titleMod = " (PbPb) (Data: pyquen PbPb150_0_10)";
            label = "pbpb150_0_10";
            legendLabel = "PbPb150";
            colorIdx = kOrange+1;
        } else if (folder.find("pp") != string::npos){
            titleMod = " (pp dijet) (Data: pyquen pp150)";
            label = "pp150";
            legendLabel = "pp150";
            colorIdx = kViolet-6;
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

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileFolders = CL.GetStringVector("input");
   string OutputBase = CL.Get("output");
   double MinPT = CL.GetDouble("minpt", 200);
   string Title = CL.Get("title", "");
   string SoftDrop = CL.Get("sd", ""); // ex: -sd SD1
   double MinTrackPT = CL.GetDouble("mintrkpt", 0);

   string outputLabel = OutputBase + "_pt" + to_string((int) MinPT) + "_trkpt" + to_string((int)MinTrackPT) + "_" + SoftDrop;

   PdfFileHelper PdfFile(outputLabel + ".pdf");
   PdfFile.AddTextPage("Jet Topics Data Plots (pT > " + to_string((int) MinPT) + " GeV)");
   vector<string> lines;
   lines.push_back("Pyquen data source location:");
   lines.push_back("/data/yjlee/pyquen/pp150/pp_*.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPb150_0_10/PbPb_0_10_*.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPbWide150_0_10/PbPbWide_0_10_*.pu14");

   lines.push_back("/data/yjlee/pyquen/ppPhotonJet150/pp_*.pu14");
   lines.push_back("/data/yjlee/pyquen/ppZJet150/pp_*.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPbWidePhotonJet150_0_10/PbPbWide_0_10_*.pu14");
   lines.push_back("/data/yjlee/pyquen/PbPbWideZJet150_0_10/PbPbWide_0_10_*.pu14");

   lines.push_back("Thermal pileup location:");
   lines.push_back("/data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz");
   PdfFile.AddTextPage(lines, 0.1, 0.9, 0.03);


    // stuff for the output csv
    ofstream outFile(outputLabel + "histogram.csv", ios_base::app);
    ostream_iterator<std::string> output_iterator(outFile, "\n");

    THStack* hNStack = new THStack("hNStack",(SoftDrop + " Constituent Multiplicity (pyquen samples);" + SoftDrop + " Constituent Multiplicity;1/N dN/d " + SoftDrop + " Constituent Multiplicity").c_str());
    THStack* hPTStack = new THStack("hPTStack","pT (pyquen samples);pT;1/N dN/dpT");
    THStack* hNLeadingStack = new THStack("hNLeadingStack","NConstituent (leading jet(s));Constitutent Multiplicity;1/N dN");
    // THStack* hNQuarkStack = new THStack("hNQuarkStack","NConstituent (leading jet(s), quark-like);Constitutent Multiplicity;1/N dN");
    // THStack* hNGluonStack = new THStack("hNGluonStack","NConstituent (leading jet(s), gluon-like);Constitutent Multiplicity;1/N dN");
    THStack* hPTTrackStack = new THStack("hPTTrackStack","Particle pT (pyquen samples);pT;1/N dN/dpT");
    TLegend* legend = new TLegend(0.6,0.7,0.97,0.92);
    // TCanvas* cn = new TCanvas("cn","",1200,900);
    // TCanvas* cpt = new TCanvas("cpt","",1200,900);

    double jetCount = 0;

    const int NUM_GRAPHS = 6;
    
   for(string folder : InputFileFolders)
   {
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

        tie(titleMod, label, legendLabel, colorIdx) = getDataLabel(folder);
        
        cout << "Label of folder: " << legendLabel << endl;

        THStack* hNQuarkGluonStack = new THStack("hNQuarkGluonStack","NConstituent (leading jet(s));Constitutent Multiplicity;1/N dN");
        TLegend* qglegend = new TLegend(0.6,0.7,0.97,0.92);
        
        TH1D* HJetN = new TH1D("HJetN", (SoftDrop + " Constituent Multiplicity" + titleMod + ";" + SoftDrop + " Constitutent Multiplicity;1/N dN").c_str(), 100, 0, 100);
        TH1D* HJetPTTrack = new TH1D("HJetPTTrack", ("Particle PT" + titleMod + ";pT;1/N dN/dpT").c_str(), 200, 0, 200);
        TH1D* HJetNLeading = new TH1D("HJetNLeading", ("NConstituent (leading jet(s))" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 100, 0, 100);
        TH1D* HJetNQuark = new TH1D("HJetNQuark", ("NConstituent (leading jet(s), quark-like)" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 100, 0, 100);
        TH1D* HJetNGluon = new TH1D("HJetNGluon", ("NConstituent (leading jet(s), gluon-like)" + titleMod + ";Constitutent Multiplicity;1/N dN").c_str(), 100, 0, 100);
        TH1D* HJetPT = new TH1D("HJetPT", ("Jet PT" + titleMod + ";pT;1/N dN/dpT").c_str(), 100, MinPT, 500);
        TH2D * h2 = new TH2D("h2", ("pT vs " + SoftDrop + " Constituent Multiplicity" + titleMod + ";" + SoftDrop + " Constituent Multiplicity;pT").c_str(), 120, 0, 120, 100, MinPT, 500);

        int fileCount = 0; // REMOVE
        while ((de = readdir(dr)) != NULL) {
            if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ) continue;
            // if (fileCount > 1) continue; // REMOVE
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
            vector<double> *SignalJetPt = nullptr;
            vector<double> *Weight = nullptr;
            vector<double> *SignalJetPhi = nullptr;
            vector <double> *LeadingZPhi = nullptr;
            vector <double> *LeadingPhotonPhi = nullptr;
            vector<vector<double>> *SignalJetConstituentPt = nullptr;

            Tree->SetBranchAddress(("SignalJet" + SoftDrop + "NConstituent").c_str(), &SignalJetNConstituent);
            Tree->SetBranchAddress("SignalJetMatrixElem", &SignalJetMatrixElem);
            Tree->SetBranchAddress("SignalJetPt", &SignalJetPt);
            Tree->SetBranchAddress("SignalJetPhi", &SignalJetPhi);
            Tree->SetBranchAddress("LeadingZPhi", &LeadingZPhi);
            Tree->SetBranchAddress("LeadingPhotonPhi", &LeadingPhotonPhi);
            Tree->SetBranchAddress("EventWeight", &Weight);
            Tree->SetBranchAddress("SignalJetConstituentPt", &SignalJetConstituentPt);

            int EntryCount = Tree->GetEntries();
            for(int iE = 0; iE < EntryCount; iE++)
            {
                Tree->GetEntry(iE);

                if(SignalJetPt == nullptr)
                    continue;

                int NJet = SignalJetPt->size();
                double MaxPT = 0; // to find max pt jet in event
                int MaxPTEventIndex = -1;
                double MaxPT2 = 0; // to find second max pt jet in event
                int MaxPTEventIndex2 = -1;

                for(int iJ = 0; iJ < NJet; iJ++)
                {
                    if((*SignalJetPt)[iJ] < MinPT) continue;
                    if((*SignalJetMatrixElem)[iJ] == -1) continue; // if the matrix element is undefined then continue
                    if(zJet && !oppositeHemisphere((*LeadingZPhi)[0], (*SignalJetPhi)[iJ])) continue; // if its a z jet and not in the opposite hemisphere according to leading z phi
                    if(photonJet && !oppositeHemisphere((*LeadingPhotonPhi)[0], (*SignalJetPhi)[iJ])) continue; // same logic but photons

                    // now let's try to find the leading 2 jets! (we'll find leading 2 even tho there may just be 1.. or zjet/photon jet only needs 1)
                    if ((*SignalJetPt)[iJ] > MaxPT) {
                        // there's probably a more clever way of using arrays and shifting values but yolo life's too short
                        // if new pt is > max pt, then replace 2nd max with the og max, and make the current the max
                        MaxPTEventIndex2 = MaxPTEventIndex;
                        MaxPT2 = MaxPT;
                        MaxPTEventIndex = iJ;
                        MaxPT = (*SignalJetPt)[iJ];
                    } else if ((*SignalJetPt)[iJ] > MaxPT2) {
                        // ow if new pt > 2nd max, then just replace 2nd max
                        MaxPTEventIndex2 = iJ;
                        MaxPT2 = (*SignalJetPt)[iJ];
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
                if (MaxPTEventIndex >=0) {
                    HJetNLeading->Fill((*SignalJetNConstituent)[MaxPTEventIndex], (*Weight)[0]);
                    if ((*SignalJetMatrixElem)[MaxPTEventIndex] == 2) {
                        // quark-like
                        HJetNQuark->Fill((*SignalJetNConstituent)[MaxPTEventIndex], (*Weight)[0]);
                    } else {
                        // gluon-like
                        HJetNGluon->Fill((*SignalJetNConstituent)[MaxPTEventIndex], (*Weight)[0]);
                    }
                    if (dijet && MaxPTEventIndex2 >= 0) {
                        HJetNLeading->Fill((*SignalJetNConstituent)[MaxPTEventIndex2], (*Weight)[0]);
                        if ((*SignalJetMatrixElem)[MaxPTEventIndex2] == 2) {
                            // quark-like
                            HJetNQuark->Fill((*SignalJetNConstituent)[MaxPTEventIndex2], (*Weight)[0]);
                        } else {
                            // gluon-like
                            HJetNGluon->Fill((*SignalJetNConstituent)[MaxPTEventIndex2], (*Weight)[0]);
                        }
                    }
                }
            }
            File.Close();
        }

        cout << "total files looked at: " << fileCount << endl;
        
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

        // TH1D* hists[3] = {HJetN, HJetPT, HJetNLeading};
        // TH1D* hists[4] = {HJetN, HJetPT, HJetPTTrack, HJetNLeading};
        TH1D* hists[NUM_GRAPHS] = {HJetN, HJetPT, HJetPTTrack, HJetNLeading, HJetNQuark, HJetNGluon};
        for (int i=0; i < NUM_GRAPHS; i++) {
            TH1D* h = hists[i];
            gStyle->SetTextSize(0.045);
            gStyle->SetTextFont(42);
            gStyle->SetPadRightMargin(0.05);
            gStyle->SetPadLeftMargin(0.15);
            gStyle->SetPadTopMargin(0.15);
            gStyle->SetPadBottomMargin(0.145);
            gStyle->SetTitleX(0.5);
            
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
            } else if (i == 2) {
                hPTTrackStack->Add(h);
            } else if (i == 3) {
                hNLeadingStack->Add(h);
            }
        }

        // qglegend
        hNQuarkGluonStack->Add(HJetNQuark);
        qglegend->AddEntry(HJetNQuark, "Quark-like", "p"); // we only need to add this once
        hNQuarkGluonStack->Add(HJetNGluon);
        qglegend->AddEntry(HJetNGluon, "Gluon-like", "p"); // we only need to add this once

        TCanvas* cqg = new TCanvas("cqg","",1200,900);
        hNQuarkGluonStack->Draw("nostackb");
        qglegend->SetTextSize(0.028);
        qglegend->SetFillColor(0);
        qglegend->SetBorderSize(1);
        qglegend->Draw("");
        cqg->SetLogy();
        cqg->Update();
        // c->SaveAs((TString) (outputLabel + to_string(i) + ".jpg"));
        PdfFile.AddCanvas(cqg);

        TCanvas* cscatter = new TCanvas("cscatter", "", 1200, 900);

        // cscatter->SetLogy();
        h2->Scale(1./h2->GetEntries(), "width");
        h2->Draw("COLZ");
        cscatter->Update();
        // cscatter->SaveAs((TString) (outputLabel + label + "scatter.jpg"));
        PdfFile.AddCanvas(cscatter);

        TCanvas* cz = new TCanvas("cz", "", 1200, 900);
        HJetNLeading->Draw();
        cz->SetLogy();
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
        c->SetLogy();
        c->Update();
        // c->SaveAs((TString) (outputLabel + to_string(i) + ".jpg"));
        PdfFile.AddCanvas(c);
    }

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




