#!/bin/bash

cd ~/work/CMSSW/CMSSW_10_3_3_patch1/src
eval `scramv1 runtime -sh`
cd -

WorkDir=$1
Input=$2
Output=$3

echo $WorkDir/runJetTools -hard $Input -nev 2000
$WorkDir/runJetTools -hard $Input -nev 2000

mv JetToyHIResult.root $Output

rm chenyi.cc


