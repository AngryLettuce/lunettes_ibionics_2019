clc
clear all
close all

%% testing .h table

%video resolution
resolution = [200, 200];
%XYZ_precision = 0.01;
%MemsAngle;
angleXOffset = -0.4156;
angleYOffset = -0.1129;
memsAngleY = 21 + angleYOffset;
memsAngleX = 0  + angleXOffset;
memsAngles  = [memsAngleX, memsAngleY];
% Wall position relative to MEMS
Zmw = 301;


Xlm = -0.04665;
Ylm = -0.07412;
Zlm = -0.9665;
Vlm = [Xlm Ylm Zlm];
Vlm = Vlm/norm(Vlm);

%maximum angles
minaX = 0.34;
maxaX = 4.22;
minaY = -0.52;
maxaY = 3.46;
maxAngles = [minaX maxaX minaY maxaY];

wallcorners = findWallCorners(Vlm, Zmw, maxAngles, memsAngles); % laser's range at wall

pixMat = genPixMat(wallcorners, resolution);

angleTable = load('angles_200_200.mat');
angleMat = angleTable.angleMat;

X = zeros(resolution(1) * resolution(2),1);
Y = zeros(resolution(1) * resolution(2),1);
XpixMat = zeros(resolution(1) * resolution(2),1);
YpixMat = zeros(resolution(1) * resolution(2),1);
i = 1;
for x = 1:resolution(1) 
    for y = 1:resolution(2)
        [xpos, ypos] = angle2XY(double(angleMat(x,y,1))/1000, double(angleMat(x,y,2))/1000, Zmw, Vlm, memsAngles);
        X(i) = xpos;
        Y(i) = ypos;
        XpixMat(i) = pixMat(x,1);
        YpixMat(i) = pixMat(y,2);
        
        i = i + 1;
    end
end


figure 
plot(X,Y, '.')

figure
plot(XpixMat, YpixMat, '.')


figure
hold on
plot(X,Y,'.')
plot(XpixMat, YpixMat, '.')
hold off

