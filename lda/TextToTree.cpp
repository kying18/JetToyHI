#include <iostream>
#include <sstream>
#include <cstdlib>
using namespace std;

#include "TFile.h"
#include "TTree.h"

int main(int argc, char *argv[]);
vector<string> Split(string All);

int main(int argc, char *argv[])
{
   for(int i = 1; i < argc; i++)
   {
      if(string(argv[i]) == "-h" || string(argv[i]) == "--h" || string(argv[i]) == "--help")
      {
         cerr << "Usage: " << argv[0] << endl;
         cerr << "   " << argv[0] << " FileName NumberOfColumns [ColumnNames]" << endl;
         return 0;
      }
   }

   if(argc != 3 && argc != 4)
   {
      cerr << "Usage: " << argv[0] << endl;
      cerr << "   " << argv[0] << " FileName NumberOfColumns [ColumnNames]" << endl;
      return -1;
   }

   string FileName = argv[1];
   int ColumnCount = atoi(argv[2]);

   vector<string> BranchNames;
   if(argc == 4)
      BranchNames = Split(argv[3]);

   TFile File(FileName.c_str(), "RECREATE");

   TTree Tree("Tree", "Tree");

   vector<double> Variables(ColumnCount);

   for(int i = 0; i < ColumnCount; i++)
   {
      if(i < (int)BranchNames.size())
      {
         // cout << BranchNames[i] << endl;
         Tree.Branch(BranchNames[i].c_str(), &Variables[i], (BranchNames[i] + "/D").c_str());
      }
      else
         Tree.Branch(Form("B%d", i + 1), &Variables[i], Form("B%d/D", i + 1));
   }

   int Index = 0;

   while(cin)
   {
      cin >> Variables[Index];
      Index = Index + 1;

      if(Index < ColumnCount)
         continue;

      Index = 0;
      Tree.Fill();

      for(int i = 0; i < ColumnCount; i++)
         Variables[i] = 0;
   }

   Tree.Write();

   File.Close();

   return 0;
}

vector<string> Split(string All)
{
   All = All + ":";
   int index = 0;

   vector<string> Result;

   while(index < All.size())
   {
      int index2 = All.find(':', index);
      if(index2 - index > 0)
         Result.push_back(All.substr(index, index2 - index));
      index = index2 + 1;
   }

   return Result;
}
