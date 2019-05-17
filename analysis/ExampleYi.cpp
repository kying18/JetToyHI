#include <vector>
#include <iostream>
using namespace std;

#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "PlotHelper4.h"
#include "SetStyle.h"
#include "CommandLine.h"

#include "Messenger.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string OutputBase = CL.Get("output", "Plots");
   vector<string> InputFileName = CL.GetStringVector("input");
   bool DoJewel = CL.GetBool("jewel", false);
   string Radius = CL.Get("R", "04");

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("Plots");

   TFile OutputFile((OutputBase + ".root").c_str(), "RECREATE");

   TH1D HPhotonPT("HPhotonPT", "Photon PT;PT;", 100, 0, 1000);
   TH1D HPhotonEta("HPhotonEta", "Photon Eta;Eta;", 100, -3.0, 3.0);
   TH1D HNJet("HNJet", "Jet count;;", 10, 0, 10);
   TH1D HJetPT("HJetPT", "Jet PT;PT;", 100, 0, 1000);
   TH1D HJetEta("HJetEta", "Jet Eta;Eta;", 100, -3.0, 3.0);
   TH1D HDPhiAJ("HDPhiAJ", "DPhi(photon, jet);;", 100, -3.14, 3.14);
   TH1D HDPhi("HDPhi", "DPhi(E-scheme, WTA-scheme);;", 100, -0.5, 0.5);
   TH1D HDEta("HDEta", "DEta(E-scheme, WTA-scheme);;", 100, -0.5, 0.5);
   TH1D HDR("HDR", "DR(E-scheme, WTA-scheme);;", 100, 0.0, 0.5);
   
   for(string FileName : InputFileName)
   {
      TFile File(FileName.c_str());

      Messenger M(File, "JetTree");

      for(int iE = 0; iE < M.GetEntries(); iE++)
      {
         M.GetEntry(iE);

         if(M.LeadingPhoton.Pt() < 10)
            continue;
         if(M.LeadingPhoton.Eta() < -2 || M.LeadingPhoton.Eta() > 2)
            continue;

         HPhotonPT.Fill(M.LeadingPhoton.Pt());
         HPhotonEta.Fill(M.LeadingPhoton.Eta());

         vector<Jet> Jets;
         if(Radius == "02")   Jets = M.SignalJetR02;
         if(Radius == "04")   Jets = M.SignalJetR04;
         if(Radius == "06")   Jets = M.SignalJetR06;
         if(Radius == "08")   Jets = M.SignalJetR08;
         if(Radius == "10")   Jets = M.SignalJetR10;

         HNJet.Fill(Jets.size());

         int JetIndex = -1;

         for(int iJ = 0; iJ < Jets.size(); iJ++)
         {
            TLorentzVector P;
            if(DoJewel == true)
               P = Jets[iJ].JewelP;
            else
               P = Jets[iJ].P;

            if(P.Pt() < 10)
               continue;
            if(P.Eta() < -2.5 || P.Eta() > 2.0)
               continue;

            HJetPT.Fill(P.Pt());
            HJetEta.Fill(P.Eta());

            double DPhiAJ = P.Phi() - M.LeadingPhoton.Phi();
            if(DPhiAJ > +M_PI)   DPhiAJ = DPhiAJ - 2 * M_PI;
            if(DPhiAJ < -M_PI)   DPhiAJ = DPhiAJ + 2 * M_PI;

            if(DPhiAJ > -M_PI / 2 && DPhiAJ < M_PI / 2)
               continue;

            JetIndex = iJ;
            break;
         }

         if(JetIndex >= 0)
         {
            TLorentzVector P;
            if(DoJewel == true)
               P = Jets[JetIndex].JewelP;
            else
               P = Jets[JetIndex].P;

            double DPhiAJ = P.Phi() - M.LeadingPhoton.Phi();
            double DPhi = P.Phi() - Jets[JetIndex].WTA.Phi();
            if(DPhi > +M_PI)   DPhi = DPhi - 2 * M_PI;
            if(DPhi < -M_PI)   DPhi = DPhi + 2 * M_PI;
            double DEta = P.Eta() - Jets[JetIndex].WTA.Eta();
            double DR = sqrt(DEta * DEta + DPhi * DPhi);

            HDPhi.Fill(DPhi);
            HDEta.Fill(DEta);
            HDR.Fill(DR);
            HDPhiAJ.Fill(DPhiAJ);
         }
      }

      File.Close();
   }

   PdfFile.AddPlot(HPhotonPT, "", true);
   PdfFile.AddPlot(HPhotonEta, "", true);
   PdfFile.AddPlot(HNJet, "", true);
   PdfFile.AddPlot(HJetPT, "", true);
   PdfFile.AddPlot(HJetEta, "", true);
   PdfFile.AddPlot(HDPhiAJ, "", false);
   PdfFile.AddPlot(HDPhi, "", false);
   PdfFile.AddPlot(HDEta, "", false);
   PdfFile.AddPlot(HDR, "", false);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   OutputFile.cd();

   HPhotonPT.Write();
   HPhotonEta.Write();
   HNJet.Write();
   HJetPT.Write();
   HJetEta.Write();
   HDPhiAJ.Write();
   HDPhi.Write();
   HDEta.Write();
   HDR.Write();

   OutputFile.Close();

   return 0;
}
