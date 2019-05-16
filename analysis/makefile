default: TestRun

Messenger.o: Messenger.cpp Messenger.h
	g++ `root-config --cflags` Messenger.cpp -o Messenger.o -c

Execute: Example.cpp Messenger.o
	g++ `root-config --cflags --libs` Example.cpp -o Execute Messenger.o \
		-I/afs/cern.ch/user/c/chenyi/RootMacros

TestRun: Execute
	./Execute
	cp Plots.pdf ~/public/html/
