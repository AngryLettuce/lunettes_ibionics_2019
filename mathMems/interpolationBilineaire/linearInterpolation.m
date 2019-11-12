clearvars
close all
clc
addpath("../");

resolution = 400;
nbOfPoint  = 5;
angleMat = zeros(resolution, resolution, 2);
[X,Y] = meshgrid(0:resolution/(nbOfPoint-1):resolution, 0:resolution/(nbOfPoint-1):resolution);
[Xq,Yq] = meshgrid(0:resolution-1, 0:resolution-1);

anglesX = [1, 2, 3, 4, 5; ...
           6, 7, 8, 9, 10; ...
           11, 12, 13, 14, 15; ...
           16, 16, 18, 19, 20; ...
           21, 22, 23, 24, 25];
       
anglesY = [1, 2, 3, 4, 5; ...
           6, 7, 8, 9, 10; ...
           11, 12, 13, 14, 15; ...
           16, 16, 18, 19, 20; ...
           21, 22, 23, 24, 25];
       
Vx = interp2(X,Y,anglesX,Xq,Yq);
Vy = interp2(X,Y,anglesY,Xq,Yq);

angleMat(:,:,1) = Vx;
angleMat(:,:,2) = Vy;

% valuesOfCornersX = zeros(4,1);
% valuesOfCornersY = zeros(4,1);
       
% for i = 0:resolution
%     for j = 0:resolution
%         point = [i,j];
%         [corners, cornersIndexes] = findNearestCorners(X,Y,point);
%         for k = 1:length(cornersIndexes)
%            cornerIndex = cornersIndexes{k};
%            valuesOfCornersX(k) = anglesX(cornerIndex(1), cornerIndex(2));
%            valuesOfCornersY(k) = anglesY(cornerIndex(1), cornerIndex(2));
%            valueX = myBiLinearInterp(point, corners, valuesOfCornersX);
%            valueY = myBiLinearInterp(point, corners, valuesOfCornersY);
%            angleMat(i+1,j+1,1) = valueX;
%            angleMat(i+1,j+1,2) = valueY;
%         end
%     end
% end


       
% [X,Y] = meshgrid(0:resolution:30, 0:resolution:30);
% data = zeros(length(X),length(X));
% for i = 1:length(X)
%     for j = 1:length(Y)
%         data(i,j) = i*j;
%     end
% end
% 
% point = [25,25];
% [corners, cornersIndexes] = findNearestCorners(X,Y,point);
% 
% valuesOfCorners = zeros(length(corners), 1);
% for i = 1:length(cornersIndexes)
%     cornerIndex = cornersIndexes{i};
%     valuesOfCorners(i) = data(cornerIndex(1), cornerIndex(2));
% end
% 
% value = myBiLinearInterp(point, corners, valuesOfCorners)




