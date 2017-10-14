function plotCrit(EvalMtx)

traffic = fieldnames(EvalMtx);  %or gra or saw, doesn't matter, the fields are the same
load = fieldnames(EvalMtx.conv);

maxi=[];
for j=1:numel(load)
    for i=1:numel(traffic) 
        %for k=1:8 
        %%aa=(j-1)+i
        maxi=[maxi; max(EvalMtx.(traffic{i}).(load{j})(:,:))];
       % end
    end
end
Maxi=max(maxi);

for j=1:numel(load)
    for i=1:numel(traffic) 

        %normalize matrices
        %A.(traffic{i}).(load{j})=norm_max(EvalMtx.(traffic{i}).(load{j}));
        A.(traffic{i}).(load{j})=bsxfun(@rdivide,EvalMtx.(traffic{i}).(load{j}),Maxi);
        %get max values
%         Maxi.(traffic{i}).(load{j})=max(EvalMtx.(traffic{i}).(load{j}));
%         Mini.(traffic{i}).(load{j})=min(EvalMtx.(traffic{i}).(load{j}));
        
        fig.(traffic{i}).(load{j})= figure('Position', [100, 100, 700, 400]);
        fig.(traffic{i}).(load{j}).PaperPositionMode = 'auto';
        bar( [A.(traffic{i}).(load{j})(:,1).'; A.(traffic{i}).(load{j})(:,3).';...
                A.(traffic{i}).(load{j})(:,4).'; A.(traffic{i}).(load{j})(:,5).'; A.(traffic{i}).(load{j})(:,6).'; ...
					 A.(traffic{i}).(load{j})(:,7).'; max(EvalMtx.(traffic{i}).(load{j})(:,8))./EvalMtx.(traffic{i}).(load{j})(:,8).']);
        set(gca,'XTickLabel',{['\begin{tabular}{c} Available \\ bitrate \\ /' ...
				num2str(Maxi(1)/1000000, '%5.2f') ' Mbps\end{tabular}' ],... %['\begin{tabular}{c} Troughput \\ /' num2str(Maxi.(traffic{i}).(load{j})(2)/1000000, '%5.2f') ' Mbps \end{tabular}' ],...
				['\begin{tabular}{c} Delay \\ /' num2str(Maxi(3)*1000, '%5.1f' ) ' ms \end{tabular}'],...
        ['\begin{tabular}{c} Jitter \\ /' num2str(Maxi(4)*1000, '%5.1f') ' ms \end{tabular}'],...
				['\begin{tabular}{c} Packet \\ loss \\ /' num2str(Maxi(5)*100, '%6.3f') ' \% \end{tabular}'],...
        ['\begin{tabular}{c} Cost \\ /' num2str(Maxi(6), '%4.2f') ' \end{tabular}'],...
				['\begin{tabular}{c} HO number \\ /' num2str(Maxi(7)) ' \end{tabular}'], ...
                ['\begin{tabular}{c} RSSI \\ ' num2str(Maxi(8)) ' \end{tabular}'], ...
                ['\begin{tabular}{c} Decision \\ criteria \end{tabular}']}, 'TickLabelInterpreter', 'latex'); 
        ylabel(['Normalized values, ' convertToBetterName(traffic{i}) convertToBetterName(load{j})],'Interpreter','Latex'); grid on;
        legend('RSS','SAW','GRA','TOPSIS','VIKOR','Location','northoutside','Orientation','horizontal');
        print(['../../../PaperAfterThesis/bilder/critBar_' traffic{i} '_' load{j}],'-depsc')
    end
end
