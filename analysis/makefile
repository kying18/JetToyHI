default: RunAuAu

Messenger.o: Messenger.cpp Messenger.h
	g++ `root-config --cflags` Messenger.cpp -o Messenger.o -c

Execute: ExampleYi.cpp Messenger.o
	g++ `root-config --cflags --libs` ExampleYi.cpp -o Execute Messenger.o

TestRun: Execute
	./Execute
	cp Plots.pdf ~/public/html/

RunAuAu: Execute
	./Execute --input `ls Samples/pp200GeV_ypjet_10pthatInf_*.root | head -n20 | tr '\n' ','` \
		--output PlotsPP --jewel false
	./Execute --input `ls Samples/AuAu200GeV_ypjet_cent05_wRecoils_10pthatInf_default_*.root | head -n20 | tr '\n' ','` \
		--output PlotsAA --jewel true
	./Execute --input `ls Samples/AuAu200GeV_ypjet_cent05_wRecoils_10pthatInf_default_*.root | head -n20 | tr '\n' ','` --output PlotsAANoJewel --jewel false
	./Execute --input `ls Samples/AuAu200GeV_ypjet_cent05_woRecoils_10pthatInf_default_*.root | head -n20 | tr '\n' ','` \
		--output PlotsAANoRecoil --jewel false
	root -l -b -q QuickPlot.C
	cp Plot*pdf ~/public/html

RunAuAuR06: Execute
	./Execute --input `ls Samples/pp200GeV_ypjet_10pthatInf_*.root | head -n20 | tr '\n' ','` \
		--output PlotsPP06 --jewel false --R 06
	./Execute --input `ls Samples/AuAu200GeV_ypjet_cent05_wRecoils_10pthatInf_default_*.root | head -n20 | tr '\n' ','` \
		--output PlotsAA06 --jewel true --R 06
	./Execute --input `ls Samples/AuAu200GeV_ypjet_cent05_wRecoils_10pthatInf_default_*.root | head -n20 | tr '\n' ','` --output PlotsAA06NoJewel --jewel false --R 06
	./Execute --input `ls Samples/AuAu200GeV_ypjet_cent05_woRecoils_10pthatInf_default_*.root | head -n20 | tr '\n' ','` \
		--output PlotsAA06NoRecoil --jewel false --R 06
	root -l -b -q QuickPlot06.C
	cp Plot*pdf ~/public/html
