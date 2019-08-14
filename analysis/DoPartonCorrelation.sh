g++ PlotPartonCorrelation.cpp `root-config --cflags --glibs` -o a.out

# ./a.out --JetPTCut 350 --Algo KT --DoZG 0 \
#    --OutputBase CorrelationUEOn --InputFile ../samples/Pythia.root --Title "Pythia8 UEOn"
# ./a.out --JetPTCut 350 --Algo CA --DoZG 0 \
#    --OutputBase CorrelationUEOn --InputFile ../samples/Pythia.root --Title "Pythia8 UEOn"
# ./a.out --JetPTCut 350 --Algo AK --DoZG 0 \
#    --OutputBase CorrelationUEOn --InputFile ../samples/Pythia.root --Title "Pythia8 UEOn"

# ./a.out --JetPTCut 350 --Algo KT --DoZG 0 \
#    --OutputBase CorrelationUEOff --InputFile ../samples/PythiaUEOff.root --Title "Pythia8 UEOff"
# ./a.out --JetPTCut 350 --Algo CA --DoZG 0 \
#    --OutputBase CorrelationUEOff --InputFile ../samples/PythiaUEOff.root --Title "Pythia8 UEOff"
# ./a.out --JetPTCut 350 --Algo AK --DoZG 0 \
#    --OutputBase CorrelationUEOff --InputFile ../samples/PythiaUEOff.root --Title "Pythia8 UEOff"

# ./a.out --JetPTCut 350 --Algo KT --DoZG 1 \
#    --OutputBase CorrelationUEOnSD1 --InputFile ../samples/Pythia.root --Title "Pythia8 UEOn (0.1, 0.0)"
# ./a.out --JetPTCut 350 --Algo KT --DoZG 2 \
#    --OutputBase CorrelationUEOnSD2 --InputFile ../samples/Pythia.root --Title "Pythia8 UEOn (0.5, 1.5)"
# ./a.out --JetPTCut 350 --Algo KT --DoZG 3 \
#    --OutputBase CorrelationUEOnSD3 --InputFile ../samples/Pythia.root --Title "Pythia8 UEOn (0.25, 0.0)"

./a.out --JetPTCut 350 --Algo CA --DoZG 0 \
   --OutputBase CorrelationThermal --InputFile ../samples/PythiaThermal.root --Title "Pythia8+thermal"
./a.out --JetPTCut 350 --Algo KT --DoZG 1 \
   --OutputBase CorrelationThermalSD1 --InputFile ../samples/PythiaThermal.root --Title "Pythia8+thermal (0.1, 0.0)"
./a.out --JetPTCut 350 --Algo KT --DoZG 2 \
   --OutputBase CorrelationThermalSD2 --InputFile ../samples/PythiaThermal.root --Title "Pythia8+thermal (0.5, 1.5)"
./a.out --JetPTCut 350 --Algo KT --DoZG 3 \
   --OutputBase CorrelationThermalSD3 --InputFile ../samples/PythiaThermal.root --Title "Pythia8+thermal (0.25, 0.0)"


