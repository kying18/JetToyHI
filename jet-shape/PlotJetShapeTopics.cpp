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
// ./plotJetShape.exe -input "/data/kying/EMMIResults/pp150,/data/kying/EMMIResults/PbPb150_0_10"


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
    ifstream kappasFile("./x100_pppbpb_kappas.csv");

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

// def topic_and_err(p1, p1_errs, p2, p2_errs, kappa, kappa_errs):
//     topic = (p1 - kappa*p2)/(1-kappa)
//     topic_errs = np.sqrt((p1 - p2)**2 * kappa_errs**2 + (1 - kappa)**2 * (p1_errs**2 + kappa**2 * p2_errs**2)) / (1 - kappa)**2
//     return [topic, topic_errs]

// def calc_topics(p1, p1_errs, p2, p2_errs, kappa12, kappa21):
//     return [topic_and_err(p1,p1_errs,p2,p2_errs,*kappa12), topic_and_err(p2,p2_errs,p1,p1_errs,*kappa21)]


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
    TLegend* legend = new TLegend(0.7,0.7,0.9,0.85);
    // TLegend* legend = new TLegend(0.1, 0.1, 0.3, 0.3);

    int plotColor = 2;
    
    // double dr = 0.05;
    // Int_t n = 6;
    Int_t n = (int) ceil(maxR/dr);
    Int_t nTopics = 2;
    Double_t x[n], y[nTopics][n];
    Double_t ex[n], ey[nTopics][n];
    string legendLabels[nTopics];
    double yMin = INFINITY;
    double yMax = -INFINITY;

    // get kappa value so that we can calculate topic = (p1 - kappa*p2)/(1-kappa)
    pair<pair<double, double>, pair<double, double>> kappas = getKappa("./x100_pppbpb_kappas.csv"); // should be pp then PbPb
    cout << "Got kappas... kappa12: " << kappas.first.first << ", kappa21: " << kappas.second.first << endl;

    // now get the two topics (make sure that the order is consistent)
    for(int f=0; f < nTopics; f++) {  // TODO: hard coded that there would be 2 inputs
        string folder = InputFileFolders[f];
        string dataLabel, fileLabel, legendLabel;

        double r_a, r_b;
        double rho, count, totalyerr;
        for (int bin=0; bin < n; bin++){
            r_a = bin * dr;
            r_b = (bin + 1) * dr;

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
        double currMin = *min_element(y[f], y[f]+n);
        yMin = min(currMin, yMin);
        double currMax = *max_element(y[f], y[f]+n);
        yMax = max(currMax, yMax);
        legendLabels[f] = legendLabel;
    }

    cout << "Successfully got og jet shape" << endl;

    // ok now we have our jet shape value for each bucket.. calculate the fractions using the formula! topic = (p1 - kappa*p2)/(1-kappa)
    Double_t topics[nTopics][n], topicsErr[nTopics][n];
    for(int i=0; i < n; i++) {
        topics[0][i] = (y[0][i] - kappas.first.first * y[1][i]) / (1 - kappas.first.first);
        topics[1][i] = (y[1][i] - kappas.second.first * y[0][i]) / (1 - kappas.second.first);

        topicsErr[0][i] = 0;
        topicsErr[1][i] = 0;
    }

    cout << "Successfully got jet shape as topics" << endl;

    yMin = min(yMin, *min_element(y[0], y[0]+n));
    yMin = min(yMin, *min_element(y[1], y[1]+n));
    yMax = max(yMax, *max_element(y[0], y[0]+n));
    yMax = max(yMax, *max_element(y[1], y[1]+n));

    // PLOTTING TOPICS JET SHAPE
    for (int i=0; i < nTopics; i++) {
        TGraph* gr = new TGraphErrors(n, x, topics[i], ex, topicsErr[i]);
        gr->SetTitle("Jet Shape;r;\\rho(r)");
        gr->SetMarkerSize(1.2);
        gr->SetMarkerColor(plotColor + i);
        gr->SetFillColor(plotColor + i);
        // gr->SetFillStyle(3001);
        gr->GetYaxis()->SetRangeUser(yMin*0.8, yMax*1.3);
        gr->GetYaxis()->SetMoreLogLabels();
        if (i == 0) {
            legend->AddEntry(gr, "Topic 1", "p");
            gr->Draw("2pz");
        } else if (i == 1) {
            legend->AddEntry(gr, "Topic 2", "p");
            gr->Draw("2pz same");
        }
    }

    // PLOTTING INPUT JET SHAPES
    for (int i=0; i < nTopics; i++) {
        TGraph* gr = new TGraphErrors(n, x, y[i], ex, y[i]);
        gr->SetTitle("Jet Shape;r;\\rho(r)");
        gr->SetMarkerSize(1.2);
        gr->SetMarkerColor(plotColor + 2 + i);
        gr->SetFillColor(plotColor + 2 + i);
        // gr->SetFillStyle(3001);
        gr->GetYaxis()->SetRangeUser(yMin*0.8, yMax*1.3);
        gr->GetYaxis()->SetMoreLogLabels();
        legend->AddEntry(gr, legendLabels[i].c_str(), "p");
        gr->Draw("2pz same");
    }

    cout << "Successfully got dem graphs" << endl;

    // PLOTTING REST OF STUFF/DRAWING

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
    cn->SaveAs(("./jetShapePlots/jetshapetopics_jetpt" + to_string((int)minPT) + "_trackpt" + to_string((int)trackPtCut) + "_" + to_string(maxR) + "r_" + to_string(n) +"buckets.jpg").c_str());

    return 0;
}