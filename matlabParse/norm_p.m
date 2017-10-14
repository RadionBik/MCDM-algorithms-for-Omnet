function normMatr = norm_p(matrix,p)

dim = size(matrix,2);

for i=1:dim
  normMatr(:,i) = matrix(:,i)./norm(matrix(:,i),p);
end

