function evalMtx = mapToCritic (input,upward)

if (upward == true)
  input=1./input;
end

rows = size(input , 1);
columns = size (input, 2);

for i=1:rows
  maxI=max(input(:,i));
  for j=1:columns
    if input(j,i)>0.8*maxI
      evalMtx(j,i)=1;
    elseif (input(j,i)>0.6*maxI && input(j,i)<=0.8*maxI)
      evalMtx(j,i)=3;
    elseif (input(j,i)>0.4*maxI && input(j,i)<=0.6*maxI)
      evalMtx(j,i)=5;
    elseif (input(j,i)>0.2*maxI && input(j,i)<=0.4*maxI)
      evalMtx(j,i)=7;
    else
      evalMtx(j,i)=9;
    end
  end
end
