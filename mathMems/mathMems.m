clc;
clearvars -except table angleMat;
close all;

%% Settings
switchplot = 1; % 1 for figure with 2 views, 0 for 1 view

%% Constants and Variables
% received pupil position
X = 100;
Y = 100;

%video resolution
resolution = [400, 400];

% Head' radius (in mm)
r = 71;

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

% Default angles
aX = 0;
aY = 0;

%Default Normal of the mems
N = memsNorm(aX, aY);
normX = [0 N(1)];
normY = [0 N(2)];
normZ = [0 N(3)];

% Default reflected vector
[Xmw, Ymw, Zmw] = findReflectedVector(Vlm, N, Zmw);
Vmw = [Xmw Ymw Zmw];
laserX = [0 Xmw];
laserY = [0 Ymw];
laserZ = [0 Zmw];


%% Making MEMS, wall and hole surfaces
mems = surfaceMatrix(N, 5, 5, [0 0 0]); 
memsMatX = mems.X;
memsMatY = mems.Y;
memsMatZ = mems.Z;

[wall, wallcorners] = findWallSurfaceSize(N, Vlm, Zmw, maxAngles); % laser's range at wall
area = 1; %test for now needs to be there in order to compile

pixMat = genPixMat(wallcorners, resolution);
xCoord = wallcorners(1,1);
yCoord = wallcorners(1,2);
zCoord = wallcorners(1,3);


%% angle matrix and XYZ matrix

% % Gen  XYZ tables
% if exist('XYZ_1000_0.01.mat') ~= 2
%     genXYZ(Vlm, maxAngles, Zmw, 0.01);
% end
% 
% %load only if not already loaded
% if exist('table') ~= 1
%     table = load('XYZ_1000_0.01.mat');
% end


%angles lookup table
% if exist(strcat('angles', '_', num2str(resolution(1)), '_', num2str(resolution(2)), '.mat')) ~= 2
%     genAngleTable(pixMat, table.array, resolution)
% end

%load only if not already loaded
if exist('angleMat') ~= 1
    angleMat = load(strcat('angles', '_', num2str(resolution(1)), '_', num2str(resolution(2)), '.mat'));
    angleMat = angleMat.angleMat;
end

%% Making figure
switch switchplot
    
    case 1 %Laser on wall without angle limitation
        [laserplot, normplot, fig_h] = plotSetup(Vlm, N, Vmw, mems, wall, 'User View');

        laserplot.XDataSource = 'laserX';
        laserplot.YDataSource = 'laserY';
        laserplot.ZDataSource = 'laserZ';

        normplot.XDataSource = 'normX';
        normplot.YDataSource = 'normY';
        normplot.ZDataSource = 'normZ';
        
    case 2 %Laser on wall and glasses view with angle limmitations
        [laserplot1, laserplot2, normplot1, normplot2, memsplot2, fig_h] = plotSetup2(Vlm, N, Vmw, mems, wall, wallcorners, pixMat);

        laserplot1.XDataSource = 'laserX';
        laserplot1.YDataSource = 'laserY';
        laserplot1.ZDataSource = 'laserZ';
        laserplot2.XDataSource = 'laserX';
        laserplot2.YDataSource = 'laserY';
        laserplot2.ZDataSource = 'laserZ';

        normplot1.XDataSource = 'normX';
        normplot1.YDataSource = 'normY';
        normplot1.ZDataSource = 'normZ';
        normplot2.XDataSource = 'normX';
        normplot2.YDataSource = 'normY';
        normplot2.ZDataSource = 'normZ';

        memsplot2.XDataSource = 'memsMatX';
        memsplot2.YDataSource = 'memsMatY';
        memsplot2.ZDataSource = 'memsMatZ';
    
    case 3 %Generate video head with laser on wall from pupil position
        v = VideoReader('../testBench/video_eye/4.avi');
        vw = VideoWriter('videoTest.avi');
        XYpos = dlmread('../testBench/video_eye/4.txt');
        vw.FrameRate = 95; %95 dans le video
        open(vw);
        figure('units','normalized','outerposition',[0 0 1 0.5])
%         subplot(1,2,1)
        [laserplot1, normplot1, pupilDot] = plotSetup3(Vlm, N, Vmw, mems, wallcorners);
%         subplot(1,2,2)
%         [laserplot2, normplot2] = plotSetup4(Vlm, N, Vmw, mems, wallcorners);

        laserplot1.XDataSource = 'laserX';
        laserplot1.YDataSource = 'laserY';
        laserplot1.ZDataSource = 'laserZ';
        laserplot2.XDataSource = 'laserX';
        laserplot2.YDataSource = 'laserY';
        laserplot2.ZDataSource = 'laserZ';

        normplot1.XDataSource = 'normX';
        normplot1.YDataSource = 'normY';
        normplot1.ZDataSource = 'normZ';
        normplot2.XDataSource = 'normX';
        normplot2.YDataSource = 'normY';
        normplot2.ZDataSource = 'normZ';

        memsplot2.XDataSource = 'memsMatX';
        memsplot2.YDataSource = 'memsMatY';
        memsplot2.ZDataSource = 'memsMatZ';
        
        pupilDot.XDataSource = 'xCoord';
        pupilDot.YDataSource = 'yCoord';
        pupilDot.ZDataSource = 'zCoord';

        for i = 1:v.NumberOfFrames
            realPos = round(XYpos(i,:));
            
%             xCoord = pixMat(realPos(1),1);
%             yCoord = pixMat(realPos(2),2);

%             [aX, aY] = getAngle(xCoord, yCoord, Zmw, table.array);
            aX = angleMat(realPos(2), realPos(1), 1);
            aY = angleMat(realPos(2), realPos(1), 2);
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

            refreshdata
            drawnow limitrate
%             pause(0.25);
            
            frame = getframe(gcf);
            writeVideo(vw,frame);
            i
        end
        close(vw)
        
    case 4 %Merging of case3 and pupil video side by side
        sideBySideVid('videoTest.avi', '../testBench/video_eye/4.avi', 'concatenate.avi', 95)
        
    case 5 %test case
          [laserplot1, normplot1, pupilDot] = plotSetup3(Vlm, N, Vmw, mems, wallcorners);
    otherwise 
end

%% Old code that might be usefull later

% Max left position (using head)
% syms X Y Z
% Head equation
% head = (Y + 70)^2 + (Z + 20)^2 - r^2;
% m1 = Z^2 + Y^2 == -20*Z - 70*Y;
% [Ymw, Zsym] = solve([head m1],[Y Z]);
% 
% if double(Zsym(1)) < 0
%     Ymw = double(Ymw(2));
%     Zz = double(Zsym(2));
% else
%     Ymw = double(Ymw(1));
%     Zz = double(Zsym(1)); 
% end
% fact = Zmw/Zz;
% 
% Xmw = fact*Xmw;
% Ymw = fact*Ymw;
% Zmw = fact*Zz;

% Vmw = [Xmw Ymw Zmw];

% lPosMaxHead = Ymw;




