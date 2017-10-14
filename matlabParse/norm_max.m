function normMatr = norm_max(matrix)

dim = size(matrix,2);

for i=1:dim
  normMatr(:,i) = matrix(:,i)./max(matrix(:,i));

end

