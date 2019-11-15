clearvars
close all
clc
addpath("../");

resolution = 200;
nbOfPointX  = 5;
nbOfPointY  = 4;

[X,Y] = meshgrid(0:resolution/(nbOfPointX-1):resolution, 0:resolution/(nbOfPointY-1):resolution);
[Xq,Yq] = meshgrid(0:resolution-1, 0:resolution-1);

anglesX = [2.38, 2.44, 2.58, 2.70, 2.88; ...
           0.94, 1.02, 1.10, 1.14, 1.18; ...
           -0.16, -0.18, -0.18, -0.16, -0.16; ...
           -1.36, -1.40, -1.40, -1.42, -1.48; ...
          % 0.04, -0.02, -0.12, -0.22, -0.26
          ];
       
anglesY = [-2.54, -1.02, 0.40, 1.86, 3.78; ...
           -2.56, -0.96, 0.44, 1.92, 3.90; ...
           -2.54, -0.94, 0.46, 1.98, 3.92; ...
           -2.58, -0.94, 0.48, 2.04, 4.07; ...
           %-1.08, -0.42, 0.2, 0.88, 1.68
           ];
       
Vx = interp2(X,Y,anglesX,Xq,Yq);
Vy = interp2(X,Y,anglesY,Xq,Yq);

angleMat = zeros(resolution, resolution, 2);
angleMat(:,:,1) = round(Vx * 1000);
angleMat(:,:,2) = round(Vy * 1000);

mat2LookUp(angleMat)



