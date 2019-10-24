clc;
clearvars;
close all;
%addpath('../')


%% Constants and Variables

%video resolution
resolution = [11, 11];
XYZ_precision = 0.1;
memsAngle = 21;

% Wall position relative to MEMS
Zmw = 1000;


Xlm = 0;
Ylm = 0;
Zlm = -1;
Vlm = [Xlm Ylm Zlm];

%maximum angles
maxaX = 3.7;
minaX = -3.7;
maxaY = 3.5;
minaY = -3.5;
maxAngles = [minaX maxaX minaY maxaY];

%% Getting the wall surface 
wallcorners = findWallCorners(Vlm, Zmw, maxAngles, memsAngle); % laser's range at wall

pixMat = genPixMat(wallcorners, resolution);

%% angle matrix and XYZ matrix

% Gen  XYZ tables
%table = genXYZ(Vlm, maxAngles, Zmw, XYZ_precision);
table = genXYZ(Vlm, maxAngles, Zmw, XYZ_precision, memsAngle);

tic
%angles lookup table
angleMat = genAngleTable_V2(pixMat, table, resolution);
toc
%mat2LookUp(angleMat);




