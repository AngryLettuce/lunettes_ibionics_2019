clc;
clearvars;
close all;

%% Constants 
%video resolution
resolution = [200, 200];

% Wall position relative to MEMS
Zmw = 301;

%precision on the MEMS angles
XYZ_precision = 0.01;
angleBuffer = 1;

%MEMS angles
angleXOffset = 0.0001;
angleYOffset = 0.2101;
memsAngleY = 21 + angleYOffset;
memsAngleX = 0  + angleXOffset;
memsAngles  = [memsAngleX, memsAngleY];

%Incident vector (laser)
Xlm = -0.0745;
Ylm = -0.0969;
Zlm = -1.01276;
Vlm = [Xlm Ylm Zlm];
Vlm = Vlm/norm(Vlm);

%maximum angles
minaX = 0.34;
maxaX = 4.22;
minaY = -0.52;
maxaY = 3.46;
maxAngles = [minaX maxaX minaY maxaY];

%% Getting the wall surface 
wallcorners = findWallCorners(Vlm, Zmw, maxAngles, memsAngles); % laser's range at wall
pixMat = genPixMat(wallcorners, resolution);
%% angle matrix and XYZ matrix
tic
% Gen  XYZ tables
%add the buffer to the max angles to allow the MEMS to reach all corners
maxAngles = [minaX-angleBuffer maxaX+angleBuffer minaY-angleBuffer maxaY+angleBuffer];
table = genXYZ(Vlm, maxAngles, Zmw, XYZ_precision, memsAngles);

%angles lookup table
angleMat = genAngleTable_V2(pixMat, table, resolution);
toc
mat2LookUp(angleMat);




