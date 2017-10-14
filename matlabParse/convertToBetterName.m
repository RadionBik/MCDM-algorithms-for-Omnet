function var = convertToBetterName(var)
    if (strcmp(var,'conv')==1)
        var = 'VoIP ';
    elseif (strcmp(var,'stream')==1)
        var = 'IPTV ';
    elseif (strcmp(var,'low')==1)
         var = 'low load';
    elseif (strcmp(var,'high')==1)
         var = 'high load';
    end
end