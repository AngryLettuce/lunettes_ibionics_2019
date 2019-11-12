clearvars
close all
clc
addpath("../");

resolution = 200;
nbOfPointX  = 5;
nbOfPointY  = 4;
angleMat = zeros(resolution, resolution, 2);
[X,Y] = meshgrid(0:resolution/(nbOfPointX-1):resolution, 0:resolution/(nbOfPointY-1):resolution);
[Xq,Yq] = meshgrid(0:resolution-1, 0:resolution-1);

anglesX = [%1.48, 1.46, 1.42, 1.42, 1.44; ...
           2.68, 2.84, 2.76, 2.74, 2.66; ...
           1.78, 1.70, 1.64, 1.56, 1.54; ...
           0.90, 0.80, 0.74, 0.64, 0.56; ...
           0.04, -0.02, -0.12, -0.22, -0.26];
       
anglesY = [%-0.94, -0.32, 0.38, 1.06, 1.82; ...
           -0.88, -0.24, 0.30, 0.94, 1.82; ...
           -0.94, -0.32, 0.24, 0.90, 1.76; ...
           -1.02, -0.38, 0.22, 0.90, 1.72; ...
           -1.08, -0.42, 0.2, 0.88, 1.68];
       
Vx = interp2(X,Y,anglesX,Xq,Yq);
Vy = interp2(X,Y,anglesY,Xq,Yq);

angleMat(:,:,1) = round(Vx * 1000);
angleMat(:,:,2) = round(Vy * 1000);

mat2LookUp(angleMat)

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




