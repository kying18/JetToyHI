#!/bin/bash
g++ getJetTopicHists.cpp $(root-config --cflags --libs) -O2 -o "getJetTopicHists.exe"
# declare -a softdrops=("SD1" "SD2" "SD3" "SD4" "SD5" "SD6")
declare -a pts=("0" "100" "200")

count=0

for i in ${pts[@]}
do
    (
        ./getJetTopicHists.exe -input "/data/kying/jetTopicsData_cone/ppZJet150,/data/kying/jetTopicsData_cone/PbPbWideZJet150_0_10,/data/kying/jetTopicsData_cone/ppPhotonJet150,/data/kying/jetTopicsData_cone/PbPbWidePhotonJet150_0_10,/data/kying/jetTopicsData_cone/pp150,/data/kying/jetTopicsData_cone/PbPb150_0_10,/data/kying/jetTopicsData_cone/PbPbWide150_0_10" -minpt $i -output "./matrix_elem/" 
    ) &
    # for sd in ${softdrops[@]}
    # do
    #     (
    #         ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp150_7000thermal,/data/kying/jetTopicsData/PbPbWide150_0_10_7000thermal,/data/kying/jetTopicsData/PbPb150_0_10_7000thermal" -minpt $i -sd $sd -output ./150_1_7000thermal/150_1_dijet_thstack
    #     ) &
    # done
    # wait  # lol hopefully this is parallelized...?

	let count+=1
    [[ $((count%5)) -eq 0 ]] && wait

done

# ./getJetTopicHists.exe -input "/data/kying/jetTopicsData_cone/ppZJet150,/data/kying/jetTopicsData_cone/PbPbWideZJet150_0_10,/data/kying/jetTopicsData_cone/ppPhotonJet150,/data/kying/jetTopicsData_cone/PbPbWidePhotonJet150_0_10,/data/kying/jetTopicsData_cone/pp150,/data/kying/jetTopicsData_cone/PbPb150_0_10,/data/kying/jetTopicsData_cone/PbPbWide150_0_10" -minpt 0 -output "./matrix_elem/" 
# ./getJetTopicHists.exe -input "/data/kying/jetTopicsData_cone/ppZJet150" -minpt 0 -output "./matrix_elem/" 