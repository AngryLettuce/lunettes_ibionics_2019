function  sequence2LookUp(fileName,sequenceName, sequence)

    dlmwrite(fileName,strcat('static constexpr float', {' '}, sequenceName, '[', num2str(length(sequence)),']', '[2] = ') ,'delimiter','');

    dlmwrite(fileName,'{','delimiter','','-append');
    values = [];
    for i = 1:length(sequence)
        values = strcat(values,'{', num2str(sequence(i,1)), ',', num2str(sequence(i,2)), '},');
    end
    
    %enleve la derniere virgule de trop
    if i == length(sequence) 
        values = strcat(values(1:end-1), '}');
    else  
        values = strcat(values(1:end-1), '},');
    end
    
    dlmwrite(fileName,values,'delimiter','','-append');

    dlmwrite(fileName,'};','delimiter','','-append');
end

