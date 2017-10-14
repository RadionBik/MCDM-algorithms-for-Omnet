function outputToFile (A,algorithmNames,criteriaNames,filename)
traffic = fieldnames(A);  %or gra or saw, doesn't matter, the fields are the same
load = fieldnames(A.conv);

diary on;
 if (exist(filename,'file'))
  delete(filename);
 end
diary(filename)

for j=1:numel(load)
    for i=1:numel(traffic) 
        Scenario=['Scenario: ' convertToBetterName(traffic{i}) ' traffic with '...
            convertToBetterName(load{j})]
        Data=array2table(A.(traffic{i}).(load{j}),'RowNames',algorithmNames,...
            'VariableNames',criteriaNames)
fprintf('------------------------------------------------------------------------\n');
    end
end

diary off;
unix(['./cleanUpOutput.sh ' filename]);

end