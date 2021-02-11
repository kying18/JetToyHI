#!/bin/bash
g++ getJetTopicHists.cpp $(root-config --cflags --libs) -O2 -o "getJetTopicHists.exe"

for i in {0, 50, 100}
do
    ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp150,/data/kying/jetTopicsData/PbPbWide150_0_10,/data/kying/jetTopicsData/PbPb150_0_10,/data/kying/jetTopicsData/pp150_7000thermal,/data/kying/jetTopicsData/PbPbWide150_0_10_7000thermal,/data/kying/jetTopicsData/PbPb150_0_10_7000thermal" -minpt "${i}" -output ./150_1_7000thermal/150_1_dijet_thstack
    for sd in {SD1, SD2, SD3, SD4, SD5, SD6}
    do
        (./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp150,/data/kying/jetTopicsData/PbPbWide150_0_10,/data/kying/jetTopicsData/PbPb150_0_10,/data/kying/jetTopicsData/pp150_7000thermal,/data/kying/jetTopicsData/PbPbWide150_0_10_7000thermal,/data/kying/jetTopicsData/PbPb150_0_10_7000thermal" -minpt "${i}" -sd "${sd}" -output ./150_1_7000thermal/150_1_dijet_thstack) &
    done
    wait  # lol hopefully this is parallelized...?
done