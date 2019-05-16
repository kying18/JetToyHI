#include <iostream>
using namespace std;

#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "PlotHelper4.h"
#include "SetStyle.h"

#include "Messenger.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   TFile File("Example.root");

   Messenger M(File, "JetTree");

   PdfFileHelper PdfFile("Plots.pdf");
   PdfFile.AddTextPage("Plots");

   TH1D HPhotonPT("HPhotonPT", "Photon PT;PT;", 100, 0, 1000);

   for(int iE = 0; iE < M.GetEntries(); iE++)
   {
      M.GetEntry(iE);
      HPhotonPT.Fill(M.LeadingPhoton.Pt());
   }

   PdfFile.AddPlot(HPhotonPT, "", true);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}
