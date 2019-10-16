function angleMat = genAngleTable_V2(pixMat, xyzMat, resolution)
% not working yet
angleMat = int16(zeros(resolution(2), resolution(1),2));
% modules = zeros(length(xyzMat),1);
% idx = 4080510;
% deltaidx = 500000;

for j = 1:resolution(2)
    for i = 1:resolution(1)
        
%         minIdx = max(1, idx -deltaidx);
%         maxIdx = min(length(xyzMat), idx + deltaidx);
        modules = (pixMat(j,1)-xyzMat(:,1)).^2 + (pixMat(i,2)-xyzMat(:,2)).^2;
        [M, I] = min(modules);
        aX = xyzMat(I, 4);
        aY = xyzMat(I, 5);
        %module = sqrt(pixMat(j,1)^2 + pixMat(i,2));
        
        %[aX, aY] = getAngle(pixMat(j,1), pixMat(i,2), 1000, xyzMat);
        
        %convert the value to int16, multply the value by 1000 since the
        %maximum angle variation is 0.001 there is no losses in precision
        angleMat(j,i, 1) = int16(aX * 1000);
        angleMat(j,i, 2) = int16(aY * 1000);
    end
end 

%save(strcat('angles', '_', num2str(resolution(1)), '_', num2str(resolution(2)), '.mat'), 'angleMat', '-v7.3')
end

