clc
clear all
close all

%% testing .h table

%video resolution
resolution = [200, 200];
%XYZ_precision = 0.01;
memsAngle = 21;

% Wall position relative to MEMS
Zmw = 27;


Xlm = 0;
Ylm = 0;
Zlm = -1;
Vlm = [Xlm Ylm Zlm];

%maximum angles
maxaX = 4.14;
minaX = 0.18;
maxaY = 4.2;
minaY = -0.14;
maxAngles = [minaX maxaX minaY maxaY];

wallcorners = findWallCorners(Vlm, Zmw, maxAngles, memsAngle); % laser's range at wall

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
        [xpos, ypos] = angle2XY(double(angleMat(x,y,1))/1000, double(angleMat(x,y,2))/1000, Zmw, Vlm, memsAngle);
        X(i) = xpos;
        XpixMat(i) = pixMat(x,1);
        YpixMat(i) = pixMat(y,2);
        Y(i) = ypos;
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

