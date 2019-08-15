./root.sh PlotBeautiful.cpp
./a.out -Input processed/20190814-LundPartonPythiaUEOn.root -Tag "PYTHIA8 Pthat300 Jet pT>350 GeV" -Cut "MatchedJetPt>350" -Output PYTHIA8_Pthat300_JetPt350.pdf
./a.out -Input processed/20190814-LundPartonPythiaUEOn50.root -Tag "PYTHIA8 Pthat50 Jet pT>70 GeV" -Cut "MatchedJetPt>70" -Output PYTHIA8_Pthat50_JetPt70.pdf

./a.out -Input processed/20190815-Lund_Meow_PythiaPthat50_r02.root -Tag "PYTHIA8 Pthat50 Jet R=0.2 pT>70 GeV" -Cut "MatchedJetPt>70" -Output PYTHIA8_Pthat50_R02_JetPt70.pdf
./a.out -Input processed/20190815-Lund_Meow_PythiaPthat50_r08.root -Tag "PYTHIA8 Pthat50 Jet R=0.8 pT>70 GeV" -Cut "MatchedJetPt>70" -Output PYTHIA8_Pthat50_R08_JetPt70.pdf

