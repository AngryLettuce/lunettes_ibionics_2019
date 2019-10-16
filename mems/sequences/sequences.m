clc;
clearvars -except table angleMat;
close all;
addpath('../')

%% load table

%load only if not already loaded
if exist('angleMat') ~= 1
    angleMat = load('angles_400_400.mat');
    angleMat = angleMat.angleMat;
end 



%% Rectangle Sequence
step = 10;
% leftBorder = squeeze(angleMat(1:step:end, 1, :));
% bottomBorder = squeeze(angleMat(400, 2:step:end, :));
% rightBorder = squeeze(angleMat(end-1:-step:1, 400, :));
% topBorder = squeeze(angleMat(1, end-1:-step:2, :));
leftBorder = [0:step:399 ; zeros(1,400/step)];
bottomBorder = [ones(1, 400/step) * 399 ;  0:step:399];
rightBorder = [399:-step:0 ; ones(1,400/step) * 399];
topBorder = [zeros(1, 400/step) ; 399:-step:0];

rectSequence = horzcat(leftBorder, bottomBorder, rightBorder, topBorder);
rectSequence = rectSequence';
% rectSequence = double(vertcat(leftBorder, bottomBorder, rightBorder, topBorder))/1000;


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
%     spiralSequence(i,1)  = double(angleMat(x(1,i), y(1,i), 1)) / 1000;
%     spiralSequence(i,2)  = double(angleMat(x(1,i), y(1,i), 2)) / 1000;
      spiralSequence(i,1)  = x(1,i);
      spiralSequence(i,2)  = y(1,i);
    
end


%% closing rectangle sequence 
closingRectSequence = [];
step = 10;
for closing = [0:40:399]

%     leftBorder = squeeze(angleMat(1 + closing:step:end-closing, 1 + closing, :));
%     bottomBorder = squeeze(angleMat(400 - closing, 2 +closing:step:end - closing, :));
%     rightBorder = squeeze(angleMat(end-1 - closing:-step:1 + closing, 400 - closing, :));
%     topBorder = squeeze(angleMat(1 + closing, end-1 - closing:-step:2 + closing, :));
%     tempClosingRectSequence = double(vertcat(leftBorder, bottomBorder, rightBorder, topBorder)) / 1000;
%     closingRectSequence = vertcat(closingRectSequence, tempClosingRectSequence);    leftBorder = squeeze(angleMat(1 + closing:step:end-closing, 1 + closing, :));
        leftBorder = [0 + closing:step:399-closing ;  ones(1, (400 - 2 * closing)/step) *(0 + closing)];
        bottomBorder = [(ones(1, (400- 2*closing)/step) * (399 - closing)) ;  (0 +closing:step:399 - closing)];
        rightBorder = [399 - closing:-step:0 + closing ;  ones(1, (400- 2*closing)/step) * (399 - closing)];
        topBorder = [ones(1,(400-2*closing)/step) * (0 + closing); 399 - closing:-step:0 + closing];
        tempClosingRectSequence = horzcat(leftBorder, bottomBorder, rightBorder, topBorder);
        closingRectSequence = horzcat(closingRectSequence, tempClosingRectSequence);

end
closingRectSequence = closingRectSequence';

%% infinity sign sequence 
t = 0  :pi/50: 2*pi; %+ 5*pi/16;
a = 1;
x = (a .* sqrt(2) .*  cos(t)) ./ (sin(t).^2 + 1);
y = (a .* sqrt(2) .*  cos(t) .* sin(t)) ./  (sin(t).^2 + 1);

x = x / (max(x) /175) + 200;
y = y / (max(y) /150) + 200;


x = round(x);
y = round(y);
% figure
% plot(x,y, '.')
% axis([0 400 0 400])

infinitySequence = (zeros(length(x), 2));
for i = [1:length(x)] 
%     infinitySequence(i,1)  = double(angleMat(x(1,i), y(1,i), 1)) / 1000;
%     infinitySequence(i,2)  = double(angleMat(x(1,i), y(1,i), 2)) / 1000;
      infinitySequence(i,1)  = x(1,i);
      infinitySequence(i,2)  = y(1,i);
    
end

%% circularLoop Sequence
t = 0:pi/100:2*pi;
x = (sin(2*t) + 1) * 200;
y = (sin(3*t) + 1) * 200;

x = round(x);
y = round(y);

circularLoopSequence = (zeros(length(x), 2));
% x(x==0) = 1;
% y(y==0) = 1;
for i = [1:length(x)] 
%     circularLoopSequence(i,1)  = double(angleMat(x(1,i), y(1,i), 1)) / 1000;
%     circularLoopSequence(i,2)  = double(angleMat(x(1,i), y(1,i), 2)) / 1000;
      circularLoopSequence(i,1)  = x(1,i);
      circularLoopSequence(i,2)  = y(1,i);
end


%% flower sequence
b = 2.35;
a = 4.23;
t = -15:0.05:20;
x = ((a+b)*cos(t) - b*cos((a/b +1) * t));
y = ((a+b)*sin(t) - b*sin((a/b +1) * t));
x = round(mat2gray(x) * 400);
y = round(mat2gray(y) * 400);
flowerSequence = (zeros(length(x), 2));
% x(x==0) = 1;
% y(y==0) = 1;
for i = [1:length(x)] 
    %flowerSequence(i,1)  = double(angleMat(x(1,i), y(1,i), 1)) / 1000;
    %flowerSequence(i,2)  = double(angleMat(x(1,i), y(1,i), 2)) / 1000;
    flowerSequence(i,1)  = x(1,i);
    flowerSequence(i,2)  = y(1,i);
    
end

%% sequence to header file 
sequence2LookUp('rectangle_LUT.h', 'rectangle_LUT', rectSequence);
sequence2LookUp('spiral_LUT.h', 'spiral_LUT', spiralSequence);
sequence2LookUp('infinity_LUT.h', 'infinity_LUT', infinitySequence);
sequence2LookUp('closingRect_LUT.h', 'closingRect_LUT', closingRectSequence);
sequence2LookUp('circularLoop_LUT.h', 'circularLoop_LUT', circularLoopSequence);



%% plot index sequence
figure()
plot(rectSequence(:,1), rectSequence(:,2), '.')
axis([0 400 0 400])
title('Simple Rectangle')

figure()
plot(closingRectSequence(:,1), closingRectSequence(:,2), '.')
axis([0 400 0 400])
title('Closing Rectangle')

figure()
plot(spiralSequence(:,1), spiralSequence(:,2), '.')
axis([0 400 0 400])
title('Spiral')

figure()
plot(infinitySequence(:,1), infinitySequence(:,2), '.')
axis([0 400 0 400])
title('infinity')

figure()
plot(circularLoopSequence(:,1), circularLoopSequence(:,2), '.')
axis([0 400 0 400])
title('Circular Loop')

figure()
plot(flowerSequence(:,1), flowerSequence(:,2), '.')
axis([0 400 0 400])
title('Flower Sequence')



%% plot angles Sequence 
%{
posRectSequence = angleSequence2Pos(rectSequence);
posSpiralSequence = angleSequence2Pos(spiralSequence);
posClosingRectSequence = angleSequence2Pos(closingRectSequence);
posInfinitySequence = angleSequence2Pos(infinitySequence);
posCircularLoopSequence = angleSequence2Pos(circularLoopSequence);
posFlowerSequence = angleSequence2Pos(flowerSequence);

figure()
plot(posRectSequence(:,1), posRectSequence(:,2), '.')
axis([-150 150 -1250 -700])
title('Simple Rectangle')

figure()
plot(posSpiralSequence(:,1), posSpiralSequence(:,2), '.')
axis([-150 150 -1250 -700])
title('Spiral')

figure()
plot(posClosingRectSequence(:,1), posClosingRectSequence(:,2), '.')
axis([-150 150 -1250 -700])
title('Closing Rectangle')

figure()
plot(posInfinitySequence(:,1), posInfinitySequence(:,2), '.')
axis([-150 150 -1250 -700])
title('infinity Symbol')

figure()
plot(posCircularLoopSequence(:,1), posCircularLoopSequence(:,2), '.')
axis([-150 150 -1250 -700])
title('Circular Loop Sequence')

figure()
plot(posFlowerSequence(:,1), posFlowerSequence(:,2), '.')
axis([-150 150 -1250 -700])
title('Flower Sequence')

%}
