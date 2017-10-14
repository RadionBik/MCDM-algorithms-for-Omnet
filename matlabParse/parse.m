function [aver] = parse(nameOfcsv)
%%
%nameOfcsv='/home/radion/omnetpp-4.6/inetMCDM/examples/wireless/handover/results/GRA-stream-low-1.csv';

numberOfCrit=36;
fileID = fopen(nameOfcsv);
names = textscan(fileID,repmat('%s ',1,numberOfCrit),1,'Delimiter',','); % 34 is a number of criteria in .csv
fclose(fileID);

%%

for i=1:size(names,2)
    names{i} = regexprep(names{i},'.*/','');        % cleanup names of vars
    names{i} = regexprep(names{i},':.*','');
    nam{i} = cell2mat(names{i}) ;                   % convert to cell string
end
critIndex = [ find(strcmp(nam,'apID')) find(strcmp(nam,'beaconDelay')) find(strcmp(nam,'beaconJitter')) ...
              find(strcmp(nam,'availBitrate')) find(strcmp(nam,'throughput AC0')) find(strcmp(nam,'cost')) ...
              find(strcmp(nam,'numberOfHO')) find(strcmp(nam,'packetLoss')) ...
              find(strcmp(nam,'RSS'))] ;

%%
%input = csvread(nameOfcsv,1,0); %reads only numerical values
input=table2array(readtable(nameOfcsv));

%%
simulTime = input(:,1);

% collect parameters into corresponding structures: .value, .time, .number (of values)
% while reducing dim of input values, by ommiting 0-values
actual.currentAP = reduceDim(input(:,critIndex(1)),simulTime);

actual.macDelay = reduceDim(input(:,critIndex(2)),simulTime); 

actual.macJitter = reduceDim(input(:,critIndex(3)),simulTime);

actual.availBitrate = reduceDim(input(:,critIndex(4)),simulTime);

actual.throughput = reduceDim(input(:,critIndex(5)),simulTime);

actual.cost = reduceDim(input(:,critIndex(6)),simulTime);

actual.numberOfHO = reduceDim(input(:,critIndex(7)),simulTime);

actual.packetLoss = reduceDim(input(:,critIndex(8)),simulTime);

actual.RSSI = reduceDim(input(:,critIndex(9)),simulTime);
%plot(currentAP.time,currentAP.value); grid on; xlim([0,max(simulTime)])
%% average critera
aver.delay = mean(actual.macDelay.value);

aver.jitter = mean(actual.macJitter.value);

aver.cost = mean(actual.cost.value);

aver.availBitrate = mean(actual.availBitrate.value);

aver.throughput = mean(actual.throughput.value);

%aver.loss = (0.05*rand+0.1)*10^-2;%(0.05*rand+0.95)*10^-6; %generated randomly. but have small range 0.95-1

aver.loss = mean(actual.packetLoss.value);
aver.numberOfHO = max(actual.numberOfHO.value);

actual.RSSI.value(actual.RSSI.value==-Inf)=-100;
aver.RSSI = mean(actual.RSSI.value);

end