#!/bin/bash
g++ getJetTopicHists.cpp $(root-config --cflags --libs) -O2 -o "getJetTopicHists.exe"
g++ getQuarkGluonTruths.cpp $(root-config --cflags --libs) -O2 -o "getQuarkGluonTruths.exe"
# declare -a softdrops=("SD1" "SD2" "SD3" "SD4" "SD5" "SD6")
declare -a pts=("0" "100")
output_folder="./final80eta2/"

count=0

# for i in ${pts[@]}
for i in `seq 80 20 160`
do
    (
        # if (($i % 100 == 0))
        # then
        #     # echo $i
        #     ./getJetTopicHists.exe -input "/data/kying/rematchMatElemsJetTopics80/ppZJet80,/data/kying/rematchMatElemsJetTopics80/PbPbWideZJet80_0_10,/data/kying/rematchMatElemsJetTopics80/ppPhotonJet80,/data/kying/rematchMatElemsJetTopics80/PbPbWidePhotonJet80_0_10,/data/kying/rematchMatElemsJetTopics80/pp80,/data/kying/rematchMatElemsJetTopics80/PbPb80_0_10,/data/kying/rematchMatElemsJetTopics80/PbPbWide80_0_10" -minpt $i -output "./matrix_elem/" 
        # fi
        # echo $i $(($i + 20))
        # echo ${output_folder}
        # echo pt${i}$(($i + 20))histogram.csv
        > ${output_folder}pt${i}$(($i + 20))_qg_truths_histogram.csv
        > ${output_folder}pt${i}$(($i + 20))histogram.csv
        
        ./getQuarkGluonTruths.exe -input "/data/kying/final80/ppPhotonJet80,/data/kying/final80/pp80,/data/kying/final80/PbPbWidePhotonJet80_0_10,/data/kying/final80/PbPbWide80_0_10" -minpt $i -maxpt $(($i + 20)) -output ${output_folder}
        ./getJetTopicHists.exe -input "/data/kying/final80/ppPhotonJet80,/data/kying/final80/pp80,/data/kying/final80/PbPbWidePhotonJet80_0_10,/data/kying/final80/PbPbWide80_0_10" -minpt $i -maxpt $(($i + 20)) -output ${output_folder}
        # > matrix_elem_pt/pt${i}histogram.csv
        # > matrix_elem_pt/pt${i}mass.csv
        # ./getJetTopicHists.exe -input "/data/kying/rematchMatElemsJetTopics80/ppZJet80,/data/kying/rematchMatElemsJetTopics80/PbPbWideZJet80_0_10,/data/kying/rematchMatElemsJetTopics80/ppPhotonJet80,/data/kying/rematchMatElemsJetTopics80/PbPbWidePhotonJet80_0_10,/data/kying/rematchMatElemsJetTopics80/pp80,/data/kying/rematchMatElemsJetTopics80/PbPb80_0_10,/data/kying/rematchMatElemsJetTopics80/PbPbWide80_0_10" -minpt $i -output "./matrix_elem_pt/" 
    ) &
    # for sd in ${softdrops[@]}
    # do
    #     (
    #         ./getJetTopicHists.exe -input "/data/kying/jetTopicsData/pp80_7000thermal,/data/kying/jetTopicsData/PbPbWide80_0_10_7000thermal,/data/kying/jetTopicsData/PbPb80_0_10_7000thermal" -minpt $i -sd $sd -output ./80_1_7000thermal/80_1_dijet_thstack
    #     ) &
    # done
    # wait  # lol hopefully this is parallelized...?

	let count+=1
    [[ $((count%5)) -eq 0 ]] && wait

done

# ./getQuarkGluonTruths.exe -input "/data/kying/final80/ppPhotonJet80,/data/kying/final80/pp80,/data/kying/final80/PbPbWidePhotonJet80_0_10,/data/kying/final80/PbPbWide80_0_10" -minpt 0 -maxpt 500 -output ./final80/
# ./getJetTopicHists.exe -input "/data/kying/final80/ppPhotonJet80,/data/kying/final80/pp80,/data/kying/final80/PbPbWidePhotonJet80_0_10,/data/kying/final80/PbPbWide80_0_10" -minpt 0 -maxpt 500 -output ./final80/

# > ./final80/pt100120histogram.csv
# ./getQuarkGluonTruths.exe -input "/data/kying/final80/ppPhotonJet80,/data/kying/final80/pp80" -minpt 100 -maxpt 120 -output ./final80/
# ./getJetTopicHists.exe -input "/data/kying/final80/ppPhotonJet80,/data/kying/final80/pp80" -minpt 100 -maxpt 120 -output ./final80/
# ./getJetTopicHists.exe -input "/data/kying/photonFilterJetTopicsData80/ppPhotonJet80,/data/kying/photonFilterJetTopicsData80/pp80" -minpt 100 -maxpt 120 -output ./final80_prev/

# ./debugDijet.exe -input "/data/kying/photonFilterJetTopicsData80/ppPhotonJet80,/data/kying/photonFilterJetTopicsData80/pp80" -minpt 100 -maxpt 120
# ./getJetTopicHists.exe -input "/data/kying/jetTopicsData80/ppZJet80" -minpt 0 -output "test" 
# ./getJetTopicHists.exe -input "/data/kying/rematchMatElemsJetTopics80/ppZJet80,/data/kying/rematchMatElemsJetTopics80/PbPbWideZJet80_0_10,/data/kying/rematchMatElemsJetTopics80/ppPhotonJet80,/data/kying/rematchMatElemsJetTopics80/PbPbWidePhotonJet80_0_10,/data/kying/rematchMatElemsJetTopics80/pp80,/data/kying/rematchMatElemsJetTopics80/PbPb80_0_10,/data/kying/rematchMatElemsJetTopics80/PbPbWide80_0_10" -minpt 0 -output "./test/" 
# ./getJetTopicHists.exe -input "/data/kying/rematchMatElemsJetTopics80/ppZJet80" -minpt 0 -output "./test/" 
# ./getJetTopicHists.exe -input "/data/kying/rematchMatElemsJetTopics80/ppZJet80,/data/kying/rematchMatElemsJetTopics80/ppPhotonJet80,/data/kying/rematchMatElemsJetTopics80/pp80" -minpt 100 -maxpt 120 -output ./test/

# ./getJetTopicHists.exe -input "/data/kying/rematchMatElemsJetTopics80/ppPhotonJet80,/data/kying/rematchMatElemsJetTopics80/PbPbWidePhotonJet80_0_10" -minpt 100 -maxpt 120 -output ./pp80_nodr/zoomedmatpt_
# NOTE TO SELF: NEED TO RUN Z JET
# ./getJetTopicHists.exe -input "/data/kying/rematchMatElemsJetTopics80/ppPhotonJet80,/data/kying/rematchMatElemsJetTopics80/pp80,/data/kying/rematchMatElemsJetTopics80/PbPbWidePhotonJet80_0_10,/data/kying/rematchMatElemsJetTopics80/PbPbWide80_0_10" -minpt 100 -maxpt 120 -output ./final80/
# ./getJetTopicHists.exe -input "/data/kying/rematchMatElemsJetTopics80/ppZJet80" -minpt 100 -maxpt 120 -output ./final80/


# Filename: pp_2.root
# Entry count... 2843
# Event 2484 leading low multiplicity
# ^ - pT: 114.181825, element:2.000000, px: 58.891000, py: 97.823000
# Filename: pp_1.root
# Entry count... 2819
# Event 1848 leading low multiplicity
# ^ - pT: 132.117644, element:2.000000, px: -78.273000, py: 106.435000
# Filename: pp_3.root
# Entry count... 2842
# Event 1483 leading low multiplicity
# ^ - pT: 129.987702, element:2.000000, px: -76.886000, py: 104.811000
# Filename: pp_4.root
# Entry count... 2856
# Event 446 leading low multiplicity
# ^ - pT: 104.927679, element:1.000000, px: 86.764000, py: -59.007000
# Filename: pp_5.root
# Entry count... 2873
# Filename: pp_9.root
# Entry count... 2724