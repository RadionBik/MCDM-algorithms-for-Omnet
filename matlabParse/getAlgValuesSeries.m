function AlgMean = getAlgValuesSeries(pathToCsv,algName,numbOfSeries)
%%
%retrieve the data from each .csv file
for s=1:numbOfSeries
Alg.conv.low{s} = parse([pathToCsv algName '-conv-low-' num2str(s) '.csv']);
Alg.stream.low{s} = parse([pathToCsv algName '-stream-low-' num2str(s) '.csv']);
Alg.conv.high{s} = parse([pathToCsv algName '-conv-high-' num2str(s) '.csv']);
Alg.stream.high{s} = parse([pathToCsv algName '-stream-high-' num2str(s) '.csv']);
end

%%
%get averaged values among all series 
criteria = fieldnames(Alg.conv.low{1});

for i=1:numel(criteria)
    %initialize results
    AlgMean.mean.conv.low.(criteria{i}) = [];
    AlgMean.mean.stream.low.(criteria{i}) = [];
    AlgMean.mean.conv.high.(criteria{i}) = [];
    AlgMean.mean.stream.high.(criteria{i}) = [];   
    % collect mean values from each subscenario into vector for further
    % averaging
    for s=1:numbOfSeries
    AlgMean.mean.conv.low.(criteria{i}) = ([AlgMean.mean.conv.low.(criteria{i}) Alg.conv.low{s}.(criteria{i})]);
    AlgMean.mean.stream.low.(criteria{i}) = ([AlgMean.mean.stream.low.(criteria{i}) Alg.stream.low{s}.(criteria{i})]);
    AlgMean.mean.conv.high.(criteria{i}) = ([AlgMean.mean.conv.high.(criteria{i}) Alg.conv.high{s}.(criteria{i})]);
    AlgMean.mean.stream.high.(criteria{i}) = ([AlgMean.mean.stream.high.(criteria{i}) Alg.stream.high{s}.(criteria{i})]);
    end
    % perform averaging
    AlgMean.mean.conv.low.(criteria{i}) = mean(AlgMean.mean.conv.low.(criteria{i}));
    AlgMean.mean.stream.low.(criteria{i}) = mean(AlgMean.mean.stream.low.(criteria{i}));
    AlgMean.mean.conv.high.(criteria{i}) = mean(AlgMean.mean.conv.high.(criteria{i}));
    AlgMean.mean.stream.high.(criteria{i}) = mean(AlgMean.mean.stream.high.(criteria{i}));
end

end
