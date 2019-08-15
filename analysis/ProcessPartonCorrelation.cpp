#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TChain.h"
#include "TGraph.h"
#include "TFile.h"

#include "CommandLine.h"
#include "SetStyle.h"

int main(int argc, char *argv[]);
double GetDR(double eta1, double phi1, double eta2, double phi2);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   int JetPTCut = CL.GetInt("JetPTCut", 350);
   string Algo = CL.Get("Algo", "KT");
   string Title = CL.Get("Title", "");
   string OutputBase = CL.Get("OutputBase", "Correlation");

   vector<string> FileNames = CL.GetStringVector("InputFiles");

   TChain Tree("JetTree", "JetTree");

   for(string File : FileNames)
      Tree.AddFile(File.c_str());


   int DoZG = CL.GetInt("DoZG", 0);

   vector<double> *SignalJetPt = nullptr;
   vector<double> *SignalJetEta = nullptr;
   vector<double> *SignalJetPhi = nullptr;

   vector<vector<double>> *SignalJetAKDRs = nullptr;
   vector<vector<double>> *SignalJetAKPT2s = nullptr;
   vector<vector<double>> *SignalJetCADRs = nullptr;
   vector<vector<double>> *SignalJetCAPT2s = nullptr;
   vector<vector<double>> *SignalJetKTDRs = nullptr;
   vector<vector<double>> *SignalJetKTPT2s = nullptr;

   vector<double> *SignalJetSD1Subjet1Pt = nullptr;
   vector<double> *SignalJetSD1Subjet1Eta = nullptr;
   vector<double> *SignalJetSD1Subjet1Phi = nullptr;
   vector<double> *SignalJetSD1Subjet2Pt = nullptr;
   vector<double> *SignalJetSD1Subjet2Eta = nullptr;
   vector<double> *SignalJetSD1Subjet2Phi = nullptr;
   vector<double> *SignalJetSD1DR12 = nullptr;
   vector<double> *SignalJetSD2Subjet1Pt = nullptr;
   vector<double> *SignalJetSD2Subjet1Eta = nullptr;
   vector<double> *SignalJetSD2Subjet1Phi = nullptr;
   vector<double> *SignalJetSD2Subjet2Pt = nullptr;
   vector<double> *SignalJetSD2Subjet2Eta = nullptr;
   vector<double> *SignalJetSD2Subjet2Phi = nullptr;
   vector<double> *SignalJetSD2DR12 = nullptr;
   vector<double> *SignalJetSD3Subjet1Pt = nullptr;
   vector<double> *SignalJetSD3Subjet1Eta = nullptr;
   vector<double> *SignalJetSD3Subjet1Phi = nullptr;
   vector<double> *SignalJetSD3Subjet2Pt = nullptr;
   vector<double> *SignalJetSD3Subjet2Eta = nullptr;
   vector<double> *SignalJetSD3Subjet2Phi = nullptr;
   vector<double> *SignalJetSD3DR12 = nullptr;
   vector<double> *SignalJetSD4Subjet1Pt = nullptr;
   vector<double> *SignalJetSD4Subjet1Eta = nullptr;
   vector<double> *SignalJetSD4Subjet1Phi = nullptr;
   vector<double> *SignalJetSD4Subjet2Pt = nullptr;
   vector<double> *SignalJetSD4Subjet2Eta = nullptr;
   vector<double> *SignalJetSD4Subjet2Phi = nullptr;
   vector<double> *SignalJetSD4DR12 = nullptr;
   vector<double> *SignalJetSD5Subjet1Pt = nullptr;
   vector<double> *SignalJetSD5Subjet1Eta = nullptr;
   vector<double> *SignalJetSD5Subjet1Phi = nullptr;
   vector<double> *SignalJetSD5Subjet2Pt = nullptr;
   vector<double> *SignalJetSD5Subjet2Eta = nullptr;
   vector<double> *SignalJetSD5Subjet2Phi = nullptr;
   vector<double> *SignalJetSD5DR12 = nullptr;
   vector<double> *PartonPt = nullptr;
   vector<double> *PartonEta = nullptr;
   vector<double> *PartonPhi = nullptr;
   vector<double> *PartonSJ1sPt = nullptr;
   vector<double> *PartonSJ1sEta = nullptr;
   vector<double> *PartonSJ1sPhi = nullptr;
   vector<double> *PartonSJ2sPt = nullptr;
   vector<double> *PartonSJ2sEta = nullptr;
   vector<double> *PartonSJ2sPhi = nullptr;
   vector<double> *PartonDRs = nullptr;

   Tree.SetBranchAddress("SignalJetPt",            &SignalJetPt);  
   Tree.SetBranchAddress("SignalJetEta",           &SignalJetEta);  
   Tree.SetBranchAddress("SignalJetPhi",           &SignalJetPhi);  
   Tree.SetBranchAddress("SignalJetAKDRs",         &SignalJetAKDRs);  
   Tree.SetBranchAddress("SignalJetAKPT2s",        &SignalJetAKPT2s);  
   Tree.SetBranchAddress("SignalJetCADRs",         &SignalJetCADRs);  
   Tree.SetBranchAddress("SignalJetCAPT2s",        &SignalJetCAPT2s);  
   Tree.SetBranchAddress("SignalJetKTDRs",         &SignalJetKTDRs);  
   Tree.SetBranchAddress("SignalJetKTPT2s",        &SignalJetKTPT2s);  
   Tree.SetBranchAddress("SignalJetSD1Subjet1Pt",  &SignalJetSD1Subjet1Pt);
   Tree.SetBranchAddress("SignalJetSD1Subjet1Eta", &SignalJetSD1Subjet1Eta);
   Tree.SetBranchAddress("SignalJetSD1Subjet1Phi", &SignalJetSD1Subjet1Phi);
   Tree.SetBranchAddress("SignalJetSD1Subjet2Pt",  &SignalJetSD1Subjet2Pt);
   Tree.SetBranchAddress("SignalJetSD1Subjet2Eta", &SignalJetSD1Subjet2Eta);
   Tree.SetBranchAddress("SignalJetSD1Subjet2Phi", &SignalJetSD1Subjet2Phi);
   Tree.SetBranchAddress("SignalJetSD1DR12",       &SignalJetSD1DR12);
   Tree.SetBranchAddress("SignalJetSD2Subjet1Pt",  &SignalJetSD2Subjet1Pt);
   Tree.SetBranchAddress("SignalJetSD2Subjet1Eta", &SignalJetSD2Subjet1Eta);
   Tree.SetBranchAddress("SignalJetSD2Subjet1Phi", &SignalJetSD2Subjet1Phi);
   Tree.SetBranchAddress("SignalJetSD2Subjet2Pt",  &SignalJetSD2Subjet2Pt);
   Tree.SetBranchAddress("SignalJetSD2Subjet2Eta", &SignalJetSD2Subjet2Eta);
   Tree.SetBranchAddress("SignalJetSD2Subjet2Phi", &SignalJetSD2Subjet2Phi);
   Tree.SetBranchAddress("SignalJetSD2DR12",       &SignalJetSD2DR12);
   Tree.SetBranchAddress("SignalJetSD3Subjet1Pt",  &SignalJetSD3Subjet1Pt);
   Tree.SetBranchAddress("SignalJetSD3Subjet1Eta", &SignalJetSD3Subjet1Eta);
   Tree.SetBranchAddress("SignalJetSD3Subjet1Phi", &SignalJetSD3Subjet1Phi);
   Tree.SetBranchAddress("SignalJetSD3Subjet2Pt",  &SignalJetSD3Subjet2Pt);
   Tree.SetBranchAddress("SignalJetSD3Subjet2Eta", &SignalJetSD3Subjet2Eta);
   Tree.SetBranchAddress("SignalJetSD3Subjet2Phi", &SignalJetSD3Subjet2Phi);
   Tree.SetBranchAddress("SignalJetSD3DR12",       &SignalJetSD3DR12);
   Tree.SetBranchAddress("SignalJetSD4Subjet1Pt",  &SignalJetSD4Subjet1Pt);
   Tree.SetBranchAddress("SignalJetSD4Subjet1Eta", &SignalJetSD4Subjet1Eta);
   Tree.SetBranchAddress("SignalJetSD4Subjet1Phi", &SignalJetSD4Subjet1Phi);
   Tree.SetBranchAddress("SignalJetSD4Subjet2Pt",  &SignalJetSD4Subjet2Pt);
   Tree.SetBranchAddress("SignalJetSD4Subjet2Eta", &SignalJetSD4Subjet2Eta);
   Tree.SetBranchAddress("SignalJetSD4Subjet2Phi", &SignalJetSD4Subjet2Phi);
   Tree.SetBranchAddress("SignalJetSD5DR12",       &SignalJetSD5DR12);
   Tree.SetBranchAddress("SignalJetSD5Subjet1Pt",  &SignalJetSD5Subjet1Pt);
   Tree.SetBranchAddress("SignalJetSD5Subjet1Eta", &SignalJetSD5Subjet1Eta);
   Tree.SetBranchAddress("SignalJetSD5Subjet1Phi", &SignalJetSD5Subjet1Phi);
   Tree.SetBranchAddress("SignalJetSD5Subjet2Pt",  &SignalJetSD5Subjet2Pt);
   Tree.SetBranchAddress("SignalJetSD5Subjet2Eta", &SignalJetSD5Subjet2Eta);
   Tree.SetBranchAddress("SignalJetSD5Subjet2Phi", &SignalJetSD5Subjet2Phi);
   Tree.SetBranchAddress("SignalJetSD5DR12",       &SignalJetSD5DR12);
   Tree.SetBranchAddress("PartonPt",               &PartonPt);
   Tree.SetBranchAddress("PartonEta",              &PartonEta);
   Tree.SetBranchAddress("PartonPhi",              &PartonPhi);
   Tree.SetBranchAddress("PartonSJ1sPt",           &PartonSJ1sPt);
   Tree.SetBranchAddress("PartonSJ1sEta",          &PartonSJ1sEta);
   Tree.SetBranchAddress("PartonSJ1sPhi",          &PartonSJ1sPhi);
   Tree.SetBranchAddress("PartonSJ2sPt",           &PartonSJ2sPt);
   Tree.SetBranchAddress("PartonSJ2sEta",          &PartonSJ2sEta);
   Tree.SetBranchAddress("PartonSJ2sPhi",          &PartonSJ2sPhi);
   Tree.SetBranchAddress("PartonDRs",              &PartonDRs);

   TFile outputfile(Form("%s.root",OutputBase.c_str()),"recreate");

   TTree *t = new TTree("t","");

   double LeadingPartonPt;
   double LeadingPartonEta;
   double LeadingPartonPhi;
   double PartonKt1;
   double PartonKt2;
   double PartonDR1;
   double PartonDR2;
   double MatchedJetPt;
   double MatchedJetEta;
   double MatchedJetPhi;
   double AKKt1;
   double AKKt2;
   double CAKt1;
   double CAKt2;
   double KTKt1;
   double KTKt2;
   double SD1Kt1;
   double SD2Kt1;
   double SD3Kt1;
   double AKDR1;
   double AKDR2;
   double CADR1;
   double CADR2;
   double KTDR1;
   double KTDR2;
   double SD1DR1;
   double SD2DR1;
   double SD3DR1;
   double SD4Kt1;
   double SD5Kt1;

   t->Branch("LeadingPartonPt" ,&LeadingPartonPt,  "LeadingPartonPt/D" );
   t->Branch("LeadingPartonEta",&LeadingPartonEta, "LeadingPartonEta/D");
   t->Branch("LeadingPartonPhi",&LeadingPartonPhi, "LeadingPartonPhi/D");
   t->Branch("PartonKt1"       ,&PartonKt1,        "PartonKt1/D");
   t->Branch("PartonKt2"       ,&PartonKt2,        "PartonKt2/D");
   t->Branch("MatchedJetPt"    ,&MatchedJetPt,     "MatchedJetPt/D" );
   t->Branch("MatchedJetEta"   ,&MatchedJetEta,    "MatchedJetEta/D");
   t->Branch("MatchedJetPhi"   ,&MatchedJetPhi,    "MatchedJetPhi/D");
   t->Branch("AKKt1"           ,&AKKt1,            "AKKt1/D");
   t->Branch("AKKt2"           ,&AKKt2,            "AKKt2/D");
   t->Branch("CAKt1"           ,&CAKt1,            "CAKt1/D");
   t->Branch("CAKt2"           ,&CAKt2,            "CAKt2/D");
   t->Branch("KTKt1"           ,&KTKt1,            "KTKt1/D");
   t->Branch("KTKt2"           ,&KTKt2,            "KTKt2/D");
   t->Branch("SD1Kt1"          ,&SD1Kt1,           "SD1Kt1/D");
   t->Branch("SD2Kt1"          ,&SD2Kt1,           "SD2Kt1/D");
   t->Branch("SD3Kt1"          ,&SD3Kt1,           "SD3Kt1/D");
   t->Branch("PartonDR1"       ,&PartonDR1,        "PartonDR1/D");
   t->Branch("PartonDR2"       ,&PartonDR2,        "PartonDR2/D");
   t->Branch("AKDR1"           ,&AKDR1,            "AKDR1/D");
   t->Branch("AKDR2"           ,&AKDR2,            "AKDR2/D");
   t->Branch("CADR1"           ,&CADR1,            "CADR1/D");
   t->Branch("CADR2"           ,&CADR2,            "CADR2/D");
   t->Branch("KTDR1"           ,&KTDR1,            "KTDR1/D");
   t->Branch("KTDR2"           ,&KTDR2,            "KTDR2/D");
   t->Branch("SD1DR1"          ,&SD1DR1,           "SD1DR1/D");
   t->Branch("SD2DR1"          ,&SD2DR1,           "SD2DR1/D");
   t->Branch("SD3DR1"          ,&SD3DR1,           "SD3DR1/D");
   t->Branch("SD4Kt1"          ,&SD4Kt1,           "SD4Kt1/D");
   t->Branch("SD5Kt1"          ,&SD5Kt1,           "SD5Kt1/D");


   int EntryCount = Tree.GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree.GetEntry(iE);
      if (iE%1000==0) cout <<iE<<endl;
      if(PartonPt->size() == 0)   // WTF      
         continue;

      if(SignalJetPt->size() == 0)   // WTF
         continue;

      LeadingPartonPt = (*PartonPt)[0];
      LeadingPartonEta = (*PartonEta)[0];
      LeadingPartonPhi = (*PartonPhi)[0];
      //cout <<LeadingPartonPt<<endl;

      int BestJetIndex = 0;
      double BestDR = -1;

      // Find the best jet contains the higest pT parton
      for(int i = 0; i < (int)SignalJetPt->size(); i++)
      {
         double DR = GetDR(LeadingPartonEta, LeadingPartonPhi, (*SignalJetEta)[i], (*SignalJetPhi)[i]);
         if(BestDR < 0 || BestDR > DR)
         {
            BestJetIndex = i;
            BestDR = DR;
         }
      }

      MatchedJetPt = (*SignalJetPt)[BestJetIndex];
      MatchedJetEta = (*SignalJetEta)[BestJetIndex];
      MatchedJetPhi = (*SignalJetPhi)[BestJetIndex];

      PartonKt1=0;
      PartonKt2=0;
      PartonDR1=0;
      PartonDR2=0;
      for (int i = 0; i < (int)PartonSJ2sPt->size(); i++){
          double Kt=(*PartonSJ2sPt)[i]*(*PartonDRs)[i];
	  if (Kt>PartonKt2) {
	     PartonKt2=Kt;
             PartonDR2=(*PartonDRs)[i];
          }
	  if (Kt>PartonKt1) {
	     PartonKt2=PartonKt1;
	     PartonDR2=PartonDR1;
	     PartonKt1=Kt;
             PartonDR1=(*PartonDRs)[i];
	  }   
      }


      // Anti-kT algorithm performance
      AKKt1=0;
      AKKt2=0;
      AKDR1=0;
      AKDR2=0;
      for (int i = 0; i < (int)(*SignalJetAKPT2s)[BestJetIndex].size(); i++){
          double Kt=(*SignalJetAKPT2s)[BestJetIndex][i]*(*SignalJetAKDRs)[BestJetIndex][i];
	  if (Kt>AKKt2) {
	     AKKt2=Kt;
	     AKDR2=(*SignalJetAKDRs)[BestJetIndex][i];
	  }
	  if (Kt>AKKt1) {
	     AKKt2=AKKt1;
	     AKDR2=AKDR1;
	     AKKt1=Kt;
	     AKDR1=(*SignalJetAKDRs)[BestJetIndex][i];
	  }   
      }

      // CA algorithm performance
      CAKt1=0;
      CAKt2=0;
      for (int i = 0; i < (int)(*SignalJetCAPT2s)[BestJetIndex].size(); i++){
          double Kt=(*SignalJetCAPT2s)[BestJetIndex][i]*(*SignalJetCADRs)[BestJetIndex][i];
	  if (Kt>CAKt2) {
	     CAKt2=Kt;
	     CADR2=(*SignalJetCADRs)[BestJetIndex][i];
	  }
	  if (Kt>CAKt1) {
	     CAKt2=CAKt1;
	     CADR2=CADR1;
	     CAKt1=Kt;
	     CADR1=(*SignalJetCADRs)[BestJetIndex][i];
	  }   
      }

      // kT algorithm performance
      KTKt1=0;
      KTKt2=0;
      for (int i = 0; i < (int)(*SignalJetKTPT2s)[BestJetIndex].size(); i++){
          double Kt=(*SignalJetKTPT2s)[BestJetIndex][i]*(*SignalJetKTDRs)[BestJetIndex][i];
	  if (Kt>KTKt2) {
	     KTKt2=Kt;
	     KTDR2=(*SignalJetKTDRs)[BestJetIndex][i];
	  }
	  if (Kt>KTKt1) {
	     KTKt2=KTKt1;
	     KTDR2=KTDR1;
	     KTKt1=Kt;
	     KTDR1=(*SignalJetKTDRs)[BestJetIndex][i];
	  }   
      }

      // Groomed jet performance 
      SD1Kt1=(*SignalJetSD1DR12)[BestJetIndex]*(*SignalJetSD1Subjet2Pt)[BestJetIndex];
      SD2Kt1=(*SignalJetSD2DR12)[BestJetIndex]*(*SignalJetSD2Subjet2Pt)[BestJetIndex];
      SD3Kt1=(*SignalJetSD3DR12)[BestJetIndex]*(*SignalJetSD3Subjet2Pt)[BestJetIndex];
      SD1DR1=(*SignalJetSD1DR12)[BestJetIndex];
      SD2DR1=(*SignalJetSD2DR12)[BestJetIndex];
      SD3DR1=(*SignalJetSD3DR12)[BestJetIndex];
      
      SD4Kt1=(*SignalJetSD4DR12)[BestJetIndex]*(*SignalJetSD4Subjet2Pt)[BestJetIndex];
      SD5Kt1=(*SignalJetSD5DR12)[BestJetIndex]*(*SignalJetSD5Subjet2Pt)[BestJetIndex];

      t->Fill();
   }

   t->Write();
   return 0;
}

double GetDR(double eta1, double phi1, double eta2, double phi2)
{
   double deta = eta1 - eta2;
   double dphi = phi1 - phi2;

   if(dphi < -M_PI)   dphi = dphi + 2 * M_PI;
   if(dphi > +M_PI)   dphi = dphi - 2 * M_PI;

   return sqrt(deta * deta + dphi * dphi);
}


