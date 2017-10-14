#!/bin/bash

cd /home/radion/omnetpp-4.6/inet/examples/wireless/handover
#		0	1	2	  3		4
algos=(RSS SAW GRA TOPSIS VIKOR )
numberAlgos=${#algos[*]}

trafficType=(conv stream)
numberTraff=${#trafficType[*]}

load=(low high)
numberLoad=${#load[*]}

#define bitrate in Mbps and cost for each AP as an array for each sub-scenario
AP1_cost=(4 4 95 27 24)
AP1_bit=(6 18 6 1 2)
AP2_cost=(95 95 4 4 70)
AP2_bit=(12 5.5 12 2 6)
AP3_cost=(58 2 70 24 58)
AP3_bit=(9 1 5.5 6 1)
AP4_cost=(27 70 58 70 27)
AP4_bit=(11 11 11 12 12)
AP5_cost=(70 58 24 58 4)
AP5_bit=(18 9 1 11 18)
AP6_cost=(24 27 27 95 95)
AP6_bit=(1 12 9 5.5 5.5)
# seeds for each subscenario
seedsSim=(8 12)
# 3 4 5)
numberSim=${#seedsSim[*]}

function changeIniFile
{
#change seed number
sed -i -e "s/seed-set.*/seed-set=${seedsSim[$1]}/g" MCDM_common.ini
#change cost of APs
sed -i "s/.*AP1.*cost.*/**.AP1.wlan[*].mgmt.cost=${AP1_cost[$1]}/" MCDM_common.ini
sed -i "s/.*AP2.*cost.*/**.AP2.wlan[*].mgmt.cost=${AP2_cost[$1]}/" MCDM_common.ini
sed -i "s/.*AP3.*cost.*/**.AP3.wlan[*].mgmt.cost=${AP3_cost[$1]}/" MCDM_common.ini
sed -i "s/.*AP4.*cost.*/**.AP4.wlan[*].mgmt.cost=${AP4_cost[$1]}/" MCDM_common.ini
sed -i "s/.*AP5.*cost.*/**.AP5.wlan[*].mgmt.cost=${AP5_cost[$1]}/" MCDM_common.ini
sed -i "s/.*AP6.*cost.*/**.AP6.wlan[*].mgmt.cost=${AP6_cost[$1]}/" MCDM_common.ini
#change bitrate of APs
sed -i "s/.*AP1.*bitrate.*/**.AP1.wlan[*].mgmt.bitrate=${AP1_bit[$1]} Mbps/" MCDM_common.ini
sed -i "s/.*AP2.*bitrate.*/**.AP2.wlan[*].mgmt.bitrate=${AP2_bit[$1]} Mbps/" MCDM_common.ini
sed -i "s/.*AP3.*bitrate.*/**.AP3.wlan[*].mgmt.bitrate=${AP3_bit[$1]} Mbps/" MCDM_common.ini
sed -i "s/.*AP4.*bitrate.*/**.AP4.wlan[*].mgmt.bitrate=${AP4_bit[$1]} Mbps/" MCDM_common.ini
sed -i "s/.*AP5.*bitrate.*/**.AP5.wlan[*].mgmt.bitrate=${AP5_bit[$1]} Mbps/" MCDM_common.ini
sed -i "s/.*AP6.*bitrate.*/**.AP6.wlan[*].mgmt.bitrate=${AP6_bit[$1]} Mbps/" MCDM_common.ini
}

for (( currentLoad=0; currentLoad<$numberLoad; currentLoad++ ))
do

for (( currentTraff=0; currentTraff<$numberTraff; currentTraff++ ))
do

for (( currentAlg=0; currentAlg<$numberAlgos; currentAlg++ ))
do

for (( simul=0; simul<$numberSim; simul++ ))
do
#--------------------------------------------------
#adjust conf in accordance with simulation number
changeIniFile $simul
#--------------------------------------------
#show current simulation
echo '*******' ${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}, $((simul+1)) scenario '*******'

#rm results/${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}-$((simul+1)).*

simulOk=false
nS=0	#check number of  repetitions
until $simulOk
do
	#run simulation and save logs to a file
	opp_run -r 0 -u Cmdenv -c ${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]} -n ../..:../../../src:../../../tutorials -l ../../../src/INET MCDM_Handover_${load[$currentLoad]}.ini	> results/${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}-$((simul+1)).txt
 
	#if simulation failed, replace seed number with a random one (10-100)
	if grep -q ERROR results/${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}-$((simul+1)).txt
	then
		echo Repetition due to failure ...		
		((nS++))
		seedNumber=$RANDOM
		let "seedNumber%=90"
		let "seedNumber=seedNumber+10"
		sed -i -e "s/seed-set.*/seed-set=$seedNumber/g" MCDM_common.ini
	else
		simulOk=true
	fi 
done

#show report on simulation time 
#grep 'T=' results/${algos[$currentAlg]}-${trafficType[$currentTraf]}-${load[$currentLoad]}-$((simul+1)).txt

if [ $nS -ne 0 ]
then
	echo The simulation has been repeated $nS Times
else
	echo The simulation is completed OK
fi

#rename .vec-file (from .*-0.vec format to -simul.vec) to keep results
cd results

mv ${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}-0.vec ${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}-$((simul+1)).vec
rm ${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}-0.*

#convert .vec files to .csv format
echo Renaming to ..$((simul+1)).vec ... Removing garbage ... converting to .csv ...
scavetool vector -p "module(**.wirelessClient.*)" -O ${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}-$((simul+1)) -F csv ${algos[$currentAlg]}-${trafficType[$currentTraff]}-${load[$currentLoad]}-$((simul+1)).vec 

cd ..
done #close simul loop
echo The subscenario loop has been completed. 

done #close algorithm loop
echo The algorithm loop has been completed. 

done #close traffic loop
echo The traffic loop has been completed. 

done #close load loop
echo The load loop has been completed. 








