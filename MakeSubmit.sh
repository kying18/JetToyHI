#!/bin/bash

WorkDir=/afs/cern.ch/user/c/chenyi/work/Temp/21413_JetToyHI/JetToyHI
OutputDir=$WorkDir/SignalOnly
SampleBase=$WorkDir/Samples/jewel_20190515_Raghav
SubmitFile=Submit.condor

rm $SubmitFile
echo WorkDir    = $WorkDir               >> $SubmitFile
echo OutputDir  = $OutputDir             >> $SubmitFile
echo SampleBase = $SampleBase            >> $SubmitFile
echo                                     >> $SubmitFile
echo Universe   = vanilla                >> $SubmitFile
echo Executable = $\(WorkDir\)/RunJob.sh >> $SubmitFile
echo should_transfer_files = NO          >> $SubmitFile
echo +JobFlavour = \"workday\"           >> $SubmitFile
echo                                     >> $SubmitFile

for file in `ls $SampleBase |  grep wRecoil`
do
   echo $file

   echo Arguments = $\(WorkDir\) $\(SampleBase\)/$file $\(OutputDir\)/${file/.pu14/.root}  >> $SubmitFile
   echo Output    = Log/out.$\(Process\)                                                   >> $SubmitFile
   echo Error     = Log/err.$\(Process\)                                                   >> $SubmitFile
   echo Log       = Log/log.$\(Process\)                                                   >> $SubmitFile
   echo Queue                                                                              >> $SubmitFile
done




