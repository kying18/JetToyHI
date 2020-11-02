//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Oct 18 13:18:24 2020 by ROOT version 6.16/00
// from TTree JetTree/JetToyHI tree
// found on file: pythia10000.root
//////////////////////////////////////////////////////////

#ifndef pythiaAnalysis_h
#define pythiaAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "uti.h"


// Header file for the classes stored in the TTree if any.
#include "vector"
#include <vector>

using namespace std;

class pythiaAnalysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   vector<double>  *SignalJetPt;
   vector<double>  *SignalJetEta;
   vector<double>  *SignalJetPhi;
   vector<double>  *SignalJetM;
   vector<double>  *SignalJetArea;
   vector<vector<double> > *SignalJetAKDRs;
   vector<vector<double> > *SignalJetAKEta1s;
   vector<vector<double> > *SignalJetAKEta2s;
   vector<vector<double> > *SignalJetAKPT1s;
   vector<vector<double> > *SignalJetAKPT2s;
   vector<vector<double> > *SignalJetAKPhi1s;
   vector<vector<double> > *SignalJetAKPhi2s;
   vector<vector<double> > *SignalJetAKZGs;
   vector<vector<double> > *SignalJetCAAKDRs;
   vector<vector<double> > *SignalJetCAAKEta1s;
   vector<vector<double> > *SignalJetCAAKEta2s;
   vector<vector<double> > *SignalJetCAAKPT1s;
   vector<vector<double> > *SignalJetCAAKPT2s;
   vector<vector<double> > *SignalJetCAAKPhi1s;
   vector<vector<double> > *SignalJetCAAKPhi2s;
   vector<vector<double> > *SignalJetCAAKZGs;
   vector<vector<double> > *SignalJetCADRs;
   vector<vector<double> > *SignalJetCAEta1s;
   vector<vector<double> > *SignalJetCAEta2s;
   vector<vector<double> > *SignalJetCAKTDRs;
   vector<vector<double> > *SignalJetCAKTEta1s;
   vector<vector<double> > *SignalJetCAKTEta2s;
   vector<vector<double> > *SignalJetCAKTPT1s;
   vector<vector<double> > *SignalJetCAKTPT2s;
   vector<vector<double> > *SignalJetCAKTPhi1s;
   vector<vector<double> > *SignalJetCAKTPhi2s;
   vector<vector<double> > *SignalJetCAKTZGs;
   vector<vector<double> > *SignalJetCAPT1s;
   vector<vector<double> > *SignalJetCAPT2s;
   vector<vector<double> > *SignalJetCAPhi1s;
   vector<vector<double> > *SignalJetCAPhi2s;
   vector<vector<double> > *SignalJetCAZGs;
   vector<vector<double> > *SignalJetKTDRs;
   vector<vector<double> > *SignalJetKTEta1s;
   vector<vector<double> > *SignalJetKTEta2s;
   vector<vector<double> > *SignalJetKTPT1s;
   vector<vector<double> > *SignalJetKTPT2s;
   vector<vector<double> > *SignalJetKTPhi1s;
   vector<vector<double> > *SignalJetKTPhi2s;
   vector<vector<double> > *SignalJetKTZGs;
   vector<double>  *SignalJetJewelPt;
   vector<double>  *SignalJetJewelEta;
   vector<double>  *SignalJetJewelPhi;
   vector<double>  *SignalJetJewelM;
   vector<double>  *SignalJetJewelArea;
   vector<double>  *Rho;
   vector<double>  *RhoM;
   vector<double>  *SignalJetSD1Pt;
   vector<double>  *SignalJetSD1Eta;
   vector<double>  *SignalJetSD1Phi;
   vector<double>  *SignalJetSD1M;
   vector<double>  *SignalJetSD1Area;
   vector<double>  *SignalJetSD1Subjet1Pt;
   vector<double>  *SignalJetSD1Subjet1Eta;
   vector<double>  *SignalJetSD1Subjet1Phi;
   vector<double>  *SignalJetSD1Subjet1M;
   vector<double>  *SignalJetSD1Subjet1Area;
   vector<double>  *SignalJetSD1Subjet2Pt;
   vector<double>  *SignalJetSD1Subjet2Eta;
   vector<double>  *SignalJetSD1Subjet2Phi;
   vector<double>  *SignalJetSD1Subjet2M;
   vector<double>  *SignalJetSD1Subjet2Area;
   vector<double>  *SignalJetSD1DR12;
   vector<double>  *SignalJetSD1ZG;
   vector<int>     *SignalJetSD1NBranch;
   vector<double>  *SignalJetSD2Pt;
   vector<double>  *SignalJetSD2Eta;
   vector<double>  *SignalJetSD2Phi;
   vector<double>  *SignalJetSD2M;
   vector<double>  *SignalJetSD2Area;
   vector<double>  *SignalJetSD2Subjet1Pt;
   vector<double>  *SignalJetSD2Subjet1Eta;
   vector<double>  *SignalJetSD2Subjet1Phi;
   vector<double>  *SignalJetSD2Subjet1M;
   vector<double>  *SignalJetSD2Subjet1Area;
   vector<double>  *SignalJetSD2Subjet2Pt;
   vector<double>  *SignalJetSD2Subjet2Eta;
   vector<double>  *SignalJetSD2Subjet2Phi;
   vector<double>  *SignalJetSD2Subjet2M;
   vector<double>  *SignalJetSD2Subjet2Area;
   vector<double>  *SignalJetSD2DR12;
   vector<double>  *SignalJetSD2ZG;
   vector<int>     *SignalJetSD2NBranch;
   vector<double>  *SignalJetSD3Pt;
   vector<double>  *SignalJetSD3Eta;
   vector<double>  *SignalJetSD3Phi;
   vector<double>  *SignalJetSD3M;
   vector<double>  *SignalJetSD3Area;
   vector<double>  *SignalJetSD3Subjet1Pt;
   vector<double>  *SignalJetSD3Subjet1Eta;
   vector<double>  *SignalJetSD3Subjet1Phi;
   vector<double>  *SignalJetSD3Subjet1M;
   vector<double>  *SignalJetSD3Subjet1Area;
   vector<double>  *SignalJetSD3Subjet2Pt;
   vector<double>  *SignalJetSD3Subjet2Eta;
   vector<double>  *SignalJetSD3Subjet2Phi;
   vector<double>  *SignalJetSD3Subjet2M;
   vector<double>  *SignalJetSD3Subjet2Area;
   vector<double>  *SignalJetSD3DR12;
   vector<double>  *SignalJetSD3ZG;
   vector<int>     *SignalJetSD3NBranch;
   vector<double>  *SignalJetSD4Pt;
   vector<double>  *SignalJetSD4Eta;
   vector<double>  *SignalJetSD4Phi;
   vector<double>  *SignalJetSD4M;
   vector<double>  *SignalJetSD4Area;
   vector<double>  *SignalJetSD4Subjet1Pt;
   vector<double>  *SignalJetSD4Subjet1Eta;
   vector<double>  *SignalJetSD4Subjet1Phi;
   vector<double>  *SignalJetSD4Subjet1M;
   vector<double>  *SignalJetSD4Subjet1Area;
   vector<double>  *SignalJetSD4Subjet2Pt;
   vector<double>  *SignalJetSD4Subjet2Eta;
   vector<double>  *SignalJetSD4Subjet2Phi;
   vector<double>  *SignalJetSD4Subjet2M;
   vector<double>  *SignalJetSD4Subjet2Area;
   vector<double>  *SignalJetSD4DR12;
   vector<double>  *SignalJetSD4ZG;
   vector<int>     *SignalJetSD4NBranch;
   vector<double>  *SignalJetSD5Pt;
   vector<double>  *SignalJetSD5Eta;
   vector<double>  *SignalJetSD5Phi;
   vector<double>  *SignalJetSD5M;
   vector<double>  *SignalJetSD5Area;
   vector<double>  *SignalJetSD5Subjet1Pt;
   vector<double>  *SignalJetSD5Subjet1Eta;
   vector<double>  *SignalJetSD5Subjet1Phi;
   vector<double>  *SignalJetSD5Subjet1M;
   vector<double>  *SignalJetSD5Subjet1Area;
   vector<double>  *SignalJetSD5Subjet2Pt;
   vector<double>  *SignalJetSD5Subjet2Eta;
   vector<double>  *SignalJetSD5Subjet2Phi;
   vector<double>  *SignalJetSD5Subjet2M;
   vector<double>  *SignalJetSD5Subjet2Area;
   vector<double>  *SignalJetSD5DR12;
   vector<double>  *SignalJetSD5ZG;
   vector<int>     *SignalJetSD5NBranch;
   vector<double>  *SignalJetSD1JewelPt;
   vector<double>  *SignalJetSD1JewelEta;
   vector<double>  *SignalJetSD1JewelPhi;
   vector<double>  *SignalJetSD1JewelM;
   vector<double>  *SignalJetSD1JewelArea;
   vector<double>  *SignalJetSD1JewelSubjet1Pt;
   vector<double>  *SignalJetSD1JewelSubjet1Eta;
   vector<double>  *SignalJetSD1JewelSubjet1Phi;
   vector<double>  *SignalJetSD1JewelSubjet1M;
   vector<double>  *SignalJetSD1JewelSubjet1Area;
   vector<double>  *SignalJetSD1JewelSubjet2Pt;
   vector<double>  *SignalJetSD1JewelSubjet2Eta;
   vector<double>  *SignalJetSD1JewelSubjet2Phi;
   vector<double>  *SignalJetSD1JewelSubjet2M;
   vector<double>  *SignalJetSD1JewelSubjet2Area;
   vector<double>  *SignalJetSD1JewelDR12;
   vector<double>  *SignalJetSD1JewelZG;
   vector<double>  *SignalJetSD2JewelPt;
   vector<double>  *SignalJetSD2JewelEta;
   vector<double>  *SignalJetSD2JewelPhi;
   vector<double>  *SignalJetSD2JewelM;
   vector<double>  *SignalJetSD2JewelArea;
   vector<double>  *SignalJetSD2JewelSubjet1Pt;
   vector<double>  *SignalJetSD2JewelSubjet1Eta;
   vector<double>  *SignalJetSD2JewelSubjet1Phi;
   vector<double>  *SignalJetSD2JewelSubjet1M;
   vector<double>  *SignalJetSD2JewelSubjet1Area;
   vector<double>  *SignalJetSD2JewelSubjet2Pt;
   vector<double>  *SignalJetSD2JewelSubjet2Eta;
   vector<double>  *SignalJetSD2JewelSubjet2Phi;
   vector<double>  *SignalJetSD2JewelSubjet2M;
   vector<double>  *SignalJetSD2JewelSubjet2Area;
   vector<double>  *SignalJetSD2JewelDR12;
   vector<double>  *SignalJetSD2JewelZG;
   vector<double>  *SignalJetSD3JewelPt;
   vector<double>  *SignalJetSD3JewelEta;
   vector<double>  *SignalJetSD3JewelPhi;
   vector<double>  *SignalJetSD3JewelM;
   vector<double>  *SignalJetSD3JewelArea;
   vector<double>  *SignalJetSD3JewelSubjet1Pt;
   vector<double>  *SignalJetSD3JewelSubjet1Eta;
   vector<double>  *SignalJetSD3JewelSubjet1Phi;
   vector<double>  *SignalJetSD3JewelSubjet1M;
   vector<double>  *SignalJetSD3JewelSubjet1Area;
   vector<double>  *SignalJetSD3JewelSubjet2Pt;
   vector<double>  *SignalJetSD3JewelSubjet2Eta;
   vector<double>  *SignalJetSD3JewelSubjet2Phi;
   vector<double>  *SignalJetSD3JewelSubjet2M;
   vector<double>  *SignalJetSD3JewelSubjet2Area;
   vector<double>  *SignalJetSD3JewelDR12;
   vector<double>  *SignalJetSD3JewelZG;
   vector<double>  *SignalJetSD4JewelPt;
   vector<double>  *SignalJetSD4JewelEta;
   vector<double>  *SignalJetSD4JewelPhi;
   vector<double>  *SignalJetSD4JewelM;
   vector<double>  *SignalJetSD4JewelArea;
   vector<double>  *SignalJetSD4JewelSubjet1Pt;
   vector<double>  *SignalJetSD4JewelSubjet1Eta;
   vector<double>  *SignalJetSD4JewelSubjet1Phi;
   vector<double>  *SignalJetSD4JewelSubjet1M;
   vector<double>  *SignalJetSD4JewelSubjet1Area;
   vector<double>  *SignalJetSD4JewelSubjet2Pt;
   vector<double>  *SignalJetSD4JewelSubjet2Eta;
   vector<double>  *SignalJetSD4JewelSubjet2Phi;
   vector<double>  *SignalJetSD4JewelSubjet2M;
   vector<double>  *SignalJetSD4JewelSubjet2Area;
   vector<double>  *SignalJetSD4JewelDR12;
   vector<double>  *SignalJetSD4JewelZG;
   vector<double>  *SignalJetSD5JewelPt;
   vector<double>  *SignalJetSD5JewelEta;
   vector<double>  *SignalJetSD5JewelPhi;
   vector<double>  *SignalJetSD5JewelM;
   vector<double>  *SignalJetSD5JewelArea;
   vector<double>  *SignalJetSD5JewelSubjet1Pt;
   vector<double>  *SignalJetSD5JewelSubjet1Eta;
   vector<double>  *SignalJetSD5JewelSubjet1Phi;
   vector<double>  *SignalJetSD5JewelSubjet1M;
   vector<double>  *SignalJetSD5JewelSubjet1Area;
   vector<double>  *SignalJetSD5JewelSubjet2Pt;
   vector<double>  *SignalJetSD5JewelSubjet2Eta;
   vector<double>  *SignalJetSD5JewelSubjet2Phi;
   vector<double>  *SignalJetSD5JewelSubjet2M;
   vector<double>  *SignalJetSD5JewelSubjet2Area;
   vector<double>  *SignalJetSD5JewelDR12;
   vector<double>  *SignalJetSD5JewelZG;
   vector<double>  *EventWeight;

   // List of branches
   TBranch        *b_SignalJetPt;   //!
   TBranch        *b_SignalJetEta;   //!
   TBranch        *b_SignalJetPhi;   //!
   TBranch        *b_SignalJetM;   //!
   TBranch        *b_SignalJetArea;   //!
   TBranch        *b_SignalJetAKDRs;   //!
   TBranch        *b_SignalJetAKEta1s;   //!
   TBranch        *b_SignalJetAKEta2s;   //!
   TBranch        *b_SignalJetAKPT1s;   //!
   TBranch        *b_SignalJetAKPT2s;   //!
   TBranch        *b_SignalJetAKPhi1s;   //!
   TBranch        *b_SignalJetAKPhi2s;   //!
   TBranch        *b_SignalJetAKZGs;   //!
   TBranch        *b_SignalJetCAAKDRs;   //!
   TBranch        *b_SignalJetCAAKEta1s;   //!
   TBranch        *b_SignalJetCAAKEta2s;   //!
   TBranch        *b_SignalJetCAAKPT1s;   //!
   TBranch        *b_SignalJetCAAKPT2s;   //!
   TBranch        *b_SignalJetCAAKPhi1s;   //!
   TBranch        *b_SignalJetCAAKPhi2s;   //!
   TBranch        *b_SignalJetCAAKZGs;   //!
   TBranch        *b_SignalJetCADRs;   //!
   TBranch        *b_SignalJetCAEta1s;   //!
   TBranch        *b_SignalJetCAEta2s;   //!
   TBranch        *b_SignalJetCAKTDRs;   //!
   TBranch        *b_SignalJetCAKTEta1s;   //!
   TBranch        *b_SignalJetCAKTEta2s;   //!
   TBranch        *b_SignalJetCAKTPT1s;   //!
   TBranch        *b_SignalJetCAKTPT2s;   //!
   TBranch        *b_SignalJetCAKTPhi1s;   //!
   TBranch        *b_SignalJetCAKTPhi2s;   //!
   TBranch        *b_SignalJetCAKTZGs;   //!
   TBranch        *b_SignalJetCAPT1s;   //!
   TBranch        *b_SignalJetCAPT2s;   //!
   TBranch        *b_SignalJetCAPhi1s;   //!
   TBranch        *b_SignalJetCAPhi2s;   //!
   TBranch        *b_SignalJetCAZGs;   //!
   TBranch        *b_SignalJetKTDRs;   //!
   TBranch        *b_SignalJetKTEta1s;   //!
   TBranch        *b_SignalJetKTEta2s;   //!
   TBranch        *b_SignalJetKTPT1s;   //!
   TBranch        *b_SignalJetKTPT2s;   //!
   TBranch        *b_SignalJetKTPhi1s;   //!
   TBranch        *b_SignalJetKTPhi2s;   //!
   TBranch        *b_SignalJetKTZGs;   //!
   TBranch        *b_SignalJetJewelPt;   //!
   TBranch        *b_SignalJetJewelEta;   //!
   TBranch        *b_SignalJetJewelPhi;   //!
   TBranch        *b_SignalJetJewelM;   //!
   TBranch        *b_SignalJetJewelArea;   //!
   TBranch        *b_Rho;   //!
   TBranch        *b_RhoM;   //!
   TBranch        *b_SignalJetSD1Pt;   //!
   TBranch        *b_SignalJetSD1Eta;   //!
   TBranch        *b_SignalJetSD1Phi;   //!
   TBranch        *b_SignalJetSD1M;   //!
   TBranch        *b_SignalJetSD1Area;   //!
   TBranch        *b_SignalJetSD1Subjet1Pt;   //!
   TBranch        *b_SignalJetSD1Subjet1Eta;   //!
   TBranch        *b_SignalJetSD1Subjet1Phi;   //!
   TBranch        *b_SignalJetSD1Subjet1M;   //!
   TBranch        *b_SignalJetSD1Subjet1Area;   //!
   TBranch        *b_SignalJetSD1Subjet2Pt;   //!
   TBranch        *b_SignalJetSD1Subjet2Eta;   //!
   TBranch        *b_SignalJetSD1Subjet2Phi;   //!
   TBranch        *b_SignalJetSD1Subjet2M;   //!
   TBranch        *b_SignalJetSD1Subjet2Area;   //!
   TBranch        *b_SignalJetSD1DR12;   //!
   TBranch        *b_SignalJetSD1ZG;   //!
   TBranch        *b_SignalJetSD1NBranch;   //!
   TBranch        *b_SignalJetSD2Pt;   //!
   TBranch        *b_SignalJetSD2Eta;   //!
   TBranch        *b_SignalJetSD2Phi;   //!
   TBranch        *b_SignalJetSD2M;   //!
   TBranch        *b_SignalJetSD2Area;   //!
   TBranch        *b_SignalJetSD2Subjet1Pt;   //!
   TBranch        *b_SignalJetSD2Subjet1Eta;   //!
   TBranch        *b_SignalJetSD2Subjet1Phi;   //!
   TBranch        *b_SignalJetSD2Subjet1M;   //!
   TBranch        *b_SignalJetSD2Subjet1Area;   //!
   TBranch        *b_SignalJetSD2Subjet2Pt;   //!
   TBranch        *b_SignalJetSD2Subjet2Eta;   //!
   TBranch        *b_SignalJetSD2Subjet2Phi;   //!
   TBranch        *b_SignalJetSD2Subjet2M;   //!
   TBranch        *b_SignalJetSD2Subjet2Area;   //!
   TBranch        *b_SignalJetSD2DR12;   //!
   TBranch        *b_SignalJetSD2ZG;   //!
   TBranch        *b_SignalJetSD2NBranch;   //!
   TBranch        *b_SignalJetSD3Pt;   //!
   TBranch        *b_SignalJetSD3Eta;   //!
   TBranch        *b_SignalJetSD3Phi;   //!
   TBranch        *b_SignalJetSD3M;   //!
   TBranch        *b_SignalJetSD3Area;   //!
   TBranch        *b_SignalJetSD3Subjet1Pt;   //!
   TBranch        *b_SignalJetSD3Subjet1Eta;   //!
   TBranch        *b_SignalJetSD3Subjet1Phi;   //!
   TBranch        *b_SignalJetSD3Subjet1M;   //!
   TBranch        *b_SignalJetSD3Subjet1Area;   //!
   TBranch        *b_SignalJetSD3Subjet2Pt;   //!
   TBranch        *b_SignalJetSD3Subjet2Eta;   //!
   TBranch        *b_SignalJetSD3Subjet2Phi;   //!
   TBranch        *b_SignalJetSD3Subjet2M;   //!
   TBranch        *b_SignalJetSD3Subjet2Area;   //!
   TBranch        *b_SignalJetSD3DR12;   //!
   TBranch        *b_SignalJetSD3ZG;   //!
   TBranch        *b_SignalJetSD3NBranch;   //!
   TBranch        *b_SignalJetSD4Pt;   //!
   TBranch        *b_SignalJetSD4Eta;   //!
   TBranch        *b_SignalJetSD4Phi;   //!
   TBranch        *b_SignalJetSD4M;   //!
   TBranch        *b_SignalJetSD4Area;   //!
   TBranch        *b_SignalJetSD4Subjet1Pt;   //!
   TBranch        *b_SignalJetSD4Subjet1Eta;   //!
   TBranch        *b_SignalJetSD4Subjet1Phi;   //!
   TBranch        *b_SignalJetSD4Subjet1M;   //!
   TBranch        *b_SignalJetSD4Subjet1Area;   //!
   TBranch        *b_SignalJetSD4Subjet2Pt;   //!
   TBranch        *b_SignalJetSD4Subjet2Eta;   //!
   TBranch        *b_SignalJetSD4Subjet2Phi;   //!
   TBranch        *b_SignalJetSD4Subjet2M;   //!
   TBranch        *b_SignalJetSD4Subjet2Area;   //!
   TBranch        *b_SignalJetSD4DR12;   //!
   TBranch        *b_SignalJetSD4ZG;   //!
   TBranch        *b_SignalJetSD4NBranch;   //!
   TBranch        *b_SignalJetSD5Pt;   //!
   TBranch        *b_SignalJetSD5Eta;   //!
   TBranch        *b_SignalJetSD5Phi;   //!
   TBranch        *b_SignalJetSD5M;   //!
   TBranch        *b_SignalJetSD5Area;   //!
   TBranch        *b_SignalJetSD5Subjet1Pt;   //!
   TBranch        *b_SignalJetSD5Subjet1Eta;   //!
   TBranch        *b_SignalJetSD5Subjet1Phi;   //!
   TBranch        *b_SignalJetSD5Subjet1M;   //!
   TBranch        *b_SignalJetSD5Subjet1Area;   //!
   TBranch        *b_SignalJetSD5Subjet2Pt;   //!
   TBranch        *b_SignalJetSD5Subjet2Eta;   //!
   TBranch        *b_SignalJetSD5Subjet2Phi;   //!
   TBranch        *b_SignalJetSD5Subjet2M;   //!
   TBranch        *b_SignalJetSD5Subjet2Area;   //!
   TBranch        *b_SignalJetSD5DR12;   //!
   TBranch        *b_SignalJetSD5ZG;   //!
   TBranch        *b_SignalJetSD5NBranch;   //!
   TBranch        *b_SignalJetSD1JewelPt;   //!
   TBranch        *b_SignalJetSD1JewelEta;   //!
   TBranch        *b_SignalJetSD1JewelPhi;   //!
   TBranch        *b_SignalJetSD1JewelM;   //!
   TBranch        *b_SignalJetSD1JewelArea;   //!
   TBranch        *b_SignalJetSD1JewelSubjet1Pt;   //!
   TBranch        *b_SignalJetSD1JewelSubjet1Eta;   //!
   TBranch        *b_SignalJetSD1JewelSubjet1Phi;   //!
   TBranch        *b_SignalJetSD1JewelSubjet1M;   //!
   TBranch        *b_SignalJetSD1JewelSubjet1Area;   //!
   TBranch        *b_SignalJetSD1JewelSubjet2Pt;   //!
   TBranch        *b_SignalJetSD1JewelSubjet2Eta;   //!
   TBranch        *b_SignalJetSD1JewelSubjet2Phi;   //!
   TBranch        *b_SignalJetSD1JewelSubjet2M;   //!
   TBranch        *b_SignalJetSD1JewelSubjet2Area;   //!
   TBranch        *b_SignalJetSD1JewelDR12;   //!
   TBranch        *b_SignalJetSD1JewelZG;   //!
   TBranch        *b_SignalJetSD2JewelPt;   //!
   TBranch        *b_SignalJetSD2JewelEta;   //!
   TBranch        *b_SignalJetSD2JewelPhi;   //!
   TBranch        *b_SignalJetSD2JewelM;   //!
   TBranch        *b_SignalJetSD2JewelArea;   //!
   TBranch        *b_SignalJetSD2JewelSubjet1Pt;   //!
   TBranch        *b_SignalJetSD2JewelSubjet1Eta;   //!
   TBranch        *b_SignalJetSD2JewelSubjet1Phi;   //!
   TBranch        *b_SignalJetSD2JewelSubjet1M;   //!
   TBranch        *b_SignalJetSD2JewelSubjet1Area;   //!
   TBranch        *b_SignalJetSD2JewelSubjet2Pt;   //!
   TBranch        *b_SignalJetSD2JewelSubjet2Eta;   //!
   TBranch        *b_SignalJetSD2JewelSubjet2Phi;   //!
   TBranch        *b_SignalJetSD2JewelSubjet2M;   //!
   TBranch        *b_SignalJetSD2JewelSubjet2Area;   //!
   TBranch        *b_SignalJetSD2JewelDR12;   //!
   TBranch        *b_SignalJetSD2JewelZG;   //!
   TBranch        *b_SignalJetSD3JewelPt;   //!
   TBranch        *b_SignalJetSD3JewelEta;   //!
   TBranch        *b_SignalJetSD3JewelPhi;   //!
   TBranch        *b_SignalJetSD3JewelM;   //!
   TBranch        *b_SignalJetSD3JewelArea;   //!
   TBranch        *b_SignalJetSD3JewelSubjet1Pt;   //!
   TBranch        *b_SignalJetSD3JewelSubjet1Eta;   //!
   TBranch        *b_SignalJetSD3JewelSubjet1Phi;   //!
   TBranch        *b_SignalJetSD3JewelSubjet1M;   //!
   TBranch        *b_SignalJetSD3JewelSubjet1Area;   //!
   TBranch        *b_SignalJetSD3JewelSubjet2Pt;   //!
   TBranch        *b_SignalJetSD3JewelSubjet2Eta;   //!
   TBranch        *b_SignalJetSD3JewelSubjet2Phi;   //!
   TBranch        *b_SignalJetSD3JewelSubjet2M;   //!
   TBranch        *b_SignalJetSD3JewelSubjet2Area;   //!
   TBranch        *b_SignalJetSD3JewelDR12;   //!
   TBranch        *b_SignalJetSD3JewelZG;   //!
   TBranch        *b_SignalJetSD4JewelPt;   //!
   TBranch        *b_SignalJetSD4JewelEta;   //!
   TBranch        *b_SignalJetSD4JewelPhi;   //!
   TBranch        *b_SignalJetSD4JewelM;   //!
   TBranch        *b_SignalJetSD4JewelArea;   //!
   TBranch        *b_SignalJetSD4JewelSubjet1Pt;   //!
   TBranch        *b_SignalJetSD4JewelSubjet1Eta;   //!
   TBranch        *b_SignalJetSD4JewelSubjet1Phi;   //!
   TBranch        *b_SignalJetSD4JewelSubjet1M;   //!
   TBranch        *b_SignalJetSD4JewelSubjet1Area;   //!
   TBranch        *b_SignalJetSD4JewelSubjet2Pt;   //!
   TBranch        *b_SignalJetSD4JewelSubjet2Eta;   //!
   TBranch        *b_SignalJetSD4JewelSubjet2Phi;   //!
   TBranch        *b_SignalJetSD4JewelSubjet2M;   //!
   TBranch        *b_SignalJetSD4JewelSubjet2Area;   //!
   TBranch        *b_SignalJetSD4JewelDR12;   //!
   TBranch        *b_SignalJetSD4JewelZG;   //!
   TBranch        *b_SignalJetSD5JewelPt;   //!
   TBranch        *b_SignalJetSD5JewelEta;   //!
   TBranch        *b_SignalJetSD5JewelPhi;   //!
   TBranch        *b_SignalJetSD5JewelM;   //!
   TBranch        *b_SignalJetSD5JewelArea;   //!
   TBranch        *b_SignalJetSD5JewelSubjet1Pt;   //!
   TBranch        *b_SignalJetSD5JewelSubjet1Eta;   //!
   TBranch        *b_SignalJetSD5JewelSubjet1Phi;   //!
   TBranch        *b_SignalJetSD5JewelSubjet1M;   //!
   TBranch        *b_SignalJetSD5JewelSubjet1Area;   //!
   TBranch        *b_SignalJetSD5JewelSubjet2Pt;   //!
   TBranch        *b_SignalJetSD5JewelSubjet2Eta;   //!
   TBranch        *b_SignalJetSD5JewelSubjet2Phi;   //!
   TBranch        *b_SignalJetSD5JewelSubjet2M;   //!
   TBranch        *b_SignalJetSD5JewelSubjet2Area;   //!
   TBranch        *b_SignalJetSD5JewelDR12;   //!
   TBranch        *b_SignalJetSD5JewelZG;   //!
   TBranch        *b_EventWeight;   //!

   pythiaAnalysis(TTree *tree=0);
   virtual ~pythiaAnalysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef pythiaAnalysis_cxx
pythiaAnalysis::pythiaAnalysis(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data/kying/lundPlaneResults/pythia_cs_10000.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/data/kying/lundPlaneResults/pythia_cs_10000.root");
      }
      f->GetObject("JetTree",tree);

   }
   Init(tree);
}

pythiaAnalysis::~pythiaAnalysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pythiaAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pythiaAnalysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void pythiaAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   SignalJetPt = 0;
   SignalJetEta = 0;
   SignalJetPhi = 0;
   SignalJetM = 0;
   SignalJetArea = 0;
   SignalJetAKDRs = 0;
   SignalJetAKEta1s = 0;
   SignalJetAKEta2s = 0;
   SignalJetAKPT1s = 0;
   SignalJetAKPT2s = 0;
   SignalJetAKPhi1s = 0;
   SignalJetAKPhi2s = 0;
   SignalJetAKZGs = 0;
   SignalJetCAAKDRs = 0;
   SignalJetCAAKEta1s = 0;
   SignalJetCAAKEta2s = 0;
   SignalJetCAAKPT1s = 0;
   SignalJetCAAKPT2s = 0;
   SignalJetCAAKPhi1s = 0;
   SignalJetCAAKPhi2s = 0;
   SignalJetCAAKZGs = 0;
   SignalJetCADRs = 0;
   SignalJetCAEta1s = 0;
   SignalJetCAEta2s = 0;
   SignalJetCAKTDRs = 0;
   SignalJetCAKTEta1s = 0;
   SignalJetCAKTEta2s = 0;
   SignalJetCAKTPT1s = 0;
   SignalJetCAKTPT2s = 0;
   SignalJetCAKTPhi1s = 0;
   SignalJetCAKTPhi2s = 0;
   SignalJetCAKTZGs = 0;
   SignalJetCAPT1s = 0;
   SignalJetCAPT2s = 0;
   SignalJetCAPhi1s = 0;
   SignalJetCAPhi2s = 0;
   SignalJetCAZGs = 0;
   SignalJetKTDRs = 0;
   SignalJetKTEta1s = 0;
   SignalJetKTEta2s = 0;
   SignalJetKTPT1s = 0;
   SignalJetKTPT2s = 0;
   SignalJetKTPhi1s = 0;
   SignalJetKTPhi2s = 0;
   SignalJetKTZGs = 0;
   SignalJetJewelPt = 0;
   SignalJetJewelEta = 0;
   SignalJetJewelPhi = 0;
   SignalJetJewelM = 0;
   SignalJetJewelArea = 0;
   Rho = 0;
   RhoM = 0;
   SignalJetSD1Pt = 0;
   SignalJetSD1Eta = 0;
   SignalJetSD1Phi = 0;
   SignalJetSD1M = 0;
   SignalJetSD1Area = 0;
   SignalJetSD1Subjet1Pt = 0;
   SignalJetSD1Subjet1Eta = 0;
   SignalJetSD1Subjet1Phi = 0;
   SignalJetSD1Subjet1M = 0;
   SignalJetSD1Subjet1Area = 0;
   SignalJetSD1Subjet2Pt = 0;
   SignalJetSD1Subjet2Eta = 0;
   SignalJetSD1Subjet2Phi = 0;
   SignalJetSD1Subjet2M = 0;
   SignalJetSD1Subjet2Area = 0;
   SignalJetSD1DR12 = 0;
   SignalJetSD1ZG = 0;
   SignalJetSD1NBranch = 0;
   SignalJetSD2Pt = 0;
   SignalJetSD2Eta = 0;
   SignalJetSD2Phi = 0;
   SignalJetSD2M = 0;
   SignalJetSD2Area = 0;
   SignalJetSD2Subjet1Pt = 0;
   SignalJetSD2Subjet1Eta = 0;
   SignalJetSD2Subjet1Phi = 0;
   SignalJetSD2Subjet1M = 0;
   SignalJetSD2Subjet1Area = 0;
   SignalJetSD2Subjet2Pt = 0;
   SignalJetSD2Subjet2Eta = 0;
   SignalJetSD2Subjet2Phi = 0;
   SignalJetSD2Subjet2M = 0;
   SignalJetSD2Subjet2Area = 0;
   SignalJetSD2DR12 = 0;
   SignalJetSD2ZG = 0;
   SignalJetSD2NBranch = 0;
   SignalJetSD3Pt = 0;
   SignalJetSD3Eta = 0;
   SignalJetSD3Phi = 0;
   SignalJetSD3M = 0;
   SignalJetSD3Area = 0;
   SignalJetSD3Subjet1Pt = 0;
   SignalJetSD3Subjet1Eta = 0;
   SignalJetSD3Subjet1Phi = 0;
   SignalJetSD3Subjet1M = 0;
   SignalJetSD3Subjet1Area = 0;
   SignalJetSD3Subjet2Pt = 0;
   SignalJetSD3Subjet2Eta = 0;
   SignalJetSD3Subjet2Phi = 0;
   SignalJetSD3Subjet2M = 0;
   SignalJetSD3Subjet2Area = 0;
   SignalJetSD3DR12 = 0;
   SignalJetSD3ZG = 0;
   SignalJetSD3NBranch = 0;
   SignalJetSD4Pt = 0;
   SignalJetSD4Eta = 0;
   SignalJetSD4Phi = 0;
   SignalJetSD4M = 0;
   SignalJetSD4Area = 0;
   SignalJetSD4Subjet1Pt = 0;
   SignalJetSD4Subjet1Eta = 0;
   SignalJetSD4Subjet1Phi = 0;
   SignalJetSD4Subjet1M = 0;
   SignalJetSD4Subjet1Area = 0;
   SignalJetSD4Subjet2Pt = 0;
   SignalJetSD4Subjet2Eta = 0;
   SignalJetSD4Subjet2Phi = 0;
   SignalJetSD4Subjet2M = 0;
   SignalJetSD4Subjet2Area = 0;
   SignalJetSD4DR12 = 0;
   SignalJetSD4ZG = 0;
   SignalJetSD4NBranch = 0;
   SignalJetSD5Pt = 0;
   SignalJetSD5Eta = 0;
   SignalJetSD5Phi = 0;
   SignalJetSD5M = 0;
   SignalJetSD5Area = 0;
   SignalJetSD5Subjet1Pt = 0;
   SignalJetSD5Subjet1Eta = 0;
   SignalJetSD5Subjet1Phi = 0;
   SignalJetSD5Subjet1M = 0;
   SignalJetSD5Subjet1Area = 0;
   SignalJetSD5Subjet2Pt = 0;
   SignalJetSD5Subjet2Eta = 0;
   SignalJetSD5Subjet2Phi = 0;
   SignalJetSD5Subjet2M = 0;
   SignalJetSD5Subjet2Area = 0;
   SignalJetSD5DR12 = 0;
   SignalJetSD5ZG = 0;
   SignalJetSD5NBranch = 0;
   SignalJetSD1JewelPt = 0;
   SignalJetSD1JewelEta = 0;
   SignalJetSD1JewelPhi = 0;
   SignalJetSD1JewelM = 0;
   SignalJetSD1JewelArea = 0;
   SignalJetSD1JewelSubjet1Pt = 0;
   SignalJetSD1JewelSubjet1Eta = 0;
   SignalJetSD1JewelSubjet1Phi = 0;
   SignalJetSD1JewelSubjet1M = 0;
   SignalJetSD1JewelSubjet1Area = 0;
   SignalJetSD1JewelSubjet2Pt = 0;
   SignalJetSD1JewelSubjet2Eta = 0;
   SignalJetSD1JewelSubjet2Phi = 0;
   SignalJetSD1JewelSubjet2M = 0;
   SignalJetSD1JewelSubjet2Area = 0;
   SignalJetSD1JewelDR12 = 0;
   SignalJetSD1JewelZG = 0;
   SignalJetSD2JewelPt = 0;
   SignalJetSD2JewelEta = 0;
   SignalJetSD2JewelPhi = 0;
   SignalJetSD2JewelM = 0;
   SignalJetSD2JewelArea = 0;
   SignalJetSD2JewelSubjet1Pt = 0;
   SignalJetSD2JewelSubjet1Eta = 0;
   SignalJetSD2JewelSubjet1Phi = 0;
   SignalJetSD2JewelSubjet1M = 0;
   SignalJetSD2JewelSubjet1Area = 0;
   SignalJetSD2JewelSubjet2Pt = 0;
   SignalJetSD2JewelSubjet2Eta = 0;
   SignalJetSD2JewelSubjet2Phi = 0;
   SignalJetSD2JewelSubjet2M = 0;
   SignalJetSD2JewelSubjet2Area = 0;
   SignalJetSD2JewelDR12 = 0;
   SignalJetSD2JewelZG = 0;
   SignalJetSD3JewelPt = 0;
   SignalJetSD3JewelEta = 0;
   SignalJetSD3JewelPhi = 0;
   SignalJetSD3JewelM = 0;
   SignalJetSD3JewelArea = 0;
   SignalJetSD3JewelSubjet1Pt = 0;
   SignalJetSD3JewelSubjet1Eta = 0;
   SignalJetSD3JewelSubjet1Phi = 0;
   SignalJetSD3JewelSubjet1M = 0;
   SignalJetSD3JewelSubjet1Area = 0;
   SignalJetSD3JewelSubjet2Pt = 0;
   SignalJetSD3JewelSubjet2Eta = 0;
   SignalJetSD3JewelSubjet2Phi = 0;
   SignalJetSD3JewelSubjet2M = 0;
   SignalJetSD3JewelSubjet2Area = 0;
   SignalJetSD3JewelDR12 = 0;
   SignalJetSD3JewelZG = 0;
   SignalJetSD4JewelPt = 0;
   SignalJetSD4JewelEta = 0;
   SignalJetSD4JewelPhi = 0;
   SignalJetSD4JewelM = 0;
   SignalJetSD4JewelArea = 0;
   SignalJetSD4JewelSubjet1Pt = 0;
   SignalJetSD4JewelSubjet1Eta = 0;
   SignalJetSD4JewelSubjet1Phi = 0;
   SignalJetSD4JewelSubjet1M = 0;
   SignalJetSD4JewelSubjet1Area = 0;
   SignalJetSD4JewelSubjet2Pt = 0;
   SignalJetSD4JewelSubjet2Eta = 0;
   SignalJetSD4JewelSubjet2Phi = 0;
   SignalJetSD4JewelSubjet2M = 0;
   SignalJetSD4JewelSubjet2Area = 0;
   SignalJetSD4JewelDR12 = 0;
   SignalJetSD4JewelZG = 0;
   SignalJetSD5JewelPt = 0;
   SignalJetSD5JewelEta = 0;
   SignalJetSD5JewelPhi = 0;
   SignalJetSD5JewelM = 0;
   SignalJetSD5JewelArea = 0;
   SignalJetSD5JewelSubjet1Pt = 0;
   SignalJetSD5JewelSubjet1Eta = 0;
   SignalJetSD5JewelSubjet1Phi = 0;
   SignalJetSD5JewelSubjet1M = 0;
   SignalJetSD5JewelSubjet1Area = 0;
   SignalJetSD5JewelSubjet2Pt = 0;
   SignalJetSD5JewelSubjet2Eta = 0;
   SignalJetSD5JewelSubjet2Phi = 0;
   SignalJetSD5JewelSubjet2M = 0;
   SignalJetSD5JewelSubjet2Area = 0;
   SignalJetSD5JewelDR12 = 0;
   SignalJetSD5JewelZG = 0;
   EventWeight = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("SignalJetPt", &SignalJetPt, &b_SignalJetPt);
   fChain->SetBranchAddress("SignalJetEta", &SignalJetEta, &b_SignalJetEta);
   fChain->SetBranchAddress("SignalJetPhi", &SignalJetPhi, &b_SignalJetPhi);
   fChain->SetBranchAddress("SignalJetM", &SignalJetM, &b_SignalJetM);
   fChain->SetBranchAddress("SignalJetArea", &SignalJetArea, &b_SignalJetArea);
   fChain->SetBranchAddress("SignalJetAKDRs", &SignalJetAKDRs, &b_SignalJetAKDRs);
   fChain->SetBranchAddress("SignalJetAKEta1s", &SignalJetAKEta1s, &b_SignalJetAKEta1s);
   fChain->SetBranchAddress("SignalJetAKEta2s", &SignalJetAKEta2s, &b_SignalJetAKEta2s);
   fChain->SetBranchAddress("SignalJetAKPT1s", &SignalJetAKPT1s, &b_SignalJetAKPT1s);
   fChain->SetBranchAddress("SignalJetAKPT2s", &SignalJetAKPT2s, &b_SignalJetAKPT2s);
   fChain->SetBranchAddress("SignalJetAKPhi1s", &SignalJetAKPhi1s, &b_SignalJetAKPhi1s);
   fChain->SetBranchAddress("SignalJetAKPhi2s", &SignalJetAKPhi2s, &b_SignalJetAKPhi2s);
   fChain->SetBranchAddress("SignalJetAKZGs", &SignalJetAKZGs, &b_SignalJetAKZGs);
   fChain->SetBranchAddress("SignalJetCAAKDRs", &SignalJetCAAKDRs, &b_SignalJetCAAKDRs);
   fChain->SetBranchAddress("SignalJetCAAKEta1s", &SignalJetCAAKEta1s, &b_SignalJetCAAKEta1s);
   fChain->SetBranchAddress("SignalJetCAAKEta2s", &SignalJetCAAKEta2s, &b_SignalJetCAAKEta2s);
   fChain->SetBranchAddress("SignalJetCAAKPT1s", &SignalJetCAAKPT1s, &b_SignalJetCAAKPT1s);
   fChain->SetBranchAddress("SignalJetCAAKPT2s", &SignalJetCAAKPT2s, &b_SignalJetCAAKPT2s);
   fChain->SetBranchAddress("SignalJetCAAKPhi1s", &SignalJetCAAKPhi1s, &b_SignalJetCAAKPhi1s);
   fChain->SetBranchAddress("SignalJetCAAKPhi2s", &SignalJetCAAKPhi2s, &b_SignalJetCAAKPhi2s);
   fChain->SetBranchAddress("SignalJetCAAKZGs", &SignalJetCAAKZGs, &b_SignalJetCAAKZGs);
   fChain->SetBranchAddress("SignalJetCADRs", &SignalJetCADRs, &b_SignalJetCADRs);
   fChain->SetBranchAddress("SignalJetCAEta1s", &SignalJetCAEta1s, &b_SignalJetCAEta1s);
   fChain->SetBranchAddress("SignalJetCAEta2s", &SignalJetCAEta2s, &b_SignalJetCAEta2s);
   fChain->SetBranchAddress("SignalJetCAKTDRs", &SignalJetCAKTDRs, &b_SignalJetCAKTDRs);
   fChain->SetBranchAddress("SignalJetCAKTEta1s", &SignalJetCAKTEta1s, &b_SignalJetCAKTEta1s);
   fChain->SetBranchAddress("SignalJetCAKTEta2s", &SignalJetCAKTEta2s, &b_SignalJetCAKTEta2s);
   fChain->SetBranchAddress("SignalJetCAKTPT1s", &SignalJetCAKTPT1s, &b_SignalJetCAKTPT1s);
   fChain->SetBranchAddress("SignalJetCAKTPT2s", &SignalJetCAKTPT2s, &b_SignalJetCAKTPT2s);
   fChain->SetBranchAddress("SignalJetCAKTPhi1s", &SignalJetCAKTPhi1s, &b_SignalJetCAKTPhi1s);
   fChain->SetBranchAddress("SignalJetCAKTPhi2s", &SignalJetCAKTPhi2s, &b_SignalJetCAKTPhi2s);
   fChain->SetBranchAddress("SignalJetCAKTZGs", &SignalJetCAKTZGs, &b_SignalJetCAKTZGs);
   fChain->SetBranchAddress("SignalJetCAPT1s", &SignalJetCAPT1s, &b_SignalJetCAPT1s);
   fChain->SetBranchAddress("SignalJetCAPT2s", &SignalJetCAPT2s, &b_SignalJetCAPT2s);
   fChain->SetBranchAddress("SignalJetCAPhi1s", &SignalJetCAPhi1s, &b_SignalJetCAPhi1s);
   fChain->SetBranchAddress("SignalJetCAPhi2s", &SignalJetCAPhi2s, &b_SignalJetCAPhi2s);
   fChain->SetBranchAddress("SignalJetCAZGs", &SignalJetCAZGs, &b_SignalJetCAZGs);
   fChain->SetBranchAddress("SignalJetKTDRs", &SignalJetKTDRs, &b_SignalJetKTDRs);
   fChain->SetBranchAddress("SignalJetKTEta1s", &SignalJetKTEta1s, &b_SignalJetKTEta1s);
   fChain->SetBranchAddress("SignalJetKTEta2s", &SignalJetKTEta2s, &b_SignalJetKTEta2s);
   fChain->SetBranchAddress("SignalJetKTPT1s", &SignalJetKTPT1s, &b_SignalJetKTPT1s);
   fChain->SetBranchAddress("SignalJetKTPT2s", &SignalJetKTPT2s, &b_SignalJetKTPT2s);
   fChain->SetBranchAddress("SignalJetKTPhi1s", &SignalJetKTPhi1s, &b_SignalJetKTPhi1s);
   fChain->SetBranchAddress("SignalJetKTPhi2s", &SignalJetKTPhi2s, &b_SignalJetKTPhi2s);
   fChain->SetBranchAddress("SignalJetKTZGs", &SignalJetKTZGs, &b_SignalJetKTZGs);
   fChain->SetBranchAddress("SignalJetJewelPt", &SignalJetJewelPt, &b_SignalJetJewelPt);
   fChain->SetBranchAddress("SignalJetJewelEta", &SignalJetJewelEta, &b_SignalJetJewelEta);
   fChain->SetBranchAddress("SignalJetJewelPhi", &SignalJetJewelPhi, &b_SignalJetJewelPhi);
   fChain->SetBranchAddress("SignalJetJewelM", &SignalJetJewelM, &b_SignalJetJewelM);
   fChain->SetBranchAddress("SignalJetJewelArea", &SignalJetJewelArea, &b_SignalJetJewelArea);
   fChain->SetBranchAddress("Rho", &Rho, &b_Rho);
   fChain->SetBranchAddress("RhoM", &RhoM, &b_RhoM);
   fChain->SetBranchAddress("SignalJetSD1Pt", &SignalJetSD1Pt, &b_SignalJetSD1Pt);
   fChain->SetBranchAddress("SignalJetSD1Eta", &SignalJetSD1Eta, &b_SignalJetSD1Eta);
   fChain->SetBranchAddress("SignalJetSD1Phi", &SignalJetSD1Phi, &b_SignalJetSD1Phi);
   fChain->SetBranchAddress("SignalJetSD1M", &SignalJetSD1M, &b_SignalJetSD1M);
   fChain->SetBranchAddress("SignalJetSD1Area", &SignalJetSD1Area, &b_SignalJetSD1Area);
   fChain->SetBranchAddress("SignalJetSD1Subjet1Pt", &SignalJetSD1Subjet1Pt, &b_SignalJetSD1Subjet1Pt);
   fChain->SetBranchAddress("SignalJetSD1Subjet1Eta", &SignalJetSD1Subjet1Eta, &b_SignalJetSD1Subjet1Eta);
   fChain->SetBranchAddress("SignalJetSD1Subjet1Phi", &SignalJetSD1Subjet1Phi, &b_SignalJetSD1Subjet1Phi);
   fChain->SetBranchAddress("SignalJetSD1Subjet1M", &SignalJetSD1Subjet1M, &b_SignalJetSD1Subjet1M);
   fChain->SetBranchAddress("SignalJetSD1Subjet1Area", &SignalJetSD1Subjet1Area, &b_SignalJetSD1Subjet1Area);
   fChain->SetBranchAddress("SignalJetSD1Subjet2Pt", &SignalJetSD1Subjet2Pt, &b_SignalJetSD1Subjet2Pt);
   fChain->SetBranchAddress("SignalJetSD1Subjet2Eta", &SignalJetSD1Subjet2Eta, &b_SignalJetSD1Subjet2Eta);
   fChain->SetBranchAddress("SignalJetSD1Subjet2Phi", &SignalJetSD1Subjet2Phi, &b_SignalJetSD1Subjet2Phi);
   fChain->SetBranchAddress("SignalJetSD1Subjet2M", &SignalJetSD1Subjet2M, &b_SignalJetSD1Subjet2M);
   fChain->SetBranchAddress("SignalJetSD1Subjet2Area", &SignalJetSD1Subjet2Area, &b_SignalJetSD1Subjet2Area);
   fChain->SetBranchAddress("SignalJetSD1DR12", &SignalJetSD1DR12, &b_SignalJetSD1DR12);
   fChain->SetBranchAddress("SignalJetSD1ZG", &SignalJetSD1ZG, &b_SignalJetSD1ZG);
   fChain->SetBranchAddress("SignalJetSD1NBranch", &SignalJetSD1NBranch, &b_SignalJetSD1NBranch);
   fChain->SetBranchAddress("SignalJetSD2Pt", &SignalJetSD2Pt, &b_SignalJetSD2Pt);
   fChain->SetBranchAddress("SignalJetSD2Eta", &SignalJetSD2Eta, &b_SignalJetSD2Eta);
   fChain->SetBranchAddress("SignalJetSD2Phi", &SignalJetSD2Phi, &b_SignalJetSD2Phi);
   fChain->SetBranchAddress("SignalJetSD2M", &SignalJetSD2M, &b_SignalJetSD2M);
   fChain->SetBranchAddress("SignalJetSD2Area", &SignalJetSD2Area, &b_SignalJetSD2Area);
   fChain->SetBranchAddress("SignalJetSD2Subjet1Pt", &SignalJetSD2Subjet1Pt, &b_SignalJetSD2Subjet1Pt);
   fChain->SetBranchAddress("SignalJetSD2Subjet1Eta", &SignalJetSD2Subjet1Eta, &b_SignalJetSD2Subjet1Eta);
   fChain->SetBranchAddress("SignalJetSD2Subjet1Phi", &SignalJetSD2Subjet1Phi, &b_SignalJetSD2Subjet1Phi);
   fChain->SetBranchAddress("SignalJetSD2Subjet1M", &SignalJetSD2Subjet1M, &b_SignalJetSD2Subjet1M);
   fChain->SetBranchAddress("SignalJetSD2Subjet1Area", &SignalJetSD2Subjet1Area, &b_SignalJetSD2Subjet1Area);
   fChain->SetBranchAddress("SignalJetSD2Subjet2Pt", &SignalJetSD2Subjet2Pt, &b_SignalJetSD2Subjet2Pt);
   fChain->SetBranchAddress("SignalJetSD2Subjet2Eta", &SignalJetSD2Subjet2Eta, &b_SignalJetSD2Subjet2Eta);
   fChain->SetBranchAddress("SignalJetSD2Subjet2Phi", &SignalJetSD2Subjet2Phi, &b_SignalJetSD2Subjet2Phi);
   fChain->SetBranchAddress("SignalJetSD2Subjet2M", &SignalJetSD2Subjet2M, &b_SignalJetSD2Subjet2M);
   fChain->SetBranchAddress("SignalJetSD2Subjet2Area", &SignalJetSD2Subjet2Area, &b_SignalJetSD2Subjet2Area);
   fChain->SetBranchAddress("SignalJetSD2DR12", &SignalJetSD2DR12, &b_SignalJetSD2DR12);
   fChain->SetBranchAddress("SignalJetSD2ZG", &SignalJetSD2ZG, &b_SignalJetSD2ZG);
   fChain->SetBranchAddress("SignalJetSD2NBranch", &SignalJetSD2NBranch, &b_SignalJetSD2NBranch);
   fChain->SetBranchAddress("SignalJetSD3Pt", &SignalJetSD3Pt, &b_SignalJetSD3Pt);
   fChain->SetBranchAddress("SignalJetSD3Eta", &SignalJetSD3Eta, &b_SignalJetSD3Eta);
   fChain->SetBranchAddress("SignalJetSD3Phi", &SignalJetSD3Phi, &b_SignalJetSD3Phi);
   fChain->SetBranchAddress("SignalJetSD3M", &SignalJetSD3M, &b_SignalJetSD3M);
   fChain->SetBranchAddress("SignalJetSD3Area", &SignalJetSD3Area, &b_SignalJetSD3Area);
   fChain->SetBranchAddress("SignalJetSD3Subjet1Pt", &SignalJetSD3Subjet1Pt, &b_SignalJetSD3Subjet1Pt);
   fChain->SetBranchAddress("SignalJetSD3Subjet1Eta", &SignalJetSD3Subjet1Eta, &b_SignalJetSD3Subjet1Eta);
   fChain->SetBranchAddress("SignalJetSD3Subjet1Phi", &SignalJetSD3Subjet1Phi, &b_SignalJetSD3Subjet1Phi);
   fChain->SetBranchAddress("SignalJetSD3Subjet1M", &SignalJetSD3Subjet1M, &b_SignalJetSD3Subjet1M);
   fChain->SetBranchAddress("SignalJetSD3Subjet1Area", &SignalJetSD3Subjet1Area, &b_SignalJetSD3Subjet1Area);
   fChain->SetBranchAddress("SignalJetSD3Subjet2Pt", &SignalJetSD3Subjet2Pt, &b_SignalJetSD3Subjet2Pt);
   fChain->SetBranchAddress("SignalJetSD3Subjet2Eta", &SignalJetSD3Subjet2Eta, &b_SignalJetSD3Subjet2Eta);
   fChain->SetBranchAddress("SignalJetSD3Subjet2Phi", &SignalJetSD3Subjet2Phi, &b_SignalJetSD3Subjet2Phi);
   fChain->SetBranchAddress("SignalJetSD3Subjet2M", &SignalJetSD3Subjet2M, &b_SignalJetSD3Subjet2M);
   fChain->SetBranchAddress("SignalJetSD3Subjet2Area", &SignalJetSD3Subjet2Area, &b_SignalJetSD3Subjet2Area);
   fChain->SetBranchAddress("SignalJetSD3DR12", &SignalJetSD3DR12, &b_SignalJetSD3DR12);
   fChain->SetBranchAddress("SignalJetSD3ZG", &SignalJetSD3ZG, &b_SignalJetSD3ZG);
   fChain->SetBranchAddress("SignalJetSD3NBranch", &SignalJetSD3NBranch, &b_SignalJetSD3NBranch);
   fChain->SetBranchAddress("SignalJetSD4Pt", &SignalJetSD4Pt, &b_SignalJetSD4Pt);
   fChain->SetBranchAddress("SignalJetSD4Eta", &SignalJetSD4Eta, &b_SignalJetSD4Eta);
   fChain->SetBranchAddress("SignalJetSD4Phi", &SignalJetSD4Phi, &b_SignalJetSD4Phi);
   fChain->SetBranchAddress("SignalJetSD4M", &SignalJetSD4M, &b_SignalJetSD4M);
   fChain->SetBranchAddress("SignalJetSD4Area", &SignalJetSD4Area, &b_SignalJetSD4Area);
   fChain->SetBranchAddress("SignalJetSD4Subjet1Pt", &SignalJetSD4Subjet1Pt, &b_SignalJetSD4Subjet1Pt);
   fChain->SetBranchAddress("SignalJetSD4Subjet1Eta", &SignalJetSD4Subjet1Eta, &b_SignalJetSD4Subjet1Eta);
   fChain->SetBranchAddress("SignalJetSD4Subjet1Phi", &SignalJetSD4Subjet1Phi, &b_SignalJetSD4Subjet1Phi);
   fChain->SetBranchAddress("SignalJetSD4Subjet1M", &SignalJetSD4Subjet1M, &b_SignalJetSD4Subjet1M);
   fChain->SetBranchAddress("SignalJetSD4Subjet1Area", &SignalJetSD4Subjet1Area, &b_SignalJetSD4Subjet1Area);
   fChain->SetBranchAddress("SignalJetSD4Subjet2Pt", &SignalJetSD4Subjet2Pt, &b_SignalJetSD4Subjet2Pt);
   fChain->SetBranchAddress("SignalJetSD4Subjet2Eta", &SignalJetSD4Subjet2Eta, &b_SignalJetSD4Subjet2Eta);
   fChain->SetBranchAddress("SignalJetSD4Subjet2Phi", &SignalJetSD4Subjet2Phi, &b_SignalJetSD4Subjet2Phi);
   fChain->SetBranchAddress("SignalJetSD4Subjet2M", &SignalJetSD4Subjet2M, &b_SignalJetSD4Subjet2M);
   fChain->SetBranchAddress("SignalJetSD4Subjet2Area", &SignalJetSD4Subjet2Area, &b_SignalJetSD4Subjet2Area);
   fChain->SetBranchAddress("SignalJetSD4DR12", &SignalJetSD4DR12, &b_SignalJetSD4DR12);
   fChain->SetBranchAddress("SignalJetSD4ZG", &SignalJetSD4ZG, &b_SignalJetSD4ZG);
   fChain->SetBranchAddress("SignalJetSD4NBranch", &SignalJetSD4NBranch, &b_SignalJetSD4NBranch);
   fChain->SetBranchAddress("SignalJetSD5Pt", &SignalJetSD5Pt, &b_SignalJetSD5Pt);
   fChain->SetBranchAddress("SignalJetSD5Eta", &SignalJetSD5Eta, &b_SignalJetSD5Eta);
   fChain->SetBranchAddress("SignalJetSD5Phi", &SignalJetSD5Phi, &b_SignalJetSD5Phi);
   fChain->SetBranchAddress("SignalJetSD5M", &SignalJetSD5M, &b_SignalJetSD5M);
   fChain->SetBranchAddress("SignalJetSD5Area", &SignalJetSD5Area, &b_SignalJetSD5Area);
   fChain->SetBranchAddress("SignalJetSD5Subjet1Pt", &SignalJetSD5Subjet1Pt, &b_SignalJetSD5Subjet1Pt);
   fChain->SetBranchAddress("SignalJetSD5Subjet1Eta", &SignalJetSD5Subjet1Eta, &b_SignalJetSD5Subjet1Eta);
   fChain->SetBranchAddress("SignalJetSD5Subjet1Phi", &SignalJetSD5Subjet1Phi, &b_SignalJetSD5Subjet1Phi);
   fChain->SetBranchAddress("SignalJetSD5Subjet1M", &SignalJetSD5Subjet1M, &b_SignalJetSD5Subjet1M);
   fChain->SetBranchAddress("SignalJetSD5Subjet1Area", &SignalJetSD5Subjet1Area, &b_SignalJetSD5Subjet1Area);
   fChain->SetBranchAddress("SignalJetSD5Subjet2Pt", &SignalJetSD5Subjet2Pt, &b_SignalJetSD5Subjet2Pt);
   fChain->SetBranchAddress("SignalJetSD5Subjet2Eta", &SignalJetSD5Subjet2Eta, &b_SignalJetSD5Subjet2Eta);
   fChain->SetBranchAddress("SignalJetSD5Subjet2Phi", &SignalJetSD5Subjet2Phi, &b_SignalJetSD5Subjet2Phi);
   fChain->SetBranchAddress("SignalJetSD5Subjet2M", &SignalJetSD5Subjet2M, &b_SignalJetSD5Subjet2M);
   fChain->SetBranchAddress("SignalJetSD5Subjet2Area", &SignalJetSD5Subjet2Area, &b_SignalJetSD5Subjet2Area);
   fChain->SetBranchAddress("SignalJetSD5DR12", &SignalJetSD5DR12, &b_SignalJetSD5DR12);
   fChain->SetBranchAddress("SignalJetSD5ZG", &SignalJetSD5ZG, &b_SignalJetSD5ZG);
   fChain->SetBranchAddress("SignalJetSD5NBranch", &SignalJetSD5NBranch, &b_SignalJetSD5NBranch);
   fChain->SetBranchAddress("SignalJetSD1JewelPt", &SignalJetSD1JewelPt, &b_SignalJetSD1JewelPt);
   fChain->SetBranchAddress("SignalJetSD1JewelEta", &SignalJetSD1JewelEta, &b_SignalJetSD1JewelEta);
   fChain->SetBranchAddress("SignalJetSD1JewelPhi", &SignalJetSD1JewelPhi, &b_SignalJetSD1JewelPhi);
   fChain->SetBranchAddress("SignalJetSD1JewelM", &SignalJetSD1JewelM, &b_SignalJetSD1JewelM);
   fChain->SetBranchAddress("SignalJetSD1JewelArea", &SignalJetSD1JewelArea, &b_SignalJetSD1JewelArea);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet1Pt", &SignalJetSD1JewelSubjet1Pt, &b_SignalJetSD1JewelSubjet1Pt);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet1Eta", &SignalJetSD1JewelSubjet1Eta, &b_SignalJetSD1JewelSubjet1Eta);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet1Phi", &SignalJetSD1JewelSubjet1Phi, &b_SignalJetSD1JewelSubjet1Phi);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet1M", &SignalJetSD1JewelSubjet1M, &b_SignalJetSD1JewelSubjet1M);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet1Area", &SignalJetSD1JewelSubjet1Area, &b_SignalJetSD1JewelSubjet1Area);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet2Pt", &SignalJetSD1JewelSubjet2Pt, &b_SignalJetSD1JewelSubjet2Pt);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet2Eta", &SignalJetSD1JewelSubjet2Eta, &b_SignalJetSD1JewelSubjet2Eta);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet2Phi", &SignalJetSD1JewelSubjet2Phi, &b_SignalJetSD1JewelSubjet2Phi);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet2M", &SignalJetSD1JewelSubjet2M, &b_SignalJetSD1JewelSubjet2M);
   fChain->SetBranchAddress("SignalJetSD1JewelSubjet2Area", &SignalJetSD1JewelSubjet2Area, &b_SignalJetSD1JewelSubjet2Area);
   fChain->SetBranchAddress("SignalJetSD1JewelDR12", &SignalJetSD1JewelDR12, &b_SignalJetSD1JewelDR12);
   fChain->SetBranchAddress("SignalJetSD1JewelZG", &SignalJetSD1JewelZG, &b_SignalJetSD1JewelZG);
   fChain->SetBranchAddress("SignalJetSD2JewelPt", &SignalJetSD2JewelPt, &b_SignalJetSD2JewelPt);
   fChain->SetBranchAddress("SignalJetSD2JewelEta", &SignalJetSD2JewelEta, &b_SignalJetSD2JewelEta);
   fChain->SetBranchAddress("SignalJetSD2JewelPhi", &SignalJetSD2JewelPhi, &b_SignalJetSD2JewelPhi);
   fChain->SetBranchAddress("SignalJetSD2JewelM", &SignalJetSD2JewelM, &b_SignalJetSD2JewelM);
   fChain->SetBranchAddress("SignalJetSD2JewelArea", &SignalJetSD2JewelArea, &b_SignalJetSD2JewelArea);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet1Pt", &SignalJetSD2JewelSubjet1Pt, &b_SignalJetSD2JewelSubjet1Pt);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet1Eta", &SignalJetSD2JewelSubjet1Eta, &b_SignalJetSD2JewelSubjet1Eta);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet1Phi", &SignalJetSD2JewelSubjet1Phi, &b_SignalJetSD2JewelSubjet1Phi);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet1M", &SignalJetSD2JewelSubjet1M, &b_SignalJetSD2JewelSubjet1M);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet1Area", &SignalJetSD2JewelSubjet1Area, &b_SignalJetSD2JewelSubjet1Area);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet2Pt", &SignalJetSD2JewelSubjet2Pt, &b_SignalJetSD2JewelSubjet2Pt);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet2Eta", &SignalJetSD2JewelSubjet2Eta, &b_SignalJetSD2JewelSubjet2Eta);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet2Phi", &SignalJetSD2JewelSubjet2Phi, &b_SignalJetSD2JewelSubjet2Phi);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet2M", &SignalJetSD2JewelSubjet2M, &b_SignalJetSD2JewelSubjet2M);
   fChain->SetBranchAddress("SignalJetSD2JewelSubjet2Area", &SignalJetSD2JewelSubjet2Area, &b_SignalJetSD2JewelSubjet2Area);
   fChain->SetBranchAddress("SignalJetSD2JewelDR12", &SignalJetSD2JewelDR12, &b_SignalJetSD2JewelDR12);
   fChain->SetBranchAddress("SignalJetSD2JewelZG", &SignalJetSD2JewelZG, &b_SignalJetSD2JewelZG);
   fChain->SetBranchAddress("SignalJetSD3JewelPt", &SignalJetSD3JewelPt, &b_SignalJetSD3JewelPt);
   fChain->SetBranchAddress("SignalJetSD3JewelEta", &SignalJetSD3JewelEta, &b_SignalJetSD3JewelEta);
   fChain->SetBranchAddress("SignalJetSD3JewelPhi", &SignalJetSD3JewelPhi, &b_SignalJetSD3JewelPhi);
   fChain->SetBranchAddress("SignalJetSD3JewelM", &SignalJetSD3JewelM, &b_SignalJetSD3JewelM);
   fChain->SetBranchAddress("SignalJetSD3JewelArea", &SignalJetSD3JewelArea, &b_SignalJetSD3JewelArea);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet1Pt", &SignalJetSD3JewelSubjet1Pt, &b_SignalJetSD3JewelSubjet1Pt);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet1Eta", &SignalJetSD3JewelSubjet1Eta, &b_SignalJetSD3JewelSubjet1Eta);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet1Phi", &SignalJetSD3JewelSubjet1Phi, &b_SignalJetSD3JewelSubjet1Phi);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet1M", &SignalJetSD3JewelSubjet1M, &b_SignalJetSD3JewelSubjet1M);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet1Area", &SignalJetSD3JewelSubjet1Area, &b_SignalJetSD3JewelSubjet1Area);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet2Pt", &SignalJetSD3JewelSubjet2Pt, &b_SignalJetSD3JewelSubjet2Pt);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet2Eta", &SignalJetSD3JewelSubjet2Eta, &b_SignalJetSD3JewelSubjet2Eta);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet2Phi", &SignalJetSD3JewelSubjet2Phi, &b_SignalJetSD3JewelSubjet2Phi);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet2M", &SignalJetSD3JewelSubjet2M, &b_SignalJetSD3JewelSubjet2M);
   fChain->SetBranchAddress("SignalJetSD3JewelSubjet2Area", &SignalJetSD3JewelSubjet2Area, &b_SignalJetSD3JewelSubjet2Area);
   fChain->SetBranchAddress("SignalJetSD3JewelDR12", &SignalJetSD3JewelDR12, &b_SignalJetSD3JewelDR12);
   fChain->SetBranchAddress("SignalJetSD3JewelZG", &SignalJetSD3JewelZG, &b_SignalJetSD3JewelZG);
   fChain->SetBranchAddress("SignalJetSD4JewelPt", &SignalJetSD4JewelPt, &b_SignalJetSD4JewelPt);
   fChain->SetBranchAddress("SignalJetSD4JewelEta", &SignalJetSD4JewelEta, &b_SignalJetSD4JewelEta);
   fChain->SetBranchAddress("SignalJetSD4JewelPhi", &SignalJetSD4JewelPhi, &b_SignalJetSD4JewelPhi);
   fChain->SetBranchAddress("SignalJetSD4JewelM", &SignalJetSD4JewelM, &b_SignalJetSD4JewelM);
   fChain->SetBranchAddress("SignalJetSD4JewelArea", &SignalJetSD4JewelArea, &b_SignalJetSD4JewelArea);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet1Pt", &SignalJetSD4JewelSubjet1Pt, &b_SignalJetSD4JewelSubjet1Pt);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet1Eta", &SignalJetSD4JewelSubjet1Eta, &b_SignalJetSD4JewelSubjet1Eta);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet1Phi", &SignalJetSD4JewelSubjet1Phi, &b_SignalJetSD4JewelSubjet1Phi);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet1M", &SignalJetSD4JewelSubjet1M, &b_SignalJetSD4JewelSubjet1M);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet1Area", &SignalJetSD4JewelSubjet1Area, &b_SignalJetSD4JewelSubjet1Area);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet2Pt", &SignalJetSD4JewelSubjet2Pt, &b_SignalJetSD4JewelSubjet2Pt);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet2Eta", &SignalJetSD4JewelSubjet2Eta, &b_SignalJetSD4JewelSubjet2Eta);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet2Phi", &SignalJetSD4JewelSubjet2Phi, &b_SignalJetSD4JewelSubjet2Phi);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet2M", &SignalJetSD4JewelSubjet2M, &b_SignalJetSD4JewelSubjet2M);
   fChain->SetBranchAddress("SignalJetSD4JewelSubjet2Area", &SignalJetSD4JewelSubjet2Area, &b_SignalJetSD4JewelSubjet2Area);
   fChain->SetBranchAddress("SignalJetSD4JewelDR12", &SignalJetSD4JewelDR12, &b_SignalJetSD4JewelDR12);
   fChain->SetBranchAddress("SignalJetSD4JewelZG", &SignalJetSD4JewelZG, &b_SignalJetSD4JewelZG);
   fChain->SetBranchAddress("SignalJetSD5JewelPt", &SignalJetSD5JewelPt, &b_SignalJetSD5JewelPt);
   fChain->SetBranchAddress("SignalJetSD5JewelEta", &SignalJetSD5JewelEta, &b_SignalJetSD5JewelEta);
   fChain->SetBranchAddress("SignalJetSD5JewelPhi", &SignalJetSD5JewelPhi, &b_SignalJetSD5JewelPhi);
   fChain->SetBranchAddress("SignalJetSD5JewelM", &SignalJetSD5JewelM, &b_SignalJetSD5JewelM);
   fChain->SetBranchAddress("SignalJetSD5JewelArea", &SignalJetSD5JewelArea, &b_SignalJetSD5JewelArea);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet1Pt", &SignalJetSD5JewelSubjet1Pt, &b_SignalJetSD5JewelSubjet1Pt);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet1Eta", &SignalJetSD5JewelSubjet1Eta, &b_SignalJetSD5JewelSubjet1Eta);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet1Phi", &SignalJetSD5JewelSubjet1Phi, &b_SignalJetSD5JewelSubjet1Phi);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet1M", &SignalJetSD5JewelSubjet1M, &b_SignalJetSD5JewelSubjet1M);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet1Area", &SignalJetSD5JewelSubjet1Area, &b_SignalJetSD5JewelSubjet1Area);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet2Pt", &SignalJetSD5JewelSubjet2Pt, &b_SignalJetSD5JewelSubjet2Pt);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet2Eta", &SignalJetSD5JewelSubjet2Eta, &b_SignalJetSD5JewelSubjet2Eta);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet2Phi", &SignalJetSD5JewelSubjet2Phi, &b_SignalJetSD5JewelSubjet2Phi);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet2M", &SignalJetSD5JewelSubjet2M, &b_SignalJetSD5JewelSubjet2M);
   fChain->SetBranchAddress("SignalJetSD5JewelSubjet2Area", &SignalJetSD5JewelSubjet2Area, &b_SignalJetSD5JewelSubjet2Area);
   fChain->SetBranchAddress("SignalJetSD5JewelDR12", &SignalJetSD5JewelDR12, &b_SignalJetSD5JewelDR12);
   fChain->SetBranchAddress("SignalJetSD5JewelZG", &SignalJetSD5JewelZG, &b_SignalJetSD5JewelZG);
   fChain->SetBranchAddress("EventWeight", &EventWeight, &b_EventWeight);
   Notify();
}

Bool_t pythiaAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pythiaAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pythiaAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef pythiaAnalysis_cxx
