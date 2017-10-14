clc; clear all; close all;
%%
pathToCsv='/home/radion/omnetpp-4.6/inetMCDM/examples/wireless/handover/results/';
numbOfSeries = 5;
RSS = getAlgValuesSeries(pathToCsv,'RSS',numbOfSeries);
SAW = getAlgValuesSeries(pathToCsv,'SAW',numbOfSeries);
GRA = getAlgValuesSeries(pathToCsv,'GRA',numbOfSeries);
TOPSIS = getAlgValuesSeries(pathToCsv,'TOPSIS',numbOfSeries);
VIKOR = getAlgValuesSeries(pathToCsv,'VIKOR',numbOfSeries);

%%  conv, stream, --> low, high
EvalMtx = getEvalMtx (RSS,SAW,GRA,TOPSIS,VIKOR);
%%
%criteria of matrix: availBitr throughput  delay   jitter loss  cos
algorithmNames={'RSS','SAW','GRA','TOPSIS','VIKOR'};
%criteriaNames={'Available Bitrate, bps','T, bps','D, s','J, s','PL, fraction','Cost','HO number'};
criteriaNames={'AvailableBitrate_bps','T_bps','D_s','J_s','PL_fraction','Cost','HOnumber','RSS'};
%outputToFile (EvalMtx,algorithmNames,criteriaNames,'outputGeneral.txt');


%%

Score.qos = evalAlg(EvalMtx,'qos');
Score.cost = evalAlg(EvalMtx,'cost');

%%
close all;
plotCrit(EvalMtx);
%decisionEval = decisionCritEval(EvalMtx);
%plotEval(Score.cost,Score.qos,decisionEval,'qos')
%plotEval(Score.cost,Score.qos,decisionEval,'cost')

