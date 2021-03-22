#ifndef SetStyle_h_ikdofqwjito3jgthyqiwko3jhigtyq2jghufolqjgthy
#define SetStyle_h_ikdofqwjito3jgthyqiwko3jhigtyq2jghufolqjgthy

#include <cstdlib>

#include "TStyle.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TColor.h"
#include "TH2D.h"
#include "TCanvas.h"

void SetThesisStyle();
void SetThesisBigStyle();
void SetThesisRegularStyle();
void SetThesisSmallStyle();
void SetThumbStyle();
double ThesisNDCMappingX(double OldX);
double ThesisNDCMappingY(double OldY);
double ThesisNDCMappingBigX(double OldX);
double ThesisNDCMappingBigY(double OldY);
double ThesisNDCMappingRegularX(double OldX);
double ThesisNDCMappingRegularY(double OldY);
double ThesisNDCMappingSmallX(double OldX);
double ThesisNDCMappingSmallY(double OldY);
void SetStyle();
void SetStyleOriginal();
void SetTDRStyle();
void SetTDRMikkoStyle();
void SetTDRMikkoCanvas(TCanvas &Canvas, bool IsSquare);
void SetTDRMikkoWorld(TH2D &HWorld, bool IsSquare);
void AddCMSPreliminary(double X = 0.15, double Y = 0.875, double Luminosity = -1, double FontSize = 0.03);
void AddCMS(double X = 0.15, double Y = 0.875, double Luminosity = -1, double FontSize = 0.03);
void AddCMSSimulation(double X = 0.15, double Y = 0.875, double Luminosity = -1, double FontSize = 0.03);
void AddHeader(string AdditionalComment = "", bool Colored = false);

void SetThesisStyle()
{
   gStyle->SetFrameBorderMode(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetCanvasColor(0);
   gStyle->SetStatColor(0);
   gStyle->SetFillColor(0);

   gStyle->SetPaperSize(20,26);
   gStyle->SetCanvasDefH(1024);
   gStyle->SetCanvasDefW(1024 * 1.05);
   gStyle->SetPadTopMargin(0.1);
   gStyle->SetPadRightMargin(0.15 / 1.05);
   gStyle->SetPadBottomMargin(0.1);
   gStyle->SetPadLeftMargin(0.1);

   // gStyle->SetTextFont(42);
   // gStyle->SetTextSize(0.08);
   gStyle->SetLabelFont(42,"x");
   gStyle->SetLabelFont(42,"y");
   gStyle->SetLabelFont(42,"z");
   gStyle->SetLabelSize(0.035,"x");
   gStyle->SetTitleSize(0.035,"x");
   gStyle->SetLabelSize(0.035,"y");
   gStyle->SetTitleSize(0.035,"y");
   gStyle->SetLabelSize(0.035,"z");
   gStyle->SetTitleSize(0.035,"z");
   gStyle->SetTitleOffset(1.10,"x");
   gStyle->SetTitleOffset(1.20,"y");
   gStyle->SetTitleOffset(1.28,"z");

   gStyle->SetMarkerStyle(20);
   gStyle->SetHistLineWidth(1);
   // gStyle->SetHistLineWidth(1.85);
   // gStyle->SetHistLineWidth(3.85);
   gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

   gStyle->SetErrorX(0.001);

   // gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetOptStat(1100);
   gStyle->SetOptStat(1);
   gStyle->SetOptFit(1111);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetLegendFont(42);
   gStyle->SetLegendBorderSize(0);
   gStyle->SetLegendFillColor(0);

   const Int_t NRGBs = 5;
   const Int_t NCont = 99;

   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
}

void SetThesisBigStyle()
{
   SetThesisStyle();
}

void SetThesisRegularStyle()
{
   gStyle->SetFrameBorderMode(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetCanvasColor(0);
   gStyle->SetStatColor(0);
   gStyle->SetFillColor(0);

   gStyle->SetPaperSize(20,26);
   gStyle->SetCanvasDefH(1024 * 1.20);
   gStyle->SetCanvasDefW(1024 * 1.20);
   gStyle->SetPadTopMargin(0.2 / 1.2);
   gStyle->SetPadRightMargin(0.2 / 1.2);
   gStyle->SetPadBottomMargin(0.2 / 1.2);
   gStyle->SetPadLeftMargin(0.2 / 1.2);

   // gStyle->SetTextFont(42);
   // gStyle->SetTextSize(0.08);
   gStyle->SetLabelFont(42,"x");
   gStyle->SetLabelFont(42,"y");
   gStyle->SetLabelFont(42,"z");
   gStyle->SetLabelSize(0.070 / 1.30,"x");
   gStyle->SetTitleSize(0.070 / 1.30,"x");
   gStyle->SetLabelSize(0.070 / 1.30,"y");
   gStyle->SetTitleSize(0.070 / 1.30,"y");
   gStyle->SetLabelSize(0.070 / 1.30,"z");
   gStyle->SetTitleSize(0.070 / 1.30,"z");
   gStyle->SetTitleOffset(1.20,"x");
   gStyle->SetTitleOffset(1.40,"y");
   gStyle->SetTitleOffset(1.56,"z");

   gStyle->SetMarkerStyle(20);
   gStyle->SetHistLineWidth(1);
   // gStyle->SetHistLineWidth(1.85);
   // gStyle->SetHistLineWidth(3.85);
   gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

   gStyle->SetErrorX(0.001);

   // gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetOptStat(1100);
   gStyle->SetOptStat(1);
   gStyle->SetOptFit(1111);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetLegendFont(42);
   gStyle->SetLegendBorderSize(0);
   gStyle->SetLegendFillColor(0);

   const Int_t NRGBs = 5;
   const Int_t NCont = 99;

   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
}

void SetThesisSmallStyle()
{
   gStyle->SetFrameBorderMode(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetCanvasColor(0);
   gStyle->SetStatColor(0);
   gStyle->SetFillColor(0);

   gStyle->SetPaperSize(20,26);
   gStyle->SetCanvasDefH(1024);
   gStyle->SetCanvasDefW(1024 * 1.05);
   gStyle->SetPadTopMargin(0.1);
   gStyle->SetPadRightMargin(0.15 / 1.05);
   gStyle->SetPadBottomMargin(0.1);
   gStyle->SetPadLeftMargin(0.1);

   // gStyle->SetTextFont(42);
   // gStyle->SetTextSize(0.08);
   gStyle->SetLabelFont(42,"x");
   gStyle->SetLabelFont(42,"y");
   gStyle->SetLabelFont(42,"z");
   gStyle->SetLabelSize(0.035,"x");
   gStyle->SetTitleSize(0.035,"x");
   gStyle->SetLabelSize(0.035,"y");
   gStyle->SetTitleSize(0.035,"y");
   gStyle->SetLabelSize(0.035,"z");
   gStyle->SetTitleSize(0.035,"z");
   gStyle->SetTitleOffset(1.10,"x");
   gStyle->SetTitleOffset(1.20,"y");
   gStyle->SetTitleOffset(1.28,"z");

   gStyle->SetMarkerStyle(20);
   gStyle->SetHistLineWidth(1);
   // gStyle->SetHistLineWidth(1.85);
   // gStyle->SetHistLineWidth(3.85);
   gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

   gStyle->SetErrorX(0.001);

   // gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetOptStat(1100);
   gStyle->SetOptStat(1);
   gStyle->SetOptFit(1111);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetLegendFont(42);
   gStyle->SetLegendBorderSize(0);
   gStyle->SetLegendFillColor(0);

   const Int_t NRGBs = 5;
   const Int_t NCont = 99;

   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
}

void SetThumbStyle()
{
   gStyle->SetFrameBorderMode(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetCanvasColor(0);
   gStyle->SetStatColor(0);
   gStyle->SetFillColor(0);

   gStyle->SetPaperSize(20,26);
   gStyle->SetCanvasDefH(1024);
   gStyle->SetCanvasDefW(1024);
   gStyle->SetPadTopMargin(0.1);
   gStyle->SetPadRightMargin(0.1);
   gStyle->SetPadBottomMargin(0.15);
   gStyle->SetPadLeftMargin(0.15);

   // gStyle->SetTextFont(42);
   // gStyle->SetTextSize(0.08);
   gStyle->SetLabelFont(42,"x");
   gStyle->SetLabelFont(42,"y");
   gStyle->SetLabelFont(42,"z");
   gStyle->SetLabelSize(0.05,"x");
   gStyle->SetTitleSize(0.06,"x");
   gStyle->SetLabelSize(0.05,"y");
   gStyle->SetTitleSize(0.06,"y");
   gStyle->SetLabelSize(0.05,"z");
   gStyle->SetTitleSize(0.06,"z");
   gStyle->SetTitleOffset(1.10,"x");
   gStyle->SetTitleOffset(1.10,"y");
   gStyle->SetTitleOffset(1.28,"z");

   gStyle->SetMarkerStyle(20);
   gStyle->SetHistLineWidth(1);
   // gStyle->SetHistLineWidth(1.85);
   // gStyle->SetHistLineWidth(3.85);
   gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

   gStyle->SetErrorX(0.001);

   // gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetOptStat(1100);
   gStyle->SetOptStat(1);
   gStyle->SetOptFit(1111);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetLegendFont(42);
   gStyle->SetLegendBorderSize(0);
   gStyle->SetLegendFillColor(0);

   const Int_t NRGBs = 5;
   const Int_t NCont = 99;

   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
}

double ThesisNDCMappingX(double OldX)
{
   return OldX / 1.05;
}

double ThesisNDCMappingY(double OldY)
{
   return OldY;
}

double ThesisNDCMappingBigX(double OldX)
{
   return ThesisNDCMappingX(OldX);
}

double ThesisNDCMappingBigY(double OldY)
{
   return ThesisNDCMappingY(OldY);
}

double ThesisNDCMappingRegularX(double OldX)
{
   return OldX / 1.2 + 0.1;
}

double ThesisNDCMappingRegularY(double OldY)
{
   return OldY / 1.2 + 0.1;
}

double ThesisNDCMappingSmallX(double OldX)
{
   return 0;
}

double ThesisNDCMappingSmallY(double OldY)
{
   return 0;
}

void SetStyle()
{
   gStyle->SetFrameBorderMode(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetCanvasColor(0);
   gStyle->SetStatColor(0);
   gStyle->SetFillColor(0);

   gStyle->SetPaperSize(20,26);
   // gStyle->SetPadTopMargin(0.05);
   // gStyle->SetPadRightMargin(0.05);
   // gStyle->SetPadBottomMargin(0.16);
   // gStyle->SetPadLeftMargin(0.12);

   // gStyle->SetTextFont(42);
   // gStyle->SetTextSize(0.08);
   gStyle->SetLabelFont(42,"x");
   gStyle->SetLabelFont(42,"y");
   gStyle->SetLabelFont(42,"z");
   gStyle->SetLabelSize(0.035,"x");
   gStyle->SetTitleSize(0.035,"x");
   gStyle->SetLabelSize(0.035,"y");
   gStyle->SetTitleSize(0.035,"y");
   gStyle->SetLabelSize(0.035,"z");
   gStyle->SetTitleSize(0.035,"z");

   gStyle->SetMarkerStyle(20);
   gStyle->SetHistLineWidth(1);
   // gStyle->SetHistLineWidth(1.85);
   // gStyle->SetHistLineWidth(3.85);
   gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

   gStyle->SetErrorX(0.001);

   // gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gStyle->SetOptStat(1100);
   gStyle->SetOptStat(1);
   gStyle->SetOptFit(1111);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   const Int_t NRGBs = 5;
   const Int_t NCont = 99;

   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
}

void SetStyleOriginal()
{
   gStyle->SetFrameBorderMode(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetCanvasColor(0);
   gStyle->SetStatColor(0);
   gStyle->SetFillColor(0);

   gStyle->SetPaperSize(20,26);
   gStyle->SetPadTopMargin(0.05);
   gStyle->SetPadRightMargin(0.05);
   gStyle->SetPadBottomMargin(0.16);
   gStyle->SetPadLeftMargin(0.12);

   gStyle->SetTextFont(132);
   gStyle->SetTextSize(0.08);
   gStyle->SetLabelFont(132,"x");
   gStyle->SetLabelFont(132,"y");
   gStyle->SetLabelFont(132,"z");
   gStyle->SetLabelSize(0.05,"x");
   gStyle->SetTitleSize(0.06,"x");
   gStyle->SetLabelSize(0.05,"y");
   gStyle->SetTitleSize(0.06,"y");
   gStyle->SetLabelSize(0.05,"z");
   gStyle->SetTitleSize(0.06,"z");

   gStyle->SetMarkerStyle(20);
   gStyle->SetHistLineWidth(1);
   // gStyle->SetHistLineWidth(1.85);
   // gStyle->SetHistLineWidth(3.85);
   gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

   gStyle->SetErrorX(0.001);

   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   gROOT->SetStyle("Plain");
   gStyle->SetOptStat(1100);
   gStyle->SetOptStat(1);
   gStyle->SetOptFit(1111);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   const Int_t NRGBs = 5;
   const Int_t NCont = 99;

   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
}

void SetTDRStyle()
{
// For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);

// For the Pad:
  gStyle->SetPadBorderMode(0);
  // gStyle->SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

// For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

// For the histo:
  // gStyle->SetHistFillColor(1);
  // gStyle->SetHistFillStyle(0);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  // gStyle->SetLegoInnerR(Float_t rad = 0.5);
  // gStyle->SetNumberContours(Int_t number = 20);

  gStyle->SetEndErrorSize(2);
  //gStyle->SetErrorMarker(20);  // Seems to give an error
  gStyle->SetErrorX(0.);
  
  gStyle->SetMarkerStyle(20);

//For the fit/function:
  gStyle->SetOptFit(1);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);

//For the date:
  gStyle->SetOptDate(0);
  // gStyle->SetDateX(Float_t x = 0.01);
  // gStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  gStyle->SetStatColor(kWhite);
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.025);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatW(0.15);
  // gStyle->SetStatStyle(Style_t style = 1001);
  // gStyle->SetStatX(Float_t x = 0);
  // gStyle->SetStatY(Float_t y = 0);

// Margins:
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.02);

// For the Global title:
  gStyle->SetOptTitle(1);    // 0=No Title
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);
  // gStyle->SetTitleH(0); // Set the height of the title box
  // gStyle->SetTitleW(0); // Set the width of the title box
  // gStyle->SetTitleX(0); // Set the position of the title box
  // gStyle->SetTitleY(0.985); // Set the position of the title box
  // gStyle->SetTitleStyle(Style_t style = 1001);
  // gStyle->SetTitleBorderSize(2);

// For the axis titles:
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.06, "XYZ");
  // gStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // gStyle->SetTitleYSize(Float_t size = 0.02);
  gStyle->SetTitleXOffset(0.9);
  gStyle->SetTitleYOffset(1.25);
  // gStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.05, "XYZ");

// For the axis:
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(0);  // 0=Text labels (and tics) only on bottom, 1=Text labels on top and bottom
  gStyle->SetPadTickY(1);

// Change for log plots:
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);

// Postscript options:
  gStyle->SetPaperSize(20.,20.);
  // gStyle->SetLineScalePS(Float_t scale = 3);
  // gStyle->SetLineStyleString(Int_t i, const char* text);
  // gStyle->SetHeaderPS(const char* header);
  // gStyle->SetTitlePS(const char* pstitle);

  // gStyle->SetBarOffset(Float_t baroff = 0.5);
  // gStyle->SetBarWidth(Float_t barwidth = 0.5);
  // gStyle->SetPaintTextFormat(const char* format = "g");
  // gStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // gStyle->SetTimeOffset(Double_t toffset);
  // gStyle->SetHistMinimumZero(kTRUE);

  //gROOT->ForceStyle();  // Try this if stuff doesn't work right
}

void SetTDRMikkoStyle()
{
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);
  
  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  // tdrStyle->SetErrorMarker(20);
  //tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);
  
  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.02);

  // For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->SetHatchesLineWidth(5);
  tdrStyle->SetHatchesSpacing(0.05);

  tdrStyle->cd();

}

void SetTDRMikkoCanvas(TCanvas &Canvas, bool IsSquare)
{
   int W = (IsSquare ? 600 : 800);
   int H = (IsSquare ? 600 : 600);
   int WRef = (IsSquare ? 600 : 800);
   int HRef = (IsSquare ? 600 : 600);

   Canvas.SetCanvasSize(W, H);

   double T = (IsSquare ? 0.07 * HRef : 0.08 * HRef);
   double B = (IsSquare ? 0.13 * HRef : 0.12 * HRef); 
   double L = (IsSquare ? 0.15 * WRef : 0.12 * WRef);
   double R = (IsSquare ? 0.05 * WRef : 0.04 * WRef);

   Canvas.SetFillColor(0);
   Canvas.SetBorderMode(0);
   Canvas.SetFrameFillStyle(0);
   Canvas.SetFrameBorderMode(0);
   Canvas.SetLeftMargin(L / W);
   Canvas.SetRightMargin(R / W);
   Canvas.SetTopMargin(T / H);
   Canvas.SetBottomMargin(B / H);
}

void SetTDRMikkoWorld(TH2D &HWorld, bool IsSquare)
{
   HWorld.SetStats(0);
   HWorld.GetYaxis()->SetTitleOffset(IsSquare ? 1.25 : 1);
   HWorld.GetXaxis()->SetTitleOffset(IsSquare ? 1.0 : 0.9);
}

void AddCMSPreliminary(double X, double Y, double Luminosity, double FontSize)
{
   TLatex latex;
   latex.SetTextAlign(12);
   latex.SetTextSize(FontSize);
   latex.SetTextFont(42);
   latex.SetNDC();

   string Unit = "pb^{-1}";
   if(Luminosity < 1 && Luminosity >= 0.001)
   {
      Unit = "nb^{-1}";
      Luminosity = Luminosity * 1000;
   }
   if(Luminosity < 0.001 && Luminosity >= 0.000001)
   {
      Unit = "#mu b^{-1}";
      Luminosity = Luminosity * 1000000;
   }
   if(Luminosity < 0.000001 && Luminosity >= 0.000000001)
   {
      Unit = "mb^{-1}";
      Luminosity = Luminosity * 1000000000;
   }
   if(Luminosity < 1000000 && Luminosity >= 1000)
   {
      Unit = "fb^{-1}";
      Luminosity = Luminosity / 1000;
   }
   if(Luminosity < 1000000000 && Luminosity >= 1000000)
   {
      Unit = "ab^{-1}";
      Luminosity = Luminosity / 1000000;
   }

   if(Luminosity > 0 && Luminosity < 10)
      latex.DrawLatex(X, Y, Form("CMS Preliminary #sqrt{s}=7 TeV  #int L dt = %.1f %s", Luminosity, Unit.c_str()));
   else if(Luminosity >= 0)
      latex.DrawLatex(X, Y, Form("CMS Preliminary #sqrt{s}=7 TeV  #int L dt = %.0f %s", Luminosity, Unit.c_str()));
   else
      latex.DrawLatex(X, Y, "CMS Preliminary #sqrt{s}=7 TeV");
}

void AddCMS(double X, double Y, double Luminosity, double FontSize)
{
   TLatex latex;
   latex.SetTextAlign(12);
   latex.SetTextSize(FontSize);
   latex.SetTextFont(42);
   latex.SetNDC();

   string Unit = "pb^{-1}";
   if(Luminosity < 1 && Luminosity >= 0.001)
   {
      Unit = "nb^{-1}";
      Luminosity = Luminosity * 1000;
   }
   if(Luminosity < 0.001 && Luminosity >= 0.000001)
   {
      Unit = "#mu b^{-1}";
      Luminosity = Luminosity * 1000000;
   }
   if(Luminosity < 0.000001 && Luminosity >= 0.000000001)
   {
      Unit = "mb^{-1}";
      Luminosity = Luminosity * 1000000000;
   }
   if(Luminosity < 1000000 && Luminosity >= 1000)
   {
      Unit = "fb^{-1}";
      Luminosity = Luminosity / 1000;
   }
   if(Luminosity < 1000000000 && Luminosity >= 1000000)
   {
      Unit = "ab^{-1}";
      Luminosity = Luminosity / 1000000;
   }

   if(Luminosity > 0 && Luminosity < 10)
      latex.DrawLatex(X, Y, Form("CMS #sqrt{s}=7 TeV  #int L dt = %.1f %s", Luminosity, Unit.c_str()));
   else if(Luminosity >= 0)
      latex.DrawLatex(X, Y, Form("CMS #sqrt{s}=7 TeV  #int L dt = %.0f %s", Luminosity, Unit.c_str()));
   else
      latex.DrawLatex(X, Y, "CMS #sqrt{s}=7 TeV");
}

void AddCMSSimulation(double X, double Y, double Luminosity, double FontSize)
{
   TLatex latex;
   latex.SetTextAlign(12);
   latex.SetTextSize(FontSize);
   latex.SetTextFont(42);
   latex.SetNDC();

   string Unit = "pb^{-1}";
   if(Luminosity < 1 && Luminosity >= 0.001)
   {
      Unit = "nb^{-1}";
      Luminosity = Luminosity * 1000;
   }
   if(Luminosity < 0.001 && Luminosity >= 0.000001)
   {
      Unit = "#mu b^{-1}";
      Luminosity = Luminosity * 1000000;
   }
   if(Luminosity < 0.000001 && Luminosity >= 0.000000001)
   {
      Unit = "mb^{-1}";
      Luminosity = Luminosity * 1000000000;
   }
   if(Luminosity < 1000000 && Luminosity >= 1000)
   {
      Unit = "fb^{-1}";
      Luminosity = Luminosity / 1000;
   }
   if(Luminosity < 1000000000 && Luminosity >= 1000000)
   {
      Unit = "ab^{-1}";
      Luminosity = Luminosity / 1000000;
   }

   if(Luminosity > 0)
      latex.DrawLatex(X, Y, Form("CMS Simulation #sqrt{s}=7 TeV  #int L dt = %.3f %s", Luminosity, Unit.c_str()));
   else
      latex.DrawLatex(X, Y, "CMS Simulation #sqrt{s}=7 TeV");
}

void AddHeader(string AdditionalComment, bool Colored)
{
   TLatex Latex;
   Latex.SetNDC();
   Latex.SetTextAlign(12);
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.01);
   if(Colored == true)
      Latex.SetTextColor(kBlack);
   else
      Latex.SetTextColor(kWhite);
   
   time_t CurrentTime = time(NULL);
   string str = "Current time is ";
   str = str + ctime(&CurrentTime);
   Latex.DrawLatex(0.01, 0.04, str.c_str());

   char *X = std::getenv("PWD");
   str = string("Working dir ") + (X ? X : "N/A");
   Latex.DrawLatex(0.01, 0.03, str.c_str());
   
   X = std::getenv("HOSTNAME");
   str = string("Host ") + (X ? X : "N/A");
   Latex.DrawLatex(0.01, 0.02, str.c_str());

   Latex.DrawLatex(0.01, 0.01, AdditionalComment.c_str());
}



#endif
