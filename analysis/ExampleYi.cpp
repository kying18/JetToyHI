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

   string InputFileName = CL.Get("input");
   string OutputFileName = CL.Get("output", "Plots.pdf");

   TFile File(InputFileName.c_str());

   Messenger M(File, "JetTree");

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("Plots");

   TH1D HPhotonPT("HPhotonPT", "Photon PT;PT;", 100, 0, 1000);
   TH1D HPhotonEta("HPhotonEta", "Photon Eta;Eta;", 100, -3.0, 3.0);
   TH1D HNJet("HNJet", "Jet count;;", 10, 0, 10);
   TH1D HJetPT("HJetPT", "Jet PT;PT;", 100, 0, 1000);
   TH1D HJetEta("HJetEta", "Jet Eta;Eta;", 100, -3.0, 3.0);
   TH1D HDPhiAJ("HDPhiAJ", "DPhi(photon, jet);;", 100, -3.14, 3.14);
   TH1D HDPhi("HDPhi", "DPhi(E-scheme, WTA-scheme);;", 100, -0.3, 0.3);
   TH1D HDEta("HDEta", "DEta(E-scheme, WTA-scheme);;", 100, -0.3, 0.3);
   TH1D HDR("HDR", "DR(E-scheme, WTA-scheme);;", 100, 0.0, 0.3);

   for(int iE = 0; iE < M.GetEntries(); iE++)
   {
      M.GetEntry(iE);
   
      if(M.LeadingPhoton.Pt() < 10)
         continue;

      HPhotonPT.Fill(M.LeadingPhoton.Pt());
      HPhotonEta.Fill(M.LeadingPhoton.Eta());

      HNJet.Fill(M.SignalJetR04.size());

      int JetIndex = -1;

      for(int iJ = 0; iJ < M.SignalJetR04.size(); iJ++)
      {
         HJetPT.Fill(M.SignalJetR04[iJ].P.Pt());
         HJetEta.Fill(M.SignalJetR04[iJ].P.Eta());
         
         double DPhiAJ = M.SignalJetR04[iJ].P.Phi() - M.LeadingPhoton.Phi();
         if(DPhiAJ > +M_PI)   DPhiAJ = DPhiAJ - 2 * M_PI;
         if(DPhiAJ < -M_PI)   DPhiAJ = DPhiAJ + 2 * M_PI;

         if(DPhiAJ > -M_PI / 2 && DPhiAJ < M_PI / 2)
            continue;

         JetIndex = iJ;
         break;
      }

      if(JetIndex >= 0)
      {
         double DPhiAJ = M.SignalJetR04[JetIndex].P.Phi() - M.LeadingPhoton.Phi();
         double DPhi = M.SignalJetR04[JetIndex].P.Phi() - M.SignalJetR04[JetIndex].WTA.Phi();
         if(DPhi > +M_PI)   DPhi = DPhi - 2 * M_PI;
         if(DPhi < -M_PI)   DPhi = DPhi + 2 * M_PI;
         double DEta = M.SignalJetR04[JetIndex].P.Eta() - M.SignalJetR04[JetIndex].WTA.Eta();
         double DR = sqrt(DEta * DEta + DPhi * DPhi);

         HDPhi.Fill(DPhi);
         HDEta.Fill(DEta);
         HDR.Fill(DR);
         HDPhiAJ.Fill(DPhiAJ);
      }
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

   return 0;
}
