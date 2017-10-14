function plotEval(ScoreConv,scoreStr,DecCritEval,focus)


%traffic = fieldnames(Score);  %or gra or saw, doesn't matter, the fields are the same
%load = fieldnames(Score.conv);

% for j=1:numel(load)
%     for i=1:numel(traffic)
%         fig.(traffic{i}).(load{j})= figure('Position', [100, 100, 400, 400]);
%         fig.(traffic{i}).(load{j}).PaperPositionMode = 'auto';
%         bar( [Score.(traffic{i}).(load{j})], 'BarWidth', 0.7);
%         set(gca,'XTickLabel',{['RSS'],['SAW'],['GRA'],['TOPSIS'],['VIKOR']}, 'TickLabelInterpreter', 'latex'); 
%         ylabel(['The evaluation score $$S_E$$, ' convertToBetterName(traffic{i}) convertToBetterName(load{j})],'Interpreter','Latex'); grid on;
%         ylim([0 0.6])
%         print(['../latex/bilder/evalBar_' focus '_' traffic{i} '_' load{j}],'-depsc')
%         %pause(1);
%     end
% end

fig.general = figure('Position', [100, 100, 700, 400]);
fig.general.PaperPositionMode = 'auto';
if(strcmp(focus,'cost'))
bar([ScoreConv.conv.low.'; scoreStr.stream.low.'; ScoreConv.conv.high.'; scoreStr.stream.high.'] )
else
bar([DecCritEval.conv.low.'; DecCritEval.stream.low.'; DecCritEval.conv.high.'; DecCritEval.stream.high.']);    
end
    
set(gca,'XTickLabel',{['VoIP (low load)'],['IPTV (low load)'],['VoIP (high load)'],['IPTV (high load)']},...
            'TickLabelInterpreter', 'latex'); 
legend('RSS','SAW','GRA','TOPSIS','VIKOR','Location','northoutside','Orientation','horizontal');
ylabel('The evaluation score $$S_E$$','Interpreter','Latex'); grid on;
print(['../../../PaperAfterThesis/bilder/evalBarGeneral' focus] ,'-depsc')

end