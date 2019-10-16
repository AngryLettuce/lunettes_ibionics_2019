function  mat2LookUp(angleMat)


dlmwrite('lookUpTable.h','static constexpr short angleTable[400][400][2] = ','delimiter','');

dlmwrite('lookUpTable.h','{','delimiter','','-append');

for x = 1:400
    values ='{';
    for y = 1:400
        values = strcat(values,'{', num2str(angleMat(x,y,1)), ',', num2str(angleMat(x,y,2)), '},');
    end
    
    %enleve la derniere virgule de trop
    if x == 400 
        values = strcat(values(1:end-1), '}');
    else  
        values = strcat(values(1:end-1), '},');
    end
    
    dlmwrite('lookUpTable.h',values,'delimiter','','-append');
end

dlmwrite('lookUpTable.h','};','delimiter','','-append');