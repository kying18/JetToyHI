#include <iostream>
#include <fstream>
#include <iterator>
#include <cmath>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "uti.h"
#include <math.h>

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

string getDataLabel(string fileName){
    string dataLabel;
    if (fileName.find("ZJet") != string::npos){
        dataLabel = "#splitline{Data: pyquen ppZJet150}{(pp z-jet)}";
        // dataLabel = " (pp ZJet) (Data: pyquen ppZJet150)";
        // label = "pp150_1_zjet";
        // legendLabel = "ppZJet150";
    } else if (fileName.find("PbPbWide") != string::npos){
        dataLabel = "#splitline{Data: pyquen PbPbWide150_0_10}{(PbPb wide dijet)}";
        // dataLabel = " (PbPb wide) (Data: pyquen PbPbWide150_0_10)";
        // label = "pbpb150_0_10_1_wide";
        // legendLabel = "PbPbWide150";
    } else if (fileName.find("PbPb") != string::npos){
        dataLabel = "#splitline{Data: pyquen PbPb150_0_10}{(PbPb dijet)}";
        // dataLabel = " (PbPb) (Data: pyquen PbPb150_0_10)";
        // label = "pbpb150_0_10_1";
        // legendLabel = "PbPb150";
    } else {
        dataLabel = "#splitline{Data: pyquen pp150}{(pp dijet)}";
        // dataLabel = " (pp Dijet) (Data: pyquen pp150)";
        // label = "pp150_1";
        // legendLabel = "pp150";
    }
    return dataLabel;
}

int main(int argc, char *argv[]) {
    SetThesisStyle();
    CommandLine CL(argc, argv);
    string fileName = CL.Get("input");

    double trackPtCut = 1.0;

    TFile File(fileName.c_str());

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
    cout << "n entries in tree: " << entryCount << endl;

    // TH2D * hist = new TH2D("hist", "Jet Shape;r;\\rho(r)", 6, 0, 0.3, 100, 1, -1);
    double dr = 0.05;  // bins are [0, 0.05), [0.05, 0.10), [0.10, 0.15), ... [0.25, 0.30)

    Int_t n = 6;
    Double_t x[n], y[n];
    Double_t ex[n], ey[n];

    double r_a;
    double r_b;
    double rho;
    for (int bin=0; bin < n; bin++){
        r_a = bin * dr;
        r_b = (bin + 1) * dr;
        cout << "ra: " << r_a << ", rb: " << r_b << endl;

        rho = 0;

        int count = 0;
        int particlesCount = 0;

        for(int i = 0; i < entryCount; i++)  // iterating through every entry in the tree // TODO wait what is this
        {
            tree->GetEntry(i);

            if(signalJetPt == nullptr)
                continue;

            int nJet = signalJetPt->size();
            // cout << "jets: " << nJet << endl;
            int nParticles = particlesEta->size();
            // cout << "particles: " << nParticles << endl;
            count += nJet;
            particlesCount += nParticles;

            for(int j = 0; j < nJet; j++) // iterating through each signal jet
            {   
                double sumTrackPts = 0;
                // now we need to iterate through each track (charged particle) in the signal jet
                for (int k = 0; k < nParticles; k++){
                    if ((*particlesPt)[k] > trackPtCut) { // only for particles > trackPtCut
                        double r = getR((*particlesEta)[k], (*signalJetEta)[j], (*particlesPhi)[k], (*signalJetPhi)[j] );  // TODO factor in weights??

                        // check track is in [r_a, r_b)
                        if (r >= r_a && r < r_b) {
                            // pT of track to var
                            sumTrackPts += (*particlesPt)[k]; // todo maybe weights?
                        }
                    }
                }
                // divide summed pT of track by pT of jet and add to var
                rho += sumTrackPts / (*signalJetPt)[j];
            }
        }
        cout << "total jets count " << count  << endl;
        cout << "total particles count " << particlesCount << endl;
        rho *= 1/dr * 1/count;
        cout << r_a + 0.5 * dr << " " << rho << endl;
        // hist->Fill(r_a + 0.5 * dr, rho); // x, y
        x[bin] = r_a + 0.5 * dr;
        y[bin] = rho;
        ex[bin] = 0.5 * dr;
        ey[bin] = 0;
    }
    TGraph* gr = new TGraphErrors(6, x, y, ex, ey);
    TCanvas* cn = new TCanvas("cn","",1200,900);
    cn->SetRightMargin(0.05);
    string dataLabel = getDataLabel(fileName);


    gr->SetTitle("Jet Shape;r;\\rho(r)");
    gr->SetMarkerSize(2);
    gr->Draw("APZ");
    cn->SetLogy();
    // hist->Draw("SCAT");
    // hist->Draw("COLZ");

    // draw text
    TLatex *text = new TLatex();
    text->SetTextSize(0.025);
    string latexText = "#splitline{p_{T}^{track} > 1GeV, p_T^{jet} > 0GeV}{"+dataLabel+"}";
    text->DrawLatexNDC(0.65, 0.82, latexText.c_str());


    cn->Update();
    cn->SaveAs("test.jpg");

    return 0;
}
