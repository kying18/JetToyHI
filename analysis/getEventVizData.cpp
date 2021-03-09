#include <iostream>
#include <fstream>
#include <iterator>
#include <stdio.h>
#include <dirent.h>
#include <cmath>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "uti.h"


// g++ getEventVizData.cpp $(root-config --cflags --libs) -O2 -o "getEventVizData.exe"
// ./getEventVizData.exe -input "/data/kying/jetTopicsData/ppPhotonJet24/pp_2.root" -output "eventViz/pp_2_" -events "144,325,373,393,433,465"

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

int getColorIndex(double etaTrack, double etaJetLeading, double etaJetSubLeading, double phiTrack, double phiJetLeading, double phiJetSubLeading, int particleID) {
    // phi is 0 to 2pi, eta is -pi to pi

    // If particle == photon:
    //    color index = 1
    // Else:
    //    If particle close to leading jet:
    //       color index = 2
    //    ElseIf particle close to subleading jet:
    //       color index = 3
    //    Else:
    //       color index = 4
    cout << particleID << " " << etaTrack << " " << phiTrack << endl;
    if (particleID == 22) { return 1; }

    if (-M_PI <= etaJetLeading && etaJetLeading <= M_PI && 0 <= phiJetLeading && phiJetLeading <= 2*M_PI) {
        double drLeading = getR(etaTrack, etaJetLeading, phiTrack, phiJetLeading);
        if (drLeading < 0.4) { return 2; }
    }

    if (-M_PI <= etaJetSubLeading && etaJetSubLeading <= M_PI && 0 <= phiJetSubLeading && phiJetSubLeading <= 2*M_PI) {
        double drSubLeading = getR(etaTrack, etaJetSubLeading, phiTrack, phiJetSubLeading);
        if (drSubLeading < 0.4) { return 3; }
    }

    return 4;
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
    // todo: what if there is no leading particle?
    return false; // false for now.. don't count it
}

int main(int argc, char *argv[])
{
    // px, py, pz, color index
    SetThesisStyle();

    CommandLine CL(argc, argv);

    string inputFile = CL.Get("input");
    string outputBase = CL.Get("output");
    double minPT = CL.GetDouble("minpt", 100);
    vector<int> events = CL.GetIntVector("events");

    // stuff for the output csv
    ofstream outFile(outputBase + "eventFile.txt", ios_base::app);
    ostream_iterator<string> output_iterator(outFile, "\n");

    TFile File(inputFile.c_str());
    cout <<inputFile.c_str()<<endl;
    TTree *Tree = (TTree *)File.Get("JetTree");

    if(Tree == nullptr)
    {
        File.Close();
        return 0;
    }

    vector<double> *SignalJetNConstituent = nullptr;
    vector<double> *SignalJetPt = nullptr;
    vector<double> *Weight = nullptr;
    vector<double> *SignalJetPhi = nullptr;
    vector<double> *SignalJetEta = nullptr;
    vector <double> *LeadingZPhi = nullptr;
    vector <double> *LeadingPhotonPhi = nullptr;
    vector <double> *LeadingPhotonEta = nullptr;
    vector <double> *LeadingPhotonPt = nullptr;
    vector<double> *ParticlesPx = nullptr;
    vector<double> *ParticlesPy = nullptr;
    vector<double> *ParticlesPz = nullptr;
    vector<int> *ParticlesID = nullptr;
    vector<double> *ParticlesEta = nullptr;
    vector<double> *ParticlesPhi = nullptr;

    Tree->SetBranchAddress("SignalJetNConstituent", &SignalJetNConstituent);
    Tree->SetBranchAddress("SignalJetPt", &SignalJetPt);
    Tree->SetBranchAddress("SignalJetPhi", &SignalJetPhi);
    Tree->SetBranchAddress("SignalJetEta", &SignalJetEta);
    Tree->SetBranchAddress("LeadingZPhi", &LeadingZPhi);
    Tree->SetBranchAddress("LeadingPhotonPhi", &LeadingPhotonPhi);
    Tree->SetBranchAddress("LeadingPhotonEta", &LeadingPhotonEta);
    Tree->SetBranchAddress("LeadingPhotonPt", &LeadingPhotonPt);
    Tree->SetBranchAddress("EventWeight", &Weight);
    Tree->SetBranchAddress("ParticlesEta", &ParticlesEta);
    Tree->SetBranchAddress("ParticlesPhi", &ParticlesPhi);
    Tree->SetBranchAddress("ParticlesPx", &ParticlesPx);
    Tree->SetBranchAddress("ParticlesPy", &ParticlesPy);
    Tree->SetBranchAddress("ParticlesPz", &ParticlesPz);
    Tree->SetBranchAddress("pdg_id", &ParticlesID);

    for(int iE: events)
    {
        Tree->GetEntry(iE);

        if(SignalJetPt == nullptr)
            continue;

        outFile << "################################" << " Event " << iE << " ################################" << endl;

        int NJet = SignalJetPt->size();
        int NParticles = ParticlesID->size();

        cout << iE << ", particles " << NParticles << endl;

        double MaxPT = 0; // to find max pt jet in event
        int MaxPTEventIndex = -1;
        double MaxPT2 = 0; // to find second max pt jet in event
        int MaxPTEventIndex2 = -1;

        // iterate through the jets in the event to find the leading jets
        for(int iJ = 0; iJ < NJet; iJ++)
        {
            cout << "leading photon phi: "<< (*LeadingPhotonPhi)[0] << ", eta: "<< (*LeadingPhotonEta)[iJ]<< ", pt: "<< (*LeadingPhotonPt)[0] << endl;
            cout << "signal jet phi: "<< (*SignalJetPhi)[0] << ", signal jet eta: "<< (*SignalJetEta)[iJ] << endl;
            
            // if(zJet && !oppositeHemisphere((*LeadingZPhi)[0], (*SignalJetPhi)[iJ])) continue; // if its a z jet and not in the opposite hemisphere according to leading z phi
            if(!oppositeHemisphere((*LeadingPhotonPhi)[0], (*SignalJetPhi)[iJ])) continue; // same logic but photons

            cout << "signal jet pt: "<< (*SignalJetPt)[iJ] << endl;

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
        }

        cout << "max pt event idx: " << MaxPTEventIndex << ", max pt event idx 2: " << MaxPTEventIndex2 << endl;
        
        double etaJetLeading = -INFINITY;
        double etaJetSubLeading = -INFINITY;
        double phiJetLeading = -INFINITY;
        double phiJetSubLeading = -INFINITY;

        if (MaxPTEventIndex > -1) {
            etaJetLeading = (*SignalJetEta)[MaxPTEventIndex];
            phiJetLeading = (*SignalJetPhi)[MaxPTEventIndex];
            outFile << "######################" << " Leading Jet: Eta " << etaJetLeading << ", Phi " << phiJetLeading << " ######################" << endl;
        }
        if (MaxPTEventIndex2 > -1) {
            etaJetSubLeading = (*SignalJetEta)[MaxPTEventIndex2];
            phiJetSubLeading = (*SignalJetPhi)[MaxPTEventIndex2];
            outFile << "######################" << " Subleading Jet: Eta " << etaJetSubLeading << ", Phi " << phiJetSubLeading << " ######################" << endl;
        }

    

        // cout << etaJetLeading << " " << phiJetLeading << " " << etaJetSubLeading << " " << phiJetSubLeading << " " << endl;

        // aight now we gotta get all the particles
        //  find the dr value and color code
        for (int iP=0; iP < NParticles; iP++) {
            int colorIndex = getColorIndex((*ParticlesEta)[iP], etaJetLeading, etaJetSubLeading, (*ParticlesPhi)[iP], phiJetLeading, phiJetSubLeading, (*ParticlesID)[iP]);
            outFile << (*ParticlesPx)[iP] << "," << (*ParticlesPy)[iP] << "," << (*ParticlesPz)[iP] << "," << colorIndex << endl;
        }
    }
    File.Close();
    

    return 0;

}


