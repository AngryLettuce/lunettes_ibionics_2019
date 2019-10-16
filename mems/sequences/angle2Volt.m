%% angle to volt conversion
close all
clearvars -except angleMat
clc

if exist('angleMat') ~= 1
    angleMat = load('angles_400_400.mat');
    angleMat = angleMat.angleMat;
end

K   = 0.0486;
epsilon = 0.067;
wn  = 16567;

filterfreq = 1000; % Hz
Tfinal = 1e-3;
% v = 100:100;

%filter 
[b,a] = besself(6,filterfreq*2*pi);
bessel = tf(b,a);

i = 1;
for v = -144:1:145
X1 = K * tf([wn^2],[1 2*epsilon*wn +wn^2]) * bessel;
X2 = v * X1 ;


y = step(X2, Tfinal);
yi(i) = y(end);
vi(i) = v;
i = i+1;
end

figure
plot(vi, yi)
