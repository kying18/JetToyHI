#!/bin/bash

# get the input file if needed
# [[ ! -e ntD_pp_HeavyFlavor_goldenjson.root ]] && { curl -O http://web.mit.edu/mithig/samples/ntD_pp_HeavyFlavor_goldenjson.root ; }

# input /data/emmi/pythia/pythia/pythiaHepMC_dijet_pthat50_s5.02TeV_UEon_Tun4C/hepmcout700.gz
# background /data/emmi/thermal/ThermalEventsMult100PtAv0.85_14.pu14.gz
PYTHIA_INPUT_DATA="/data/kying/lundPlaneResults/pythia_10000.root"
PYTHIA_THERMAL_INPUT_DATA="/data/kying/lundPlaneResults/pythia_thermal_10000.root"
PYTHIA_INPUT_CS_DATA="/data/kying/lundPlaneResults/pythia_cs_10000.root"
PYTHIA_THERMAL_CS_INPUT_DATA="/data/kying/lundPlaneResults/pythia_thermal_cs_10000.root"

# PYTHIA_OUT="outputfiles/pythia/"
# PYTHIA_THERMAL_OUT="outputfiles/pythia_thermal/"
# PYTHIA_CS_OUT="outputfiles/pythia_cs/"
# PYTHIA_THERMAL_CS_OUT="outputfiles/pythia_thermal_cs/"

TEMP_OUT="temp/"

# FOLDERS=("outputfiles" "temp" "outputfiles/pythia" "outputfiles/pythia_thermal" "outputfiles/pythia_cs" "outputfiles/pythia_thermal_cs" "plots" )
FOLDERS=("temp/" "temp/test/")
for i in ${FOLDERS[@]} ; do mkdir -p $i ; done ;

# OUTFOLDERS = ($PYTHIA_OUT $PYTHIA_THERMAL_OUT $PYTHIA_CS_OUT $PYTHIA_THERMAL_CS_OUT)
# OUTFOLDERS=($TEMP_OUT)
# for outf in ${OUTFOLDERS[@]} 
# do
# BRANCHES=("SignalJetPt" "SignalJetSD1DR12" "SignalJetSD1Pt" "SignalJetSD1ZG" "SignalJetSD2DR12" "SignalJetSD2Pt" "SignalJetSD2ZG")
BRANCHES=("SignalJetPt")
for branch in ${BRANCHES[@]}
do 
    # echo $outf + " " + $branch
    # TYPE="test"
    root -l -b -q "savehist.C+("\"$PYTHIA_INPUT_DATA\",\"$TEMP_OUT\",\"$branch\",\"$TYPE\"")"
done
# done
# root -l -b -q "fitD.C+("\"$OUTPUT\"")"


rm *.d
rm *.pcm
rm *.so
