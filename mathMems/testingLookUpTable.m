clc
clear all
close all

%% testing .h table

% Laser's position relative to MEMS
Xlm = 0;
Ylm = 0;
Zlm = 18.5;

Vlm = [Xlm Ylm Zlm];

Zmw = 1000;

aX = -3.690;
aY = 4.486;
%Default Normal of the mems
N = memsNorm(aX, aY);
normX = [0 N(1)];
normY = [0 N(2)];
normZ = [0 N(3)];

% Default reflected vector
[Xmw, Ymw, Zmw] = findReflectedVector(Vlm, N, Zmw);
laserX = [0 Xmw];
laserY = [0 Ymw];
laserZ = [0 Zmw];