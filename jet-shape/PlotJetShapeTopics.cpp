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

// g++ PlotJetShapeTopics.cpp $(root-config --cflags --libs) -O2 -o "plotJetShapeTopics.exe"
// ./plotJetShapeTopics.exe -input "/data/kying/EMMIResults/pp80,/data/kying/EMMIResults/PbPb80_0_10"
// ./plotJetShapeTopics.exe -input "/data/kying/final80/pp80,/data/kying/final80/ppPhotonJet80" -kappas "80_pt80100_eta_pp80_pp80_photonjet_SN,N=4_400,16000,14000_0_80" -minpt 80 -maxpt 100 

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

// we need to read the kappas!
pair<vector<double>, vector<double>> readAllKappas(string filePath){
    pair<vector<double>, vector<double>> result;

    // Create an input filestream
    ifstream kappasFile(filePath);

    // Make sure the file is open
    if(!kappasFile.is_open()) throw runtime_error("Could not open kappas csv file");

    string line, kappa;
    int i = 0;
    cout.precision(17);
    while(getline(kappasFile, line, ',')){
        stringstream ss(line);
        while(getline(ss, kappa, ',')){
            if (i == 0) {
                result.first.push_back(stod(kappa));
            } else {
                result.second.push_back(stod(kappa));
            }
            if(ss.peek() == ',') ss.ignore();
        }
        i += 1;
    }

    return result;
}

// pair < mean, std >
pair<double, double> getMeanAndStd(vector<double> & data){
    pair<double, double> result;

    int totalN = data.size();

    // getting the mean
    double totalSum = 0;
    for (double d: data) {
        totalSum += d;
    }
    double mean = totalSum / totalN;
    result.first = mean;

    // getting the standard deviation
    double totalSquareDistMean = 0;
    for (double d: data) {
        totalSquareDistMean += pow(d - mean, 2);
    }
    double std = sqrt(totalSquareDistMean/totalN); // Note: should be totalN - 1 if sample, but tbh this is like diff between 999 and 1000
    result.second = std;

    return result;
}

pair<pair<double, double>, pair<double, double>> getKappa(string filePath){
    pair<pair<double, double>, pair<double, double>> result;
    pair<vector<double>, vector<double>> kappas = readAllKappas(filePath);

    result.first = getMeanAndStd(kappas.first);
    result.second = getMeanAndStd(kappas.second);
    return result;
}

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

tuple<string, string, string> getDataLabel(string folder) {
    // given the folder, return how to modify the title, the label of the output file, and the label in the legend
    string titleMod, label, legendLabel;
    if (folder.find("ppZJet") != string::npos){
        titleMod = " (pp z-jet) (Data: pyquen ppZJet80)";
        label = "pp80_zjet";
        legendLabel = "ppZJet80";
    } else if (folder.find("PbPbWideZJet") != string::npos) {
        titleMod = " (PbPb wide z-jet) (Data: pyquen PbPbWideZJet80)";
        label = "pbpb80_0_10_wide_zjet";
        legendLabel = "PbPbWideZJet80";
    } else if (folder.find("ppPhotonJet") != string::npos) {
        titleMod = " (pp photon jet) (Data: pyquen ppPhotonJet80)";
        label = "pp80_photonjet";
        legendLabel = "ppPhotonJet80";
    } else if (folder.find("PbPbWidePhotonJet") != string::npos) {
        titleMod = " (PbPb wide photon jet) (Data: pyquen PbPbWidePhotonJet80)";
        label = "pbpb80_0_10_wide_photonjet";
        legendLabel = "PbPbWidePhotonJet80";
    } else if (folder.find("PbPbWide") != string::npos){
        titleMod = " (PbPb wide) (Data: pyquen PbPbWide80_0_10)";
        label = "pbpb80_0_10_wide";
        legendLabel = "PbPbWide80";
    } else if (folder.find("PbPb") != string::npos){
        titleMod = " (PbPb) (Data: pyquen PbPb80_0_10)";
        label = "pbpb80_0_10";
        legendLabel = "PbPb80";
    } else if (folder.find("pp") != string::npos){
        titleMod = " (pp dijet) (Data: pyquen pp80)";
        label = "pp80";
        legendLabel = "pp80";
    }

    return make_tuple(titleMod, label, legendLabel);
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

int main(int argc, char *argv[]) {
    SetThesisStyle();
     
    CommandLine CL(argc, argv);

    vector<string> InputFileFolders = CL.GetStringVector("input");
    double MinPT = CL.GetDouble("minpt", 0);
    double MaxPT = CL.GetDouble("maxpt", 500);
    double trackPtCut = CL.GetDouble("trkpt", 1.0);
    string kappasFile = CL.Get("kappas");
    double dr = CL.GetDouble("dr", 0.05);  // bins are [0, 0.05), [0.05, 0.10), [0.10, 0.15), ... [0.25, 0.30)
    double MaxR = CL.GetDouble("maxr", 0.30);  // max value of max on rightmost bin
    
    bool pp = true; // boolean for title of plot

    pair<pair<double, double>, pair<double, double>> kappas = getKappa("./kappas/" + kappasFile + ".csv"); // should be pp then PbPb
    cout << "Got kappas... kappa12: " << kappas.first.first << ", kappa21: " << kappas.second.first << endl;

    int NBins = (int) ceil(MaxR/dr);
    Int_t NTopics = 2;
    Double_t x[NBins], y[NTopics*3][NBins]; // 01 original, 23 q/g truth, 45 topics
    Double_t ex[NBins], ey[NTopics*3][NBins];
    string legendLabels[NTopics*3];
    double yMin = INFINITY;
    double yMax = -INFINITY;

    int count[NTopics+1][NBins]; // 0 will be for the folder, 1 quark, 2 gluon
    double rho[NTopics+1][NBins];
    for (int i=0; i < NTopics+1; i++) {
        fill(count[i], count[i]+NBins, 0);
        fill(rho[i], rho[i]+NBins, 0);
    }

    for(int f=0; f < NTopics; f++) {
        string folder = InputFileFolders[f];
        cout << folder << endl;
        struct dirent *de; // to get files in path
        DIR *dir = opendir(folder.c_str()); 
        if (dir == NULL)  // opendir returns NULL if couldn't open directory 
        { 
            cout << "Could not open directory: " << folder << endl;
            return 0; 
        }

        string titleMod, label;
        TString legendLabel;
        tie(titleMod, label, legendLabel) = getDataLabel(folder);

        if (legendLabel == "PbPbWide80") pp = false;

        // INITIALIZING THE BUCKETS FOR JET SHAPE
        // int count [NBins];
        // double rho [NBins];
        fill(count[0], count[0]+NBins, 0);
        fill(rho[0], rho[0]+NBins, 0);

        bool photonJet = false;
        if (folder.find("PhotonJet") != string::npos) photonJet = true;

        int fileCount = 0;
        while ((de = readdir(dir)) != NULL) {
            if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ) continue;
            // if (fileCount >= 10) continue; // REMOVE
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
            vector<double> *SignalJetEta = nullptr;
            vector <double> *LeadingZPhi = nullptr;
            vector <double> *LeadingPhotonPhi = nullptr;
            vector<vector<double>> *SignalJetConstituentPt = nullptr;
            vector<vector<double>> *SignalJetConstituentEta = nullptr;
            vector<vector<double>> *SignalJetConstituentPhi = nullptr;
            vector<vector<double>> *SignalJetConstituentPid = nullptr;

            Tree->SetBranchAddress("SignalJetNConstituent", &SignalJetNConstituent);
            Tree->SetBranchAddress("SignalJetMatrixElem", &SignalJetMatrixElem);
            Tree->SetBranchAddress("SignalJetPt", &SignalJetPt);
            Tree->SetBranchAddress("SignalJetPhi", &SignalJetPhi);
            Tree->SetBranchAddress("SignalJetEta", &SignalJetEta);
            Tree->SetBranchAddress("LeadingZPhi", &LeadingZPhi);
            Tree->SetBranchAddress("LeadingPhotonPhi", &LeadingPhotonPhi);
            Tree->SetBranchAddress("EventWeight", &Weight);
            Tree->SetBranchAddress("SignalJetConstituentPt", &SignalJetConstituentPt);
            Tree->SetBranchAddress("SignalJetConstituentEta", &SignalJetConstituentEta);
            Tree->SetBranchAddress("SignalJetConstituentPhi", &SignalJetConstituentPhi);
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
                    // if((*SignalJetMatrixElem)[iJ] <= 0) continue; // if the matrix element is undefined then continue
                    if(fabs((*SignalJetEta)[iJ]) >= 1) continue; // if abs(eta) > 1 then continue
                    // if((*SignalJetMatrixElemDR)[iJ] > 0.4) continue;
                    // if(zJet && !oppositeHemisphere((*LeadingZPhi)[0], (*SignalJetPhi)[iJ])) continue; // if its a z jet and not in the opposite hemisphere according to leading z phi
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

                int jetIdxs[2];
                jetIdxs[0] = MaxJetPTEventIndex;
                if (photonJet) jetIdxs[1] = -1;
                else jetIdxs[1] = MaxJetPTEventIndex2;
                for (int iJ : jetIdxs) {
                    if (iJ == -1) continue;
                    double sumTrackPts[NTopics+1][NBins];
                    for (int i=0; i < NTopics + 1; i++) {
                        fill(sumTrackPts[i], sumTrackPts[i]+NBins, 0);
                    }

                    int NParticles = (*SignalJetConstituentPt)[iJ].size();

                    int matElem = (*SignalJetMatrixElem)[iJ];

                    for (int iP = 0; iP < NParticles; iP++){
                        if ((*SignalJetConstituentPt)[iJ][iP] > trackPtCut) {
                            double r = getR((*SignalJetConstituentEta)[iJ][iP], (*SignalJetEta)[iJ], (*SignalJetConstituentPhi)[iJ][iP], (*SignalJetPhi)[iJ]);

                            for (int bin=0; bin < NBins; bin++){
                                if ((r >= bin * dr) & (r < (bin + 1) * dr)) {
                                    if (matElem == 21) {
                                        // gluon
                                        sumTrackPts[2][bin] += (*SignalJetConstituentPt)[iJ][iP];
                                    } else {
                                        sumTrackPts[1][bin] += (*SignalJetConstituentPt)[iJ][iP];
                                    }
                                    sumTrackPts[0][bin] += (*SignalJetConstituentPt)[iJ][iP];
                                }
                            }

                        }
                    }
                    for (int bin=0; bin < NBins; bin++){
                        if (matElem == 21) {
                            // gluon
                            rho[2][bin] += sumTrackPts[2][bin] * (*Weight)[0] / (*SignalJetPt)[iJ];
                            count[2][bin] += (*Weight)[0];
                        } else {
                            rho[1][bin] += sumTrackPts[1][bin] * (*Weight)[0] / (*SignalJetPt)[iJ];
                            count[1][bin] += (*Weight)[0];
                        }
                        rho[0][bin] += sumTrackPts[0][bin] * (*Weight)[0] / (*SignalJetPt)[iJ];
                        count[0][bin] += (*Weight)[0];
                    }
                }
            }
            File.Close();
        }
        for (int bin=0; bin < NBins; bin++){
            rho[0][bin] *= 1 / dr * 1 / count[0][bin];
            cout << (bin + 0.5) * dr << " " << rho[0][bin] << endl;
            x[bin] = (bin + 0.5) * dr;
            ex[bin] = 0.5 * dr;
            y[f][bin] = rho[0][bin];
            ey[f][bin] = 0;
        }
        legendLabels[f] = legendLabel;
    }

    for (int bin=0; bin < NBins; bin++){
        rho[1][bin] *= 1 / dr * 1 / count[1][bin];
        rho[2][bin] *= 1 / dr * 1 / count[2][bin];

        y[2][bin] = rho[1][bin];
        y[3][bin] = rho[2][bin];

        cout << "rho q,  rho g: " << rho[1][bin] << ", " << rho[2][bin] << endl;
        ey[2][bin] = 0;
        ey[3][bin] = 0;
    }
    legendLabels[2] = "Quark";
    legendLabels[3] = "Gluon";

    cout << "Successfully got og jet shape" << endl;

    // ok now we have our jet shape value for each bucket.. calculate the fractions using the formula! topic = (p1 - kappa*p2)/(1-kappa)
    for(int i=0; i < NBins; i++) {
        cout << "kappa second: " << kappas.second.first << endl;
        cout << "y1: " << y[1][i] << ", y0: " << y[0][i] << endl;
        y[4][i] = (y[0][i] - kappas.first.first * y[1][i]) / (1 - kappas.first.first);
        y[5][i] = (y[1][i] - kappas.second.first * y[0][i]) / (1 - kappas.second.first);

        ey[4][i] = 0;
        ey[5][i] = 0;
    }
    legendLabels[4] = "Topic 1";
    legendLabels[5] = "Topic 2";

    for (int i=0; i < NTopics * 3 ; i++) {
        yMin = min(yMin, *min_element(y[i], y[i]+NBins));
        yMax = max(yMax, *max_element(y[i], y[i]+NBins));
    }

    cout << "ymin and ymax: " << yMin << ", " << yMax << endl;

    TCanvas* cn = new TCanvas("cn","cn",1200,900);

    TLegend* legend = new TLegend(0.7,0.7,0.9,0.85);
    int plotColor;
    vector<int> primaryColors = GetPrimaryColors();
    vector<int> diffColors = GetDifferentiableColors();
    vector<int> diffLightColors = GetDifferentiableColorsLight();

    // PLOTTING TOPICS JET SHAPE
    // int startIndex = NTopics;
    string sampleLabel = pp? "pp" : "PbPb";
    int startIndex = NTopics;
    for (int i=startIndex; i < NTopics*3; i++) {
        if (i < NTopics) plotColor = 17 + i%2;
        // else if (i < NTopics * 2) plotColor = kGray + i%2 + 1;
        else if (i < NTopics * 2) plotColor = primaryColors[i%2];
        else if (i < NTopics * 3) plotColor = 13 + (i%2)*2 ;
        // plotColor = diffLightColors[i];
        TGraph* gr = new TGraphErrors(NBins, x, y[i], ex, ey[i]);
        gr->SetTitle(("Jet Shape (" + sampleLabel + ");r;\\rho(r)").c_str());
        gr->SetMarkerSize(1.2);
        gr->SetMarkerColor(plotColor);
        gr->SetFillColor(plotColor);
        // gr->SetFillStyle(3001);
        gr->GetYaxis()->SetRangeUser(yMin*0.8, yMax*1.3);
        gr->GetYaxis()->SetMoreLogLabels();
        legend->AddEntry(gr, legendLabels[i].c_str(), "p");
        if (i == startIndex) {
            gr->Draw("2apz");
        } else {
            gr->Draw("2pz same");
        }
        cn->Update();
    }

    cout << "Successfully got dem graphs" << endl;

    // PLOTTING REST OF STUFF/DRAWING

    // draw text
    TLatex *text = new TLatex();
    text->SetTextSize(0.025);
    string latexText = "#splitline{p_{T}^{track} > " + to_string((int)trackPtCut) + "GeV, " + to_string((int)MinPT) + " GeV < p_{T}^{jet} < " + to_string((int)MaxPT) + "GeV}{| #eta^{jet} | < 1 }"; //0.3 < |\\eta^{jet}| < 2
    // string latexText = "#splitline{p_{T}^{track} > 1GeV, p_T^{jet} > 0GeV}{"+dataLabel+"}";
    // text->DrawLatexNDC(0.65, 0.82, latexText.c_str());
    text->DrawLatexNDC(0.15, 0.2, latexText.c_str());
    // mg->Draw("a");

    cout << "draw text" << endl;

    legend->SetTextSize(0.025);
    legend->SetFillColor(0);
    legend->SetBorderSize(1);
    legend->Draw("");

    cout << "draw legend" << endl;

    cn->SetLogy(1);
    cn->SetRightMargin(0.05);
    cn->Update();
    cout << "before save" << endl;
    cn->SaveAs(("./jetShapePlots/topics_" + kappasFile + "_jetpt" + to_string((int)MinPT) + "_trackpt" + to_string((int)trackPtCut) + "_" + to_string(MaxR) + "r_" + to_string(NBins) +"buckets.jpg").c_str());
    // cn->SaveAs("./jetShapePlots/temp.png");

    cout << "save plot" << endl;

    return 0;
}