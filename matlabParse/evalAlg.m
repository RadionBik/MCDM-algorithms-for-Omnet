function score = evalAlg(A, focus)

traffic = fieldnames(A);  %or gra or saw, doesn't matter, the fields are the same
load = fieldnames(A.conv);

for j=1:numel(load)
    for i=1:numel(traffic) 
        
%criteria of matrix: availBitr throughput  delay   jitter loss  cost

% construction of the QoS set  BW D J L
%QoS= [ A.(traffic{i}).(load{j})(:,2) A.(traffic{i}).(load{j})(:,3)...
%    A.(traffic{i}).(load{j})(:,4) A.(traffic{i}).(load{j})(:,5)];
QoS= [ A.(traffic{i}).(load{j})(:,3)...
    A.(traffic{i}).(load{j})(:,4) A.(traffic{i}).(load{j})(:,5)];   

% availBitr  cost         
Network = [ A.(traffic{i}).(load{j})(:,1) A.(traffic{i}).(load{j})(:,6) ];
     
% convert to the same type (upward)   
%modQoS = invert(QoS, [0 1 1 1]);
modQoS = invert(QoS, [1 1 1]);
modNetwork= invert(Network, [0 1]);

% normalization of input
normModQoS=norm_p(modQoS,2);
normModNetwork=norm_p(modNetwork,2);

% the weights are taken from chapter 3.
if (strcmp(traffic{i},'conv')==1)
    %w = [0.06; 0.58; 0.26; 0.1];
    w = [0.65; 0.27; 0.08];
elseif (strcmp(traffic{i},'stream')==1)
    w = [ 0.1; 0.64; 0.26];
end    
   
vectorQoS=normModQoS*w;

% ab c qos
scoreQos.(traffic{i}).(load{j})=vectorQoS;
if (strcmp(focus,'qos')==1)
    w_net = [ 0.54; 0.09; 0.37] ;
elseif (strcmp(focus,'cost')==1)
    w_net = [ 0.09; 0.54; 0.37] ;
end    


% availBitr cost  
E = [ normModNetwork vectorQoS] * w_net;

score.(traffic{i}).(load{j}) = E ./ (sum(w_net));


    end
end

%criteria of matrix: availBitr throughput  delay   jitter loss  cos
algorithmNames={'RSS','SAW','GRA','TOPSIS','VIKOR'};
%criteriaNames={'Available Bitrate, bps','T, bps','D, s','J, s','PL, fraction','Cost','HO number'};
criteriaNames={'Evaluation_score_of_the_QoS_set'};
outputToFile (scoreQos,algorithmNames,criteriaNames,'outputQoS.txt');

%criteria of matrix: availBitr throughput  delay   jitter loss  cos
algorithmNames={'RSS','SAW','GRA','TOPSIS','VIKOR'};
%criteriaNames={'Available Bitrate, bps','T, bps','D, s','J, s','PL, fraction','Cost','HO number'};
criteriaNames={['Evaluation_score_with_' focus '_focus']};
outputToFile (score,algorithmNames,criteriaNames,['output_' focus 'Aggreg.txt']);
end
