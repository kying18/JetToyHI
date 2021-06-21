g++ PlotJetShapeTopics.cpp $(root-config --cflags --libs) -O2 -o "plotJetShapeTopics.exe"

./plotJetShapeTopics.exe -input "/data/kying/final80/pp80,/data/kying/final80/ppPhotonJet80" -kappas "80_pt80100_eta_pp80_pp80_photonjet_SN,N=4_400,16000,14000_0_80" -minpt 80 -maxpt 100 &
./plotJetShapeTopics.exe -input "/data/kying/final80/PbPbWide80_0_10,/data/kying/final80/PbPbWidePhotonJet80_0_10" -kappas "80_pt80100_eta_pbpb80_0_10_wide_pbpb80_0_10_wide_photonjet_SN,N=4_400,16000,14000_0_80" -minpt 80 -maxpt 100 &
./plotJetShapeTopics.exe -input "/data/kying/final80/pp80,/data/kying/final80/ppPhotonJet80" -kappas "80_pt100120_eta_pp80_pp80_photonjet_SN,N=4_400,16000,14000_0_80" -minpt 100 -maxpt 120 &
./plotJetShapeTopics.exe -input "/data/kying/final80/PbPbWide80_0_10,/data/kying/final80/PbPbWidePhotonJet80_0_10" -kappas "80_pt100120_eta_pbpb80_0_10_wide_pbpb80_0_10_wide_photonjet_SN,N=4_400,16000,14000_0_80" -minpt 100 -maxpt 120 &