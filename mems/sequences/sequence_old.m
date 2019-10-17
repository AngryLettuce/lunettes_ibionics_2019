clc;
clearvars -except table angleMat;
close all;

%% load table

%load only if not already loaded
if exist('angleMat') ~= 1
    angleMat = load('angles_400_400.mat');
    angleMat = angleMat.angleMat;
end



%% Rectangle Sequence

leftBorder = squeeze(angleMat(:, 1, :));
bottomBorder = squeeze(angleMat(400, 2:end, :));
rightBorder = squeeze(angleMat(end-1:-1:1, 400, :));
topBorder = squeeze(angleMat(1, end-1:-1:2, :));

rectSequence = double(vertcat(leftBorder, bottomBorder, rightBorder, topBorder))/1000;


%% spiral sequence 

spiralRes = 100;
% given values
pos = [200 200 ;    % startpoint
       340 340 ] ;  % endpoint
nturns = 3 ;    % number of turns (integer value)
% engine
dp = diff(pos,1,1) ;
R = hypot(dp(1), dp(2)) ;
phi0 = atan2(dp(2), dp(1)) ;
phi = linspace(0, nturns*2*pi, spiralRes) ; % 10000 = resolution
r = linspace(0, R, numel(phi)) ;
x = round(pos(1,1) + r .* cos(phi + phi0)) ;
y = round(pos(1,2) + r  .* sin(phi + phi0)) ;


spiralSequence = (zeros(length(x), 2));
for i = [1:spiralRes] 
    spiralSequence(i,1)  = double(angleMat(x(1,i), y(1,i), 1)) / 1000;
    spiralSequence(i,2)  = double(angleMat(x(1,i), y(1,i), 2)) / 1000;
    
end


%% closing rectangle sequence 
closingRectSequence = [];
for closing = [0:40:399]

    leftBorder = squeeze(angleMat(:, 1, :));
    bottomBorder = squeeze(angleMat(400, 2:end, :));
    rightBorder = squeeze(angleMat(end-1:-1:1, 400, :));
    topBorder = squeeze(angleMat(1, end-1:-1:2, :));

    closingRectSequence = double(vertcat(closingRectSequence, leftBorder, bottomBorder, rightBorder, topBorder))/1000;

end

%% plot Sequence 
posRectSequence = angleSequence2Pos(rectSequence);
posSpiralSequence = angleSequence2Pos(spiralSequence);
posClosingRectSequence = angleSequence2Pos(closingRectSequence);
axisLimits = [-75 75 -75 75];

figure()
plot(posRectSequence(:,1), posRectSequence(:,2))
axis(axisLimits)
title('Simple Rectangle')

figure()
plot(posSpiralSequence(:,1), posSpiralSequence(:,2), '.')
axis(axisLimits)
title('Spiral')

figure()
plot(posClosingRectSequence(:,1), posClosingRectSequence(:,2))
axis(axisLimits)
title('Closing Rectangle')

