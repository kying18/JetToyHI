#include <iostream>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("input");
   string OutputBase = CL.Get("output");
   double MinPT = CL.GetDouble("minpt", 200);
   string Title = CL.Get("title", "");

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("Lund Plane");
    
   for(string FileName : InputFileNames)
   {
        string title = "Jet Constituent Multiplicity";
        if (FileName.find("ZJet")){
            title += " (ZJet)";
        }

        TH1D HJetN("HJetN", title.c_str(), 100, 0, 100);
        TFile File(FileName.c_str());

        TTree *Tree = (TTree *)File.Get("JetTree");

        if(Tree == nullptr)
        {
            File.Close();
            continue;
        }

        vector<double> *SignalJetNConstituent = nullptr;
        vector<double> *SignalJetPt = nullptr;
        vector<double> *Weight = nullptr;

        Tree->SetBranchAddress("SignalJetNConstituent", &SignalJetNConstituent);
        Tree->SetBranchAddress("SignalJetPt", &SignalJetPt);
        Tree->SetBranchAddress("EventWeight", &Weight);

        int EntryCount = Tree->GetEntries();

        for(int iE = 0; iE < EntryCount; iE++)
        {
            Tree->GetEntry(iE);

            if(SignalJetPt == nullptr)
                continue;

            int NJet = SignalJetPt->size();
            for(int iJ = 0; iJ < NJet; iJ++)
            {
                if((*SignalJetPt)[iJ] < MinPT)
                continue;
                
                HJetN.Fill((*SignalJetNConstituent)[iJ], (*Weight)[0]);
            }
        }
        PdfFile.AddPlot(HJetN, "", true);

        File.Close();
        
   }

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




