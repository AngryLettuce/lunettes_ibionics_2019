function angleMat = genAngleTable_V3(pixMat, resolution, d, memsAngle)

angleMat = int16(zeros(resolution(2), resolution(1),2));


for j = 1:resolution(2)
    for i = 1:resolution(1)
        
        [aX, aY] = XYZ2Angle(pixMat(j,1), pixMat(i,2), pixMat(1,3), d, memsAngle);


        angleMat(j,i, 1) = int16(aX * 1000);
        angleMat(j,i, 2) = int16(aY * 1000);
    end
end 

%save(strcat('angles', '_', num2str(resolution(1)), '_', num2str(resolution(2)), '.mat'), 'angleMat', '-v7.3')
end

