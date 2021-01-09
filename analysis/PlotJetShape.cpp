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

double get_r(double eta_track, double eta_jet, double phi_track, double phi_jet) {
    // this is the reconstructed track's radial distance from jet axis (defined by eta_jet and phi_jet)
    // tracks = charged particles
    double deta = eta_track - eta_jet;
    double dphi = phi_track - phi_jet;

    // dphi should be between -pi and pi
    assert(dphi >= -M_PI && dphi <= M_PI);

    return sqrt(pow(deta,2) + pow(dphi, 2));
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
        particlesCount += nParticles;

        double rho = 0;

        for(int j = 0; j < nJet; j++) // iterating through each signal jet
        {   
            double sumTrackPts = 0;
            // now we need to iterate through each track (charged particle) in the signal jet
            for (int k = 0; k < nParticles; k++){
                if ((*particlesPt)[k] > trackPtCut) { // only for particles > trackPtCut
                    double r = get_r((*particlesEta)[k], (*signalJetEta)[j], (*particlesPhi)[k], (*signalJetPhi)[j] )  // TODO factor in weights??

                    // check track is in [r_a, r_b)
                    if (r >= r_a && r < r_b) {
                        // pT of track to var
                        sumTrackPts += (*particlesPt)[k]; // todo maybe weights?
                    }
                }
            }

            // divide summed pT of track by pT of jet and add to var
            rho += sumTrackPts / (*signalJetPt)[j];
            rho *= 1/dr * 1/Njet;
        }

    }
    cout << "total jets count " << count << endl;
    cout << "total particles count " << particlesCount << endl;
    return 0;
}
