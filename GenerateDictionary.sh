#!/bin/bash

rootcling -v4 -f RootDictionary.cxx  -rmf LibRootDictionary.rootmap -rml LibRootDictionary.so LinkDef.h
g++ -c -o LibRootDictionary.o RootDictionary.cxx `root-config --cflags --libs`

