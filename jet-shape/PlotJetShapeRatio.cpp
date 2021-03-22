#include <iostream>
#include <fstream>
#include <iterator>
#include <cmath>
#include <dirent.h>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "uti.h"
#include <math.h>

// g++ PlotJetShapeRatio.cpp $(root-config --cflags --libs) -O2 -o "plotJetShapeRatio.exe"
// ./plotJetShapeRatio.exe -input "/data/kying/EMMIResults/pp150,/data/kying/EMMIResults/PbPb150_0_10,/data/kying/EMMIResults/PbPbWide150_0_10"

// tracks are charged particles
// use particleseta, particlesphi, signaljeteta, signaljetphi
// dphi needs to be between -pi and pi
// jet shape = 1/dr 1/Njet sum[[pT_track for track in tracks if track in dr bucket] / pT_jet for jet in jets]
// in the example, they used dr = 0.05

// plot the sum pT distribution from tracks
// as a function of delta R with respect to the jet
// and delta R is defined as dR = sqrt(delta eta ^2 + delta phi ^2)
// delta eta = track eta - jet eta
// delta phi = track phi - jet phi
// (of course, need to be careful here about the delta phi calculation, don’t miss the factor of 2 pi here)
// we want the delta phi to be between -pi and pi
// runEmmi should be able to do it
// so jet shape is like
// the constituent momentum spectrum as a function of dR

double getR(double etaTrack, double etaJet, double phiTrack, double phiJet) {
    // this is the reconstructed track's radial distance from jet axis (defined by etaJet and phiJet)
    // tracks = charged particles
    double deta = etaTrack - etaJet;
    // signal jet phi: SignalJet04Phi --> same # entries (range from 0 to 2pi)
    // particles phi: ParticlesPhi --> same # of entries (range from 0 to 2pi)
    double dphi = phiTrack - phiJet;

    // cout << "dphi: " << dphi << endl;
    // dphi should be between -pi and pi
    if (dphi < -M_PI) {
        dphi = 2*M_PI - abs(dphi);  // if phi is < -pi, then 2pi - value of dphi is new
    } else if (dphi > M_PI) {
        dphi = - (2*M_PI - dphi); // if phi > pi, then 2pi - dphi is new value, but need to negate
    }

    assert(dphi >= -M_PI && dphi <= M_PI);

    return sqrt(pow(deta,2) + pow(dphi, 2));
}

tuple<string, string, string> getDataLabel(string fileName){
    string dataLabel, label, legendLabel;
    if (fileName.find("ZJet") != string::npos){
        dataLabel = "#splitline{Data: pyquen ppZJet150}{(pp z-jet)}";
        // dataLabel = " (pp ZJet) (Data: pyquen ppZJet150)";
        label = "pp150_1_zjet";
        legendLabel = "ppZJet150";
    } else if (fileName.find("PbPbWide") != string::npos){
        dataLabel = "#splitline{Data: pyquen PbPbWide150_0_10}{(PbPb wide dijet)}";
        // dataLabel = " (PbPb wide) (Data: pyquen PbPbWide150_0_10)";
        label = "pbpb150_0_10_1_wide";
        legendLabel = "PbPbWide150";
    } else if (fileName.find("PbPb") != string::npos){
        dataLabel = "#splitline{Data: pyquen PbPb150_0_10}{(PbPb dijet)}";
        // dataLabel = " (PbPb) (Data: pyquen PbPb150_0_10)";
        label = "pbpb150_0_10_1";
        legendLabel = "PbPb150";
    } else {
        dataLabel = "#splitline{Data: pyquen pp150}{(pp dijet)}";
        // dataLabel = " (pp Dijet) (Data: pyquen pp150)";
        label = "pp150_1";
        legendLabel = "pp150";
    }
    return make_tuple(dataLabel, label, legendLabel);
}

int main(int argc, char *argv[]) {
    SetThesisStyle();
    CommandLine CL(argc, argv);
    // string fileName = CL.Get("input");
    double trackPtCut = CL.GetDouble("trkpt", 1.0);
    double minPT = CL.GetDouble("minpt", 100);
    double dr = CL.GetDouble("dr", 0.05);  // bins are [0, 0.05), [0.05, 0.10), [0.10, 0.15), ... [0.25, 0.30)
    double maxR = CL.GetDouble("maxr", 0.30);  // max value of max on rightmost bin
    vector<string> InputFileFolders = CL.GetStringVector("input");

    // TMultiGraph* mg = new TMultiGraph();
    TCanvas* cn = new TCanvas("cn","cn",1200,900);
    TLegend* legend = new TLegend(0.7,0.8,0.95,0.9);
    // TLegend* legend = new TLegend(0.1, 0.1, 0.3, 0.3);

    int plotColor = 2;
    
    // double dr = 0.05;
    // Int_t n = 6;
    Int_t n = (int) ceil(maxR/dr);
    // cout << "n buckets: " << n << endl;
    Double_t x[n], y[3][n];
    Double_t ex[n], ey[3][n];

    // hard coded data from https://twiki.cern.ch/twiki/pub/CMSPublic/PhysicsResultsHIN12002/DataPoint.txt
    // centrality 0-10%
    double xData[6] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275};
    double yData[6] = {1.022, 0.959, 0.862, 0.960, 1.209, 1.357};
    double exData[6] = {0.025, 0.025, 0.025, 0.025, 0.025, 0.025};
    double eyData[6] = {0.004, 0.007, 0.009, 0.013, 0.022, 0.041};

    double yMin = INFINITY;
    double yMax = -INFINITY;

    for(int f=0; f < 3; f++) {  // TODO: hard coded that there would be 2 inputs
        string folder = InputFileFolders[f];
        string dataLabel, fileLabel, legendLabel;

        double r_a, r_b;
        double rho, count, totalyerr;
        for (int bin=0; bin < n; bin++){
            r_a = bin * dr;
            r_b = (bin + 1) * dr;
            // cout << "ra: " << r_a << ", rb: " << r_b << endl;

            rho = 0;
            count = 0;
            totalyerr = 0;

            struct dirent *de; // to get files in path
            DIR *dir = opendir(folder.c_str());
            if (dir == NULL)  // opendir returns NULL if couldn't open directory 
            { 
                cout << "Could not open directory: " << folder << endl;
                return 0; 
            } 

            while ((de = readdir(dir)) != NULL) {
                if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ) {continue;}
                string fileName = de->d_name;
                TFile File((folder + "/" + fileName).c_str());
                tie(dataLabel, fileLabel, legendLabel) = getDataLabel(fileName);

                TTree *tree = (TTree *)File.Get("JetTree");

                if(tree == nullptr)
                {
                    File.Close();
                    return 0;
                }

                // signal jet eta: SignalJet04Eta --> approx 3779 entries (range from -pi to pi)
                // signal jet phi: SignalJet04Phi --> same # entries (range from 0 to 2pi)
                // signal jet pt: SignalJet04Pt --> 3779 (range from 0 to approx 500?)
                // particles eta: ParticlesEta --> approx 249374 entries (looks like range from -9 to +9)
                // particles phi: ParticlesPhi --> same # of entries (range from 0 to 2pi)
                vector<double> *signalJetEta = nullptr;
                vector<double> *signalJetPhi = nullptr;
                vector<double> *signalJetPt = nullptr;
                vector<double> *weight = nullptr;
                vector<double> *particlesPt = nullptr;
                vector<double> *particlesEta = nullptr;
                vector<double> *particlesPhi = nullptr;

                tree->SetBranchAddress("SignalJet04Eta", &signalJetEta);
                tree->SetBranchAddress("SignalJet04Phi", &signalJetPhi);
                tree->SetBranchAddress("SignalJet04Pt", &signalJetPt);
                tree->SetBranchAddress("EventWeight", &weight);
                tree->SetBranchAddress("ParticlesPt", &particlesPt);
                tree->SetBranchAddress("ParticlesEta", &particlesEta);
                tree->SetBranchAddress("ParticlesPhi", &particlesPhi);

                // if we use h-> GetEntries(), this is number of entries = number of jets
                int entryCount = tree->GetEntries();  // N = N_event, this is per-event count
                // cout << "n entries in tree: " << entryCount << endl;

                // int particlesCount = 0;

                for(int i = 0; i < entryCount; i++)  // iterating through every entry in the tree
                {
                    tree->GetEntry(i);

                    if(signalJetPt == nullptr) {continue;}

                    int nJet = signalJetPt->size();
                    // cout << "jets: " << nJet << endl;
                    int nParticles = particlesEta->size();
                    // cout << "particles: " << nParticles << endl;
                    // particlesCount += nParticles;

                    for(int j = 0; j < nJet; j++) // iterating through each signal jet
                    {
                        if ((*signalJetPt)[j] < minPT) continue;
                        double yerr = 0;
                        double sumTrackPts = 0;
                        count += (*weight)[0];
                        
                        // now we need to iterate through each track (charged particle) in the signal jet
                        for (int k = 0; k < nParticles; k++){
                            if ((*particlesPt)[k] > trackPtCut && abs((*signalJetEta)[j]) > 0.3 && abs((*signalJetEta)[j]) < 2) { // only for particles > trackPtCut
                                double r = getR((*particlesEta)[k], (*signalJetEta)[j], (*particlesPhi)[k], (*signalJetPhi)[j] ); 

                                // check track is in [r_a, r_b)
                                if (r >= r_a && r < r_b) {
                                    // pT of track to var
                                    sumTrackPts += (*particlesPt)[k]; 
                                    yerr += pow((*particlesPt)[k] * (*weight)[0] / (*signalJetPt)[j], 2);
                                }
                            }
                        }
                        // divide summed pT of track by pT of jet and add to var
                        rho += sumTrackPts * (*weight)[0] / (*signalJetPt)[j];

                        totalyerr += yerr;
                    }
                }
                File.Close();
                // cout << "rho: " << rho << " count: " <<  count << endl;
            }
            rho *= 1/dr * 1/count;
            totalyerr = sqrt(totalyerr) * 1/dr * 1/count;
            cout << r_a + 0.5 * dr << " " << rho << endl;
            x[bin] = r_a + 0.5 * dr;
            ex[bin] = 0.5 * dr;
            y[f][bin] = rho;
            ey[f][bin] = totalyerr;
        }
    }

    Double_t ratio[n], ratioe[n], ratioWide[n];
    for(int i=0; i < n; i++) {
        ratio[i] = y[1][i] / y[0][i]; // pbpb / pp ratio
        ratioWide[i] = y[2][i] / y[0][i]; // pbpb wide / pp ratio
        // ratioe[i] = ey[1][i] + ey[0][i];
        ratioe[i] = 0;
        // cout << "ratio: " << ratio[i] << ", r error: " << ratioe[i] << endl;
    }

    TGraph* gr = new TGraphErrors(n, x, ratio, ex, ratioe);
    legend->AddEntry(gr, "Pyquen (PbPb / pp)", "p");
    // TGraph* gr = new TGraph(n, x, ratio);
    gr->SetTitle("Jet Shape Ratio;r;\\rho(r)^{PbPb}/\\rho(r)^{pp}");
    gr->SetMarkerSize(1.2);
    gr->SetMarkerColor(plotColor);
    gr->SetFillColor(plotColor);
    // gr->SetFillStyle(3001);
    gr->GetYaxis()->SetRangeUser(0.45, 1.55);
    // gr->GetYaxis()->SetMoreLogLabels();
    gr->Draw("2apz");

    plotColor += 1;

    gr = new TGraphErrors(n, x, ratioWide, ex, ratioe);
    legend->AddEntry(gr, "Pyquen (PbPb Wide / pp)", "p");
    // TGraph* gr = new TGraph(n, x, ratio);
    gr->SetTitle("Jet Shape Ratio;r;\\rho(r)^{PbPb}/\\rho(r)^{pp}");
    gr->SetMarkerSize(1.2);
    gr->SetMarkerColor(plotColor);
    gr->SetFillColor(plotColor);
    // gr->SetFillStyle(3001);
    gr->GetYaxis()->SetRangeUser(0.45, 1.55);
    // gr->GetYaxis()->SetMoreLogLabels();
    gr->Draw("2pz same");

    // plot data
    gr = new TGraphErrors(n, xData, yData, exData, ratioe);

    legend->AddEntry(gr, "Data", "p");

    gr->SetTitle("Jet Shape Ratio;r;\\rho(r)^{PbPb}/\\rho(r)^{pp}");
    gr->SetMarkerSize(1.2);
    gr->SetMarkerColor(38);
    gr->SetFillColor(38);
    // gr->SetFillStyle(3001);
    // gr->GetYaxis()->SetMoreLogLabels();
    
    // gr->GetYaxis()->SetRangeUser(yMin*0.8, yMax*1.3);
    gr->Draw("2pz same"); // ow draw on top
    cn->Update();

    TLine *line = new TLine(0, 1.0, maxR+0.025, 1.0);
    line->Draw();
    
    // gr->GetYaxis()->SetRangeUser(yMin*0.8, yMax*1.3);
    cn->Update();

    // draw text
    TLatex *text = new TLatex();
    text->SetTextSize(0.025);
    string latexText = "#splitline{p_{T}^{track} > " + to_string((int)trackPtCut) + "GeV, p_{T}^{jet} > " + to_string((int)minPT) + "GeV}{0.3 < | #eta^{jet} | < 2 }"; //0.3 < |\\eta^{jet}| < 2
    // string latexText = "#splitline{p_{T}^{track} > 1GeV, p_T^{jet} > 0GeV}{"+dataLabel+"}";
    // text->DrawLatexNDC(0.65, 0.82, latexText.c_str());
    text->DrawLatexNDC(0.15, 0.2, latexText.c_str());
    // mg->Draw("a");

    legend->SetTextSize(0.025);
    legend->SetFillColor(0);
    legend->SetBorderSize(1);
    legend->Draw("");

    // cn->SetLogy(1);
    cn->SetRightMargin(0.05);
    cn->Update();
    cn->SaveAs(("./jetShapePlots/pppbpbratio_withdata_jetpt" + to_string((int)minPT) + "_trackpt" + to_string((int)trackPtCut) + "_" + to_string(maxR) + "r_" + to_string(n) +"buckets.jpg").c_str());

    return 0;
}
