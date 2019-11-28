clearvars
close all
clc
addpath("../");

resolution = 200;
nbOfPointX  = 5;
nbOfPointY  = 4;

[X,Y] = meshgrid(linspace(0, resolution-1, nbOfPointX), linspace(0, resolution-1, nbOfPointY));
[Xq,Yq] = meshgrid(0:resolution-1, 0:resolution-1);

anglesX = [2.50, 2.56, 2.65, 2.75, 2.84; ...
           1.01, 1.08, 1.12, 1.15, 1.20; ...
           -0.09, -0.10, -0.13, -0.13, -0.14; ...
           -1.50, -1.31, -1.34, -1.40, -1.46; ...
          % 0.04, -0.02, -0.12, -0.22, -0.26
          ];
       
anglesY = [-3.13, -1.31, 0.02, 1.38, 3.22; ...
           -3.37, -1.36, 0.05, 1.44, 3.28; ...
           -3.30, -1.30, 0.06, 1.42, 3.28; ...
           -3.32, -1.28, 0.10, 1.49, 3.36; ...
           %-1.08, -0.42, 0.2, 0.88, 1.68
           ];
       
Vx = interp2(X,Y,anglesX,Xq,Yq);
Vy = interp2(X,Y,anglesY,Xq,Yq);

angleMat = zeros(resolution, resolution, 2);
angleMat(:,:,1) = round(Vx * 1000);
angleMat(:,:,2) = round(Vy * 1000);

mat2LookUp(angleMat)



