#!/bin/bash


# jetTopicsData_cone is when we remove all of the jets where the photon or zjet are "out of bounds" for the detector
count=0

for i in {100..100}
do
	echo $i
	(
		./runLundPlane -hard "/data/yjlee/pyquen/ppPhotonJet150/pp_${i}.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData_cone/ppPhotonJet150/pp_${i}.root" -type photonjet
		./runLundPlane -hard "/data/yjlee/pyquen/PbPbWidePhotonJet150_0_10/PbPbWide_0_10_${i}.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData_cone/PbPbWidePhotonJet150_0_10/PbPbWide_0_10_${i}.root" -type photonjet
		./runLundPlane -hard "/data/yjlee/pyquen/ppZJet150/pp_${i}.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData_cone/ppZJet150/pp_${i}.root" -type zjet
		./runLundPlane -hard "/data/yjlee/pyquen/PbPbWideZJet150_0_10/PbPbWide_0_10_${i}.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData_cone/PbPbWideZJet150_0_10/PbPbWide_0_10_${i}.root" -type zjet
		./runLundPlane -hard "/data/yjlee/pyquen/pp150/pp_${i}.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData_cone/pp150/pp_${i}.root" -type dijet
		./runLundPlane -hard "/data/yjlee/pyquen/PbPbWide150_0_10/PbPbWide_0_10_${i}.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData_cone/PbPbWide150_0_10/PbPbWide_0_10_${i}.root" -type dijet
		./runLundPlane -hard "/data/yjlee/pyquen/PbPb150_0_10/PbPb_0_10_${i}.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData_cone/PbPb150_0_10/PbPb150_0_10_${i}.root" -type dijet

		# ./runLundPlane -hard "/data/yjlee/pyquen/ppPhotonJet150/pp_${i}.pu14" -hardtype PU14 -pileup /data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz -pileuptype PU14 -nev 2000 -output "/data/kying/jetTopicsData/ppPhotonJet150_7000thermal/pp_${i}.root" -type photonjet
		# ./runLundPlane -hard "/data/yjlee/pyquen/PbPbWidePhotonJet150_0_10/PbPbWide_0_10_${i}.pu14" -hardtype PU14 -pileup /data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz -pileuptype PU14 -nev 2000 -output "/data/kying/jetTopicsData/PbPbWidePhotonJet150_0_10_7000thermal/PbPbWide_0_10_${i}.root" -type photonjet
		# ./runLundPlane -hard "/data/yjlee/pyquen/ppZJet150/pp_${i}.pu14" -hardtype PU14 -pileup /data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz -pileuptype PU14 -nev 2000 -output "/data/kying/jetTopicsData/ppZJet150_7000thermal/pp_${i}.root" -type zjet
		# ./runLundPlane -hard "/data/yjlee/pyquen/PbPbWideZJet150_0_10/PbPbWide_0_10_${i}.pu14" -hardtype PU14 -pileup /data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz -pileuptype PU14 -nev 2000 -output "/data/kying/jetTopicsData/PbPbWideZJet150_0_10_7000thermal/PbPbWide_0_10_${i}.root" -type zjet
		# ./runLundPlane -hard "/data/yjlee/pyquen/pp150/pp_${i}.pu14" -hardtype PU14 -pileup /data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz -pileuptype PU14 -nev 2000 -output "/data/kying/jetTopicsData/pp150_7000thermal/pp_${i}.root" -type dijet
		# ./runLundPlane -hard "/data/yjlee/pyquen/PbPbWide150_0_10/PbPbWide_0_10_${i}.pu14" -hardtype PU14 -pileup /data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz -pileuptype PU14 -nev 2000 -output "/data/kying/jetTopicsData/PbPbWide150_0_10_7000thermal/PbPbWide_0_10_${i}.root" -type dijet
		# ./runLundPlane -hard "/data/yjlee/pyquen/PbPb150_0_10/PbPb_0_10_${i}.pu14" -hardtype PU14 -pileup /data/emmi/thermal/ThermalEventsMult7000PtAv1.20_14.pu14.gz -pileuptype PU14 -nev 2000 -output "/data/kying/jetTopicsData/PbPb150_0_10_7000thermal/PbPb150_0_10_${i}.root" -type dijet

	) &

	let count+=1
    [[ $((count%5)) -eq 0 ]] && wait

done


# ./runLundPlane -hard "/data/yjlee/pyquen/ppPhotonJet150/pp_2.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData/ppPhotonJet24/pp_2.root" -type photonjet
# ./runLundPlane -hard "/data/yjlee/pyquen/ppPhotonJet150/pp_4.pu14" -hardtype PU14 -nev 2000 -output "/data/kying/jetTopicsData/ppPhotonJet24/pp_4.root" -type photonjet