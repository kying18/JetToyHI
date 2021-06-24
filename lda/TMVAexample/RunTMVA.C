#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

int RunTMVA()
{
   TMVA::Tools::Instance();

   TFile InputType1("JetType1.root");
   TFile InputType2("JetType2.root");

   TTree *TreeType1 = (TTree *)InputType1.Get("Tree");
   TTree *TreeType2 = (TTree *)InputType2.Get("Tree");

   TString outfileName = "TMVAOutput.root";
   TFile* outputFile = TFile::Open(outfileName, "RECREATE");

   TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outputFile,
      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

   dataloader->AddVariable("V1", 'D');
   dataloader->AddVariable("V2", 'D');
   dataloader->AddVariable("V3", 'D');
   dataloader->AddVariable("V4", 'D');
   dataloader->AddVariable("V5", 'D');

   dataloader->AddSpectator("VTotal", 'D');

   dataloader->AddSignalTree(TreeType1);
   dataloader->AddBackgroundTree(TreeType2);

   dataloader->PrepareTrainingAndTestTree("", "",
      "nTrain_Signal=2500:nTrain_Background=2500:SplitMode=Random:NormMode=NumEvents:!V" );

   factory->BookMethod(dataloader, TMVA::Types::kLD, "LD",
      "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   factory->TrainAllMethods();
   factory->TestAllMethods();
   factory->EvaluateAllMethods();

   outputFile->Close();

   delete factory;
   delete dataloader;
   
   if(!gROOT->IsBatch())
      TMVA::TMVAGui( outfileName );

   return 0;
}

int main(int argc, char** argv)
{
   return RunTMVA();
}