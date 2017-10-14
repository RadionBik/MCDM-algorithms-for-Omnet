function struct = reduceDim (vecData,simulTime)

% for i=1:size(simulTime,1)
%     if (vecData(i)==0)
%         simulTime(i)=0;
%     end
% end

simulTime(isnan(vecData)==1)=[]; %remove NaN values in the array
vecData(isnan(vecData)==1)=[]; 

struct.value = vecData;
struct.time = simulTime;
struct.number = size(vecData,1);
end