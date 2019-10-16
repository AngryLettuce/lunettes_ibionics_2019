clc;
clearvars;
close all;
%addpath('../')


%% Constants and Variables

%video resolution
resolution = [400, 400];

% Wall position relative to MEMS
Zmw = 1000;

% Laser's position relative to MEMS
Xlm = 0;
Ylm = 0;
Zlm = 18.5;

Vlm = [Xlm Ylm Zlm];

%maximum angles
maxaX = 3.7;
minaX = -3.7;
maxaY = 4.5;
minaY = -3.5;
maxAngles = [minaX maxaX minaY maxaY];

%% Getting the wall surface 
wallcorners = findWallCorners(Vlm, Zmw, maxAngles); % laser's range at wall

pixMat = genPixMat(wallcorners, resolution);
xCoord = wallcorners(1,1);
yCoord = wallcorners(1,2);
zCoord = wallcorners(1,3);

%% angle matrix and XYZ matrix
XYZ_precision = 0.01;
tic

% Gen  XYZ tables
% if exist(strcat('XYZ_', num2str(Zmw),'_', num2str(XYZ_precision), '.mat')) ~= 2
%     
% end
table = genXYZ(Vlm, maxAngles, Zmw, XYZ_precision);
%load only if not already loaded
% if exist('table') ~= 1
%     table = load(strcat('XYZ_', num2str(Zmw),'_', num2str(XYZ_precision), '.mat'));
%     table = table.array;
% end

%angles lookup table

% if exist(strcat('angles', '_', num2str(resolution(1)), '_', num2str(resolution(2)), '.mat')) ~= 2
angleMat = genAngleTable_V2(pixMat, table, resolution);

%mat2LookUp(angleMat);
% end
toc
%load only if not already loaded
% if exist('angleMat') ~= 1
%     angleMat = load(strcat('angles', '_', num2str(resolution(1)), '_', num2str(resolution(2)), '.mat'));
%     angleMat = angleMat.angleMat;
% end


