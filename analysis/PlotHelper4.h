#ifndef PlotHelper17544
#define PlotHelper17544

// Plot helper, (1) 5653 version
// Intended to help organizing plots and to avoid cluttering
// The main idea is to write a plot both to a pdf and to a TFile at the same time,
//    so that we can check quickly using pdf and come back to TFile later if needed
//    the helper functions here will be mostly on *.pdf files
// Added feature compared to 6152: being able to insert a table of contents (and it's clickable!)
//    automatic home button
//    automatic page number
// Developer: Yi Chen, (1) 5653

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

#include "TCanvas.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TVirtualPad.h"
#include "TPDF.h"
#include "TStyle.h"

class PdfFileHelper
{
private:
   string FileName;
   string Option;
   bool Status;   // false - file not opened; true - file good and ongoing
private:
   bool AutomaticHomeButton;
   double HomeButtonX;
   double HomeButtonY;
   double HomeButtonSize;
   string HomeButtonDestination;
   bool PrintPageNumber;
   int NextPageNumber;
public:
   PdfFileHelper();
   PdfFileHelper(string filename);
   PdfFileHelper(string filename, string option);
   ~PdfFileHelper();
   void Open(string filename);
   void Close();
   string GetFileName();
   void SetOption(string option);
   string GetOption();
   template <class PlotType> void AddPlot(PlotType *Histogram, string PlotOption = "",
      bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   template <class PlotType> void Add4PanelPlot(PlotType *Histogram1, PlotType *Histogram2,
      PlotType *Histogram3, PlotType *Histogram4, string PlotOption = "",
      bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   template <class PlotType> void AddPlot(PlotType &Histogram, string PlotOption = "",
      bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   template <class PlotType> void Add4PanelPlot(PlotType &Histogram1, PlotType &Histogram2,
      PlotType &Histogram3, PlotType &Histogram4, string PlotOption = "",
      bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   template <class PlotType> void AddNormalizedPlot(PlotType *Histogram, string PlotOption = "",
      bool LogY = false, bool LogZ = false, bool Grid = false);
   template <class PlotType> void AddNormalizedPlot(PlotType &Histogram, string PlotOption = "",
      bool LogY = false, bool LogZ = false, bool Grid = false);
   template <class PlotType> void AddPlotWithText(PlotType *Histogram, string Text,
      string PlotOption = "", double X = 0.1, double Y = 0.9, double TextSize = 0.03);
   template <class PlotType> void AddPlotWithText(PlotType &Histogram, string Text,
      string PlotOption = "", double X = 0.1, double Y = 0.9, double TextSize = 0.03);
   void AddHistogramFromFile(TFile &File, string HistogramName,
      string PlotOption = "", bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   void Add4PanelHistogramFromFile(TFile &File, string HistogramName1, string HistogramName2,
      string HistogramName3, string HistogramName4,
      string PlotOption = "", bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   void AddGraphFromFile(TFile &File, string GraphName,
      string PlotOption = "", bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   void AddPlot(TTree *Tree, string ToPlot, string Selection, string PlotOption = "", string Titles = ";;",
      int Bin = 100, double Min = 0, double Max = 100,
      bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   void AddPlotProfile(TTree *Tree, string ToPlot, string Selection, string PlotOption = "prof", string Titles = ";;",
      int Bin = 100, double Min = 0, double Max = 100,
      bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   void AddPlot2D(TTree *Tree, string ToPlot, string Selection, string PlotOption = "", string Titles = ";;",
      int BinX = 100, double MinX = 0, double MaxX = 100,
      int BinY = 100, double MinY = 0, double MaxY = 100,
      bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   void AddPlotProfile2D(TTree *Tree, string ToPlot, string Selection, string PlotOption = "colz", string Titles = ";;",
      int BinX = 100, double MinX = 0, double MaxX = 100,
      int BinY = 100, double MinY = 0, double MaxY = 100,
      bool LogY = false, bool LogZ = false, bool Grid = false, bool LogX = false);
   void AddCanvas(TCanvas *Canvas);
   void AddCanvas(TCanvas &Canvas);
   void AddCanvasWithText(TCanvas *Canvas, string Text, double X = 0.1, double Y = 0.9, double TextSize = 0.03);
   void AddCanvasWithText(TCanvas &Canvas, string Text, double X = 0.1, double Y = 0.9, double TextSize = 0.03);
   void AddTextPage(string Text, double TextSize = 0.05);
   void AddTextPage(string Text, double X, double Y, double TextSize = 0.05);
   void AddTextPage(vector<string> Text, double X = 0.1, double Y = 0.9, double TextSize = 0.04);
   void AddTimeStampPage();
   void AddTableOfContentPage(vector<string> Items, vector<string> Destinations,
      double X = 0.11, double Y = 0.8, double TextSize = 0.03);
public:
   void PrintStringToPdfFile(string String);
   void PrintLineToPdfFile(string Line);
   void InsertNamedDestination(string Name);
   void InsertBallLinkAbsolute(int X, int Y, int Radius, string Destination);
   void InsertHomeButtonAbsolute(double X, double Y, double Size, string Destination);
   void SetAutomaticHomeButton(bool newvalue = true, string Destination = "HomePage",
      double X = 0.97, double Y = 0.97, double Size = 0.03);
   void SetPageNumber(bool printnumber);
   void InsertPageNumber(TCanvas &Canvas, int PageNumber = -1);
};

PdfFileHelper::PdfFileHelper()
{
   Status = false;
   FileName = "";
   Option = "Landscape";

   AutomaticHomeButton = false;
   HomeButtonX = 50;
   HomeButtonY = 50;
   HomeButtonSize = 75;
   HomeButtonDestination = "";
   PrintPageNumber = true;
   NextPageNumber = 1;
}

PdfFileHelper::PdfFileHelper(string filename)
{
   Option = "Landscape";

   AutomaticHomeButton = false;
   HomeButtonX = 50;
   HomeButtonY = 50;
   HomeButtonSize = 75;
   HomeButtonDestination = "";
   PrintPageNumber = true;
   NextPageNumber = 1;
   
   Open(filename);
}

PdfFileHelper::PdfFileHelper(string filename, string option)
{
   Option = option;
   
   AutomaticHomeButton = false;
   HomeButtonX = 50;
   HomeButtonY = 50;
   HomeButtonSize = 75;
   HomeButtonDestination = "";
   PrintPageNumber = true;
   NextPageNumber = 1;

   Open(filename);
}

PdfFileHelper::~PdfFileHelper()
{
   if(Status == true)
      Close();
}

void PdfFileHelper::Open(string filename)
{
   FileName = filename;

   Status = true;

   TCanvas canvas;
   canvas.SaveAs((FileName + "[").c_str(), Option.c_str());

   // TPDF *PDFHandle = (TPDF *)gROOT->GetListOfSpecials()->FindObject(FileName.c_str());
   // if(PDFHandle != NULL)
   // {
   //    PDFHandle->PrintStr("\n");
   //    PDFHandle->PrintStr("%% pdfmark prolog: tell ps reader to ignore pdfmark if it's not supported\n");
   //    PDFHandle->PrintStr("/pdfmark where {pop} {userdict /pdfmark /cleartomark load put} ifelse\n");
   //    PDFHandle->PrintStr("\n");
   // }

   NextPageNumber = 1;
}

void PdfFileHelper::Close()
{
   TCanvas canvas;
   canvas.SaveAs((FileName + "]").c_str(), Option.c_str());

   Status = false;
   FileName = "";
   NextPageNumber = -1;
}

string PdfFileHelper::GetFileName()
{
   return FileName;
}

void PdfFileHelper::SetOption(string option)
{
   Option = option;
}

string PdfFileHelper::GetOption()
{
   return Option;
}

template <class PlotType> void PdfFileHelper::AddPlot(PlotType *Histogram, string PlotOption, bool LogY, bool LogZ, bool Grid, bool LogX)
{
   if(Histogram == NULL)
      return;
   
   TCanvas canvas;

   Histogram->Draw(PlotOption.c_str());

   if(LogX == true)
      canvas.SetLogx();
   if(LogY == true)
      canvas.SetLogy();
   if(LogZ == true)
      canvas.SetLogz();

   if(Grid == true)
   {
      canvas.SetGridx();
      canvas.SetGridy();
   }

   AddCanvas(canvas);
}

template <class PlotType> void PdfFileHelper::Add4PanelPlot(PlotType *Histogram1, PlotType *Histogram2,
   PlotType *Histogram3, PlotType *Histogram4, string PlotOption, bool LogY, bool LogZ, bool Grid, bool LogX)
{
   TCanvas canvas;

   canvas.Divide(2, 2);

   PlotType *Histograms[4] = {Histogram1, Histogram2, Histogram3, Histogram4};

   for(int i = 0; i < 4; i++)
   {
      if(Histograms[i] == NULL)
         continue;

      TVirtualPad *Pad = canvas.cd(i + 1);

      Histograms[i]->Draw(PlotOption.c_str());

      if(LogX == true)
         Pad->SetLogx();
      if(LogY == true)
         Pad->SetLogy();
      if(LogZ == true)
         Pad->SetLogz();

      if(Grid == true)
      {
         Pad->SetGridx();
         Pad->SetGridy();
      }
   }

   AddCanvas(canvas);
}

template <class PlotType> void PdfFileHelper::AddPlot(PlotType &Histogram, string PlotOption, bool LogY, bool LogZ, bool Grid, bool LogX)
{
   AddPlot(&Histogram, PlotOption, LogY, LogZ, Grid, LogX);
}

template <class PlotType> void Add4PanelPlot(PlotType &Histogram1, PlotType &Histogram2,
   PlotType &Histogram3, PlotType &Histogram4, string PlotOption, bool LogY, bool LogZ, bool Grid, bool LogX)
{
   Add4PanelPlot(&Histogram1, &Histogram2, &Histogram3, &Histogram4, PlotOption, LogY, LogZ, Grid, LogX);
}

template <class PlotType> void PdfFileHelper::AddNormalizedPlot(PlotType *Histogram, string PlotOption,
   bool LogY, bool LogZ, bool Grid)
{
   if(Histogram == NULL)
      return;

   TCanvas canvas;

   Histogram->DrawNormalized(PlotOption.c_str());

   if(Histogram->GetMaximum() > 1e-10)
   {
      if(LogY == true)
         canvas.SetLogy();
      if(LogZ == true)
         canvas.SetLogz();
   }
   if(Grid == true)
   {
      canvas.SetGridx();
      canvas.SetGridy();
   }

   AddCanvas(canvas);
}

template <class PlotType> void PdfFileHelper::AddNormalizedPlot(PlotType &Histogram, string PlotOption,
   bool LogY, bool LogZ, bool Grid)
{
   AddNormalizedPlot(&Histogram, PlotOption, LogY, LogZ, Grid);
}

template <class PlotType> void PdfFileHelper::AddPlotWithText(PlotType *Histogram, string Text,
   string PlotOption, double X, double Y, double TextSize)
{
   TCanvas canvas;

   Histogram->Draw(PlotOption.c_str());

   AddCanvasWithText(canvas, Text, X, Y, TextSize);
}

template <class PlotType> void PdfFileHelper::AddPlotWithText(PlotType &Histogram, string Text,
   string PlotOption, double X, double Y, double TextSize)
{
   AddPlotWithText(&Histogram, Text, PlotOption, X, Y, TextSize);
}

void PdfFileHelper::AddHistogramFromFile(TFile &File, string HistogramName, string PlotOption, bool LogY,
   bool LogZ, bool Grid, bool LogX)
{
   TH1 *Histogram = (TH1 *)File.Get(HistogramName.c_str());
   if(Histogram == NULL)
      return;

   AddPlot(Histogram, PlotOption, LogY, LogZ, Grid, LogX);
}

void PdfFileHelper::Add4PanelHistogramFromFile(TFile &File, string HistogramName1, string HistogramName2,
   string HistogramName3, string HistogramName4, string PlotOption, bool LogY, bool LogZ, bool Grid, bool LogX)
{
   TH1 *Histograms[4];

   Histograms[0] = (TH1 *)File.Get(HistogramName1.c_str());
   Histograms[1] = (TH1 *)File.Get(HistogramName2.c_str());
   Histograms[2] = (TH1 *)File.Get(HistogramName3.c_str());
   Histograms[3] = (TH1 *)File.Get(HistogramName4.c_str());

   Add4PanelPlot(Histograms[0], Histograms[1], Histograms[2], Histograms[3], PlotOption, LogY, LogZ, Grid, LogX);
}

void PdfFileHelper::AddGraphFromFile(TFile &File, string GraphName, string PlotOption, bool LogY, bool LogZ,
   bool Grid, bool LogX)
{
   TGraph *Graph = (TGraph *)File.Get(GraphName.c_str());
   if(Graph == NULL)
      return;

   AddPlot(Graph, PlotOption, LogY, LogZ, Grid, LogX);
}
   
void PdfFileHelper::AddPlot(TTree *Tree, string ToPlot, string Selection, string PlotOption, string Titles,
   int Bin, double Min, double Max,
   bool LogY, bool LogZ, bool Grid, bool LogX)
{
   if(Tree == NULL)
      return;

   TH1D H("TempHistogramForPdfFileHelper", Titles.c_str(), Bin, Min, Max);
   H.SetStats(0);

   Tree->Draw(Form("%s>>TempHistogramForPdfFileHelper", ToPlot.c_str()), Selection.c_str(), PlotOption.c_str());

   AddPlot(H, PlotOption, LogY, LogZ, Grid, LogX);
}

void PdfFileHelper::AddPlotProfile(TTree *Tree, string ToPlot, string Selection, string PlotOption, string Titles,
   int Bin, double Min, double Max,
   bool LogY, bool LogZ, bool Grid, bool LogX)
{
   if(Tree == NULL)
      return;

   TProfile H("TempHistogramForPdfFileHelper", Titles.c_str(), Bin, Min, Max);
   H.SetStats(0);

   Tree->Draw(Form("%s>>TempHistogramForPdfFileHelper", ToPlot.c_str()), Selection.c_str(), PlotOption.c_str());

   AddPlot(H, PlotOption, LogY, LogZ, Grid, LogX);
}

void PdfFileHelper::AddPlot2D(TTree *Tree, string ToPlot, string Selection, string PlotOption, string Titles,
   int BinX, double MinX, double MaxX,
   int BinY, double MinY, double MaxY,
   bool LogY, bool LogZ, bool Grid, bool LogX)
{
   if(Tree == NULL)
      return;

   TH2D H("TempHistogramForPdfFileHelper", Titles.c_str(), BinX, MinX, MaxX, BinY, MinY, MaxY);
   H.SetStats(0);

   Tree->Draw(Form("%s>>TempHistogramForPdfFileHelper", ToPlot.c_str()), Selection.c_str(), PlotOption.c_str());

   AddPlot(H, PlotOption, LogY, LogZ, Grid, LogX);
}

void PdfFileHelper::AddPlotProfile2D(TTree *Tree, string ToPlot, string Selection, string PlotOption, string Titles,
   int BinX, double MinX, double MaxX,
   int BinY, double MinY, double MaxY,
   bool LogY, bool LogZ, bool Grid, bool LogX)
{
   if(Tree == NULL)
      return;

   TProfile2D H("TempHistogramForPdfFileHelper", Titles.c_str(), BinX, MinX, MaxX, BinY, MinY, MaxY);
   H.SetStats(0);

   Tree->Draw(Form("%s>>TempHistogramForPdfFileHelper", ToPlot.c_str()), Selection.c_str(), PlotOption.c_str());

   AddPlot(H, PlotOption, LogY, LogZ, Grid, LogX);
}

void PdfFileHelper::AddCanvas(TCanvas *Canvas)
{
   if(Canvas == NULL)
      return;

   if(PrintPageNumber == true)
      InsertPageNumber(*Canvas, NextPageNumber);
   Canvas->Print(FileName.c_str(), Option.c_str());
   NextPageNumber = NextPageNumber + 1;

   if(AutomaticHomeButton == true)
      InsertHomeButtonAbsolute(HomeButtonX, HomeButtonY, HomeButtonSize, HomeButtonDestination);
}

void PdfFileHelper::AddCanvas(TCanvas &Canvas)
{
   AddCanvas(&Canvas);
}

void PdfFileHelper::AddCanvasWithText(TCanvas *Canvas, string Text, double X, double Y, double TextSize)
{
   Canvas->cd();

   TLatex text(X, Y, Text.c_str());
   text.SetTextSize(TextSize);
   text.Draw();

   if(PrintPageNumber == true)
      InsertPageNumber(*Canvas, NextPageNumber);
   Canvas->Print(FileName.c_str(), Option.c_str());
   NextPageNumber = NextPageNumber + 1;

   if(AutomaticHomeButton == true)
      InsertHomeButtonAbsolute(HomeButtonX, HomeButtonY, HomeButtonSize, HomeButtonDestination);
}

void PdfFileHelper::AddCanvasWithText(TCanvas &Canvas, string Text, double X, double Y, double TextSize)
{
   AddCanvasWithText(&Canvas, Text, X, Y, TextSize);
}

void PdfFileHelper::AddTextPage(string Text, double TextSize)
{
   TCanvas canvas;

   TLatex text(0.5, 0.5, Text.c_str());
   text.SetTextAlign(22);
   text.SetTextFont(42);
   text.SetTextSize(TextSize);
   text.Draw();

   if(PrintPageNumber == true)
      InsertPageNumber(canvas, NextPageNumber);
   canvas.Print(FileName.c_str(), Option.c_str());
   NextPageNumber = NextPageNumber + 1;

   if(AutomaticHomeButton == true)
      InsertHomeButtonAbsolute(HomeButtonX, HomeButtonY, HomeButtonSize, HomeButtonDestination);
}

void PdfFileHelper::AddTextPage(string Text, double X, double Y, double TextSize)
{
   TCanvas canvas;

   TLatex text(X, Y, Text.c_str());
   text.SetTextFont(42);
   text.SetTextSize(TextSize);
   text.Draw();

   if(PrintPageNumber == true)
      InsertPageNumber(canvas, NextPageNumber);
   canvas.Print(FileName.c_str(), Option.c_str());
   NextPageNumber = NextPageNumber + 1;

   if(AutomaticHomeButton == true)
      InsertHomeButtonAbsolute(HomeButtonX, HomeButtonY, HomeButtonSize, HomeButtonDestination);
}

void PdfFileHelper::AddTextPage(vector<string> Text, double X, double Y, double TextSize)
{
   TCanvas canvas;

   vector<TLatex *> texts;

   for(int i = 0; i < (int)Text.size(); i++)
   {
      texts.push_back(new TLatex(X, Y - i * TextSize * 1.75, Text[i].c_str()));
      texts[i]->SetName(Form("TextLine%d", i));
      texts[i]->SetTextFont(42);
      texts[i]->SetTextSize(TextSize);
   }

   for(int i = 0; i < (int)Text.size(); i++)
      texts[i]->Draw();

   if(PrintPageNumber == true)
      InsertPageNumber(canvas, NextPageNumber);
   canvas.Print(FileName.c_str(), Option.c_str());
   NextPageNumber = NextPageNumber + 1;

   if(AutomaticHomeButton == true)
      InsertHomeButtonAbsolute(HomeButtonX, HomeButtonY, HomeButtonSize, HomeButtonDestination);

   for(int i = 0; i < (int)Text.size(); i++)
      delete texts[i];
   texts.clear();
}

void PdfFileHelper::AddTimeStampPage()
{
   time_t CurrentTime = time(NULL);

   string str = "Generated at ";
   str = str + ctime(&CurrentTime);

   AddTextPage(str);
}

void PdfFileHelper::AddTableOfContentPage(vector<string> Items, vector<string> Destinations, double X, double Y,
   double TextSize)
{
   cout << "Table of content not inplemented yet!" << endl;

   return;

   TCanvas canvas;

   vector<TLatex *> texts;

   texts.push_back(new TLatex(0.37, Y + 0.1, "Table of Contents"));
   texts[0]->SetName("TextLine00000");
   texts[0]->SetTextSize(0.05);

   for(int i = 0; i < (int)Items.size(); i++)
   {
      texts.push_back(new TLatex(X, Y - i * TextSize * 1.5, Items[i].c_str()));
      texts[i+1]->SetName(Form("TextLine%d", i));
      texts[i+1]->SetTextSize(TextSize);
   }

   for(int i = 0; i < (int)texts.size(); i++)
      texts[i]->Draw();

   if(PrintPageNumber == true)
      InsertPageNumber(canvas, NextPageNumber);
   canvas.Print(FileName.c_str(), Option.c_str());
   NextPageNumber = NextPageNumber + 1;

   if(AutomaticHomeButton == true)
      InsertHomeButtonAbsolute(HomeButtonX, HomeButtonY, HomeButtonSize, HomeButtonDestination);

   for(int i = 0; i < (int)Items.size() && i < (int)Destinations.size(); i++)
   {
      if(Destinations[i] == "")
         continue;

      double TextWidth = 0.8;

      double MagicNumber = 4.235;   // will need to change to something better
      double X1 = canvas.XtoPixel(X - TextSize * 0.2) * MagicNumber;
      double X2 = canvas.XtoPixel(X + TextWidth + TextSize * 0.2) * MagicNumber;
      double Y1 = canvas.YtoPixel(1 - (Y - i * TextSize * 1.5) + TextSize * 0.2) * MagicNumber;
      double Y2 = canvas.YtoPixel(1 - (Y - i * TextSize * 1.5) - TextSize * 1) * MagicNumber;

      stringstream box;
      box << "[ /Rect [" << X1 << " " << Y1 << " " << X2 << " " << Y2 << "]";

      PrintLineToPdfFile("");
      PrintLineToPdfFile(box.str());
      PrintLineToPdfFile("/Color [0 0 1]");
      PrintLineToPdfFile("/Dest /" + Destinations[i]);
      PrintLineToPdfFile("/Subtype /Link");
      PrintLineToPdfFile("/ANN pdfmark");
      PrintLineToPdfFile("");
   }

   for(int i = 0; i < (int)texts.size(); i++)
      delete texts[i];
   texts.clear();
}

void PdfFileHelper::PrintStringToPdfFile(string String)
{
   TPDF *PDFHandle = (TPDF *)gROOT->GetListOfSpecials()->FindObject(FileName.c_str());
   if(PDFHandle == NULL)   // something's wrong.  file not opened yet?   anyways do nothing.
      return;

   PDFHandle->PrintStr(String.c_str());
}

void PdfFileHelper::PrintLineToPdfFile(string Line)
{
   PrintStringToPdfFile(Line + "\n");
}

void PdfFileHelper::InsertNamedDestination(string Name)
{
   cout << "Print string to PS file not implemented yet!" << endl;

   return;

   PrintLineToPdfFile("");
   // PrintLineToPdfFile("%% Defining named destination: " + Name);
   PrintLineToPdfFile("[ /Dest /" + Name);
   PrintLineToPdfFile("   /DEST pdfmark");
   PrintLineToPdfFile("");
}

void PdfFileHelper::InsertBallLinkAbsolute(int X, int Y, int Radius, string Destination)
{
   cout << "Print string to PS file not implemented yet!" << endl;

   return;

   stringstream str1;
   str1 << "newpath " << X << " " << Y << " " << Radius << " 0 360 arc";
   stringstream str2;
   str2 << "[ /Rect [" << X - Radius << " " << Y - Radius
      << " " << X + Radius << " " << Y + Radius << "]";

   PrintLineToPdfFile("");
   PrintLineToPdfFile(str1.str());
   PrintLineToPdfFile("fill");
   PrintLineToPdfFile("");
   PrintLineToPdfFile(str2.str());
   PrintLineToPdfFile("/Color [0 0 1]");
   PrintLineToPdfFile("/Dest /" + Destination);
   PrintLineToPdfFile("/Subtype /Link");
   PrintLineToPdfFile("/ANN pdfmark");
   PrintLineToPdfFile("");
}

void PdfFileHelper::InsertHomeButtonAbsolute(double X, double Y, double Size, string Destination)
{
   double H = 0.4;   // height and width of the small box in the bottom, in units of "Size"
   double W = 0.75;

   double X2 = 0.7;   // chimney left side x (lower side y is always 0.5), and width/height in units of "Size"
   double W2 = 0.125;
   double H2 = 0.4;

   double LowerLeftX = X - Size / 2;
   double LowerLeftY = Y - Size / 2;

   double PointsX[11] = {0};
   double PointsY[11] = {0};

   PointsX[0] = 0;              PointsY[0] = H;
   PointsX[1] = 0.5;            PointsY[1] = 1;

   PointsX[2] = X2;             PointsY[2] = (1 - X2) / (1 - 0.5) * (1 - H) + H;
   PointsX[3] = X2;             PointsY[3] = H2 + H;
   PointsX[4] = X2 + W2;        PointsY[4] = H2 + H;
   PointsX[5] = X2 + W2;        PointsY[5] = (1 - X2 - W2) / (1 - 0.5) * (1 - H) + H;

   PointsX[6] = 1;              PointsY[6] = H;
   PointsX[7] = 0.5 + W / 2;    PointsY[7] = H;
   PointsX[8] = 0.5 + W / 2;    PointsY[8] = 0;
   PointsX[9] = 0.5 - W / 2;    PointsY[9] = 0;
   PointsX[10] = 0.5 - W / 2;   PointsY[10] = H;

   TPDF *PDFHandle = (TPDF *)gROOT->GetListOfSpecials()->FindObject(FileName.c_str());
   
   PrintLineToPdfFile("");
   for(int i = 0; i < 11; i++)
   {
      double X = PDFHandle->UtoPDF(LowerLeftX + PointsX[i] * Size);
      double Y = PDFHandle->VtoPDF(LowerLeftY + PointsY[i] * Size);

      char TempString[1000];
      sprintf(TempString, " %f %f ", X, Y);
      
      if(i == 0)
         PrintStringToPdfFile(string(TempString) + " m");
      else
         PrintStringToPdfFile(string(TempString) + " l");
      
   }
   PrintLineToPdfFile(" f*");
   
   /*
   stringstream box;
   box << "[ /Rect [" << PDFHandle->UtoPDF(LowerLeftX) << " " << PDFHandle->VtoPDF(LowerLeftY) << " "
      << PDFHandle->UtoPDF(LowerLeftX + Size) << " " << PDFHandle->VtoPDF(LowerLeftY + Size) << "]";

   PrintLineToPdfFile("");
   PrintLineToPdfFile(box.str());
   PrintLineToPdfFile("  /Open true");
   PrintLineToPdfFile("  /Color [0 0 1]");
   PrintLineToPdfFile("  /Subtype /Square");
   // PrintLineToPdfFile("/Dest /" + Destination);
   // PrintLineToPdfFile("/Subtype /Link");
   PrintLineToPdfFile("/ANN pdfmark");
   PrintLineToPdfFile("");
   */
}

void PdfFileHelper::SetAutomaticHomeButton(bool newvalue, string Destination, double X, double Y, double Size)
{
   AutomaticHomeButton = newvalue;
   HomeButtonDestination = Destination;
   HomeButtonX = X;
   HomeButtonY = Y;
   HomeButtonSize = Size;
}

void PdfFileHelper::SetPageNumber(bool printnumber)
{
   PrintPageNumber = printnumber;
}

void PdfFileHelper::InsertPageNumber(TCanvas &Canvas, int PageNumber)
{
   if(PageNumber < 0)
      PageNumber = NextPageNumber;

   Canvas.cd();

   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.02);
   Latex.SetTextAlign(31);
   Latex.SetNDC(true);
   Latex.DrawLatex(0.99, 0.01, Form("%d", PageNumber));
}

#endif


