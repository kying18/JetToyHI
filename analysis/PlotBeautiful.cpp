#include <iostream>
using namespace std;

#include "TCanvas.h"
#include "TFile.h"
#include "TH2D.h"

#include "SetStyle.h"
#include "CommandLine.h"
#include "PlotHelper4.h"

int main(int argc, char *argv[]);
void Add2DPlot(PdfFileHelper &PdfFile, string OutputBase, TTree *Tree, string ToPlot, string Selection,
   string Title, int NX, double XMin, double XMax, int NY, double YMin, double YMax);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string Tag = CL.Get("Tag", ""); 
   string OutputBase = CL.Get("OutputBase", "Plots");

   string Cut = CL.Get("Cut", ""); 

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage(Tag);

   TFile File(CL.Get("Input").c_str());

   TTree *Tree = (TTree *)File.Get(CL.Get("Tree", "t").c_str());

   Add2DPlot(PdfFile, OutputBase, Tree, "PartonDR1:KTDR1", Cut.c_str(),
      Tag + "KT Declustering;KTDR1;PartonDR1", 100, 0, 0.5, 100, 0, 0.5);
   Add2DPlot(PdfFile, OutputBase, Tree, "PartonDR1:AKDR1", Cut.c_str(),
      Tag + "AK Declustering;AKDR1;PartonDR1", 100, 0, 0.5, 100, 0, 0.5);
   Add2DPlot(PdfFile, OutputBase, Tree, "PartonDR1:CADR1", Cut.c_str(),
      Tag + "CA Declustering;CADR1;PartonDR1", 100, 0, 0.5, 100, 0, 0.5);

   Add2DPlot(PdfFile, OutputBase, Tree, "PartonDR1:SD1DR1", Cut.c_str(),
      Tag + "(Z_{cut},#beta)=(0.1,0.0);SD1DR1;PartonDR1", 100, 0, 0.5, 100, 0, 0.5);
   Add2DPlot(PdfFile, OutputBase, Tree, "PartonDR1:SD2DR1", Cut.c_str(),
      Tag + "(Z_{cut},#beta)=(0.5,1.5);SD2DR1;PartonDR1", 100, 0, 0.5, 100, 0, 0.5);
   Add2DPlot(PdfFile, OutputBase, Tree, "PartonDR1:SD3DR1", Cut.c_str(),
      Tag + "(Z_{cut},#beta)=(0.25,0.0);SD3DR1;PartonDR1", 100, 0, 0.5, 100, 0, 0.5);
   Add2DPlot(PdfFile, OutputBase, Tree, "PartonDR1:SD4DR1", Cut.c_str(),
      Tag + "(Z_{cut},#beta)=(0.15,-1.0);SD4DR1;PartonDR1", 100, 0, 0.5, 100, 0, 0.5);
   Add2DPlot(PdfFile, OutputBase, Tree, "PartonDR1:SD5DR1", Cut.c_str(),
      Tag + "(Z_{cut},#beta)=(5.0,5.0);SD5DR1;PartonDR1", 100, 0, 0.5, 100, 0, 0.5);

   File.Close();

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

void Add2DPlot(PdfFileHelper &PdfFile, string OutputBase, TTree *Tree, string ToPlot, string Selection,
   string Title, int NX, double XMin, double XMax, int NY, double YMin, double YMax)
{
   static int Count = 1;

   TCanvas Canvas;

   TH2D H("H", Title.c_str(), NX, XMin, XMax, NY, YMin, YMax);
   H.SetStats(0);

   Tree->Draw((ToPlot + ">>H").c_str(), Selection.c_str(), "colz");

   Count = Count + 1;
   Canvas.SaveAs(Form("%s_%d.png", OutputBase.c_str(), Count));
   PdfFile.AddCanvas(Canvas);

   Canvas.SetLogz();
   
   Count = Count + 1;
   Canvas.SaveAs(Form("%s_%d.png", OutputBase.c_str(), Count));
   PdfFile.AddCanvas(Canvas);
}



