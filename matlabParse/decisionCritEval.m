function evalDec = decisionCritEval(A)

%RSS AB C
traffic = fieldnames(A);  %or gra or saw, doesn't matter, the fields are the same
load = fieldnames(A.conv);
for j=1:numel(load)
    for i=1:numel(traffic)
        if(i==1)
            w=[0.05; 0.18; 0.77];
        elseif(i==2)
            w=[0.05; 0.77; 0.18];
        end
        decisionCrit=[ -1.*A.(traffic{i}).(load{j})(:,8) A.(traffic{i}).(load{j})(:,1) ...
            A.(traffic{i}).(load{j})(:,6) ];
        
        % convert to the same type (upward)
        evalDec.(traffic{i}).(load{j}) = norm_p(invert(decisionCrit,[1 0 1]),2)*w;
    end
end
end