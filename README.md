This repository holds an implementation of Multi-Criteria Handover Algorithms for INET 3.0.0 and OMNeT++ 4.6. Note: it hasn't been tested with other versions of OMNeT++!

The files are included 'as is', in hope to be useful for anyone.

For the details you are encouraged to read the paper:
Radion Bikmukhamedov, Adel Nadeev, Yevgeniy Yeryomin, Jochen Seitz, 'Simulating multi-criteria handover algorithms in OMNeT++' in 2017 International Siberian Conference on Control and Communications (SIBCON)

What was modified in INET 3.0.0:
1. The decision parameters (e.g. RSS,availableBitrate, Jitter, Delay, Cost) are collected across the INET library and further transmitted via the Beacon frame object to the Ieee80211AgentSTA.cc file. 

2. The parameters within the Ieee80211AgentSTA.cc file are assembled into the allPathsCriteriaValues string variable and transmitted to the MCDM-library (see 'inet/src/inet/linklayer/ieee80211/mgmt/mcda') within the Ieee80211AgentSTA::chooseBSS_new() function.

An example of calling the library:

	bestIndex = McdaAlg::selectAPWithMCDA(allPathsCriteriaValues,decisionAlgorithm,trafficType,criteriaNumber);

where

'decisionAlgorithm' is a variable sent from the MCDM_Handover_(low|high).ini files,

'trafficType' is sent from MCDM-common.ini and implements either Conversational or Streaming traffic.

'criteriaNumber' defines how many criteria to be used during decision-making and directly affects which configuration files to use during the decision-making process. 
Configuration files for 3 and 5 criteria have been created (see the *.dat files in 'inet/src/inet/linklayer/ieee80211/mgmt/mcda/'). 

To clarify, there are 3 stages in a decision-making algorithm (see implemented functions in 'MCDM_Algorithms.h'):

1.Normalization. Enhanced Max-Min method is implemented, where the max and min values are configured in enhNorm*.dat. 

2.Weighting. The Weighted Least Squares method is applied. The preferences are expressed via a predefined matrix of pair-wise comparisons (A*conv.dat and A*stream.dat must be adjusted accordingly). 

3. Ranking stage. There are 4 possible options: Simple Additive Weighting (SAW), TOPSIS, GRA, VIKOR.

Note, that the paths to the .dat files are written in 'MCDM_algorithms.cc' explicitly, therefore they should be corrected in accordance with the actual location of OMNeT++ in your system.

Example configurations are provided as well. See the .ned and .ini files in 'inet/examples/' 

In order to automatize the simulations, a bash script 'getResPositionFile.sh' has been developed that changes the configuration of an .ini file in accordance with the current type of scenario. Moreover, some workaround have been implemented in-there due to the OMNeT++ bug, where an STA couldn't resolve MAC address of an AP.
Moreover, several Matlab files (see the 'matlabParse' folder) have been developed to help process the resulting .csv files, which are retrieved from OMNeT++ .vec files by means of scavetool. 



