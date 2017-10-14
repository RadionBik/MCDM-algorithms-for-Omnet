function matrix = invert(input, howToVector)

scanLen = size(howToVector,2);

for i=1:scanLen
    if (howToVector(i)~=0)
      matrix(:,i) = 1./input(:,i);  
    else
      matrix(:,i)=input(:,i);
    end
end

end