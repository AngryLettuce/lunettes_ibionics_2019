function [wall, wallcorner] = findWallSurfaceSize(N, Vlm, Zmw, maxAngles)
wall = [];
delta = 0.01 ;

for fact = [1 -1]
    if fact == 1
        aY = maxAngles(3); %Max angle for left position
    else
        aY = maxAngles(4); %Max angle for right position
    end
    for aX = fact * maxAngles(2):-fact* delta:fact*maxAngles(1) %Max angles for up and down
%         aY = -fact*5 ;
        Xn = -Zmw/tand(180-90+aX);
        Yn = -Zmw/tand(180-90-22+aY);
        NULC = [Xn Yn Zmw];
        [X, Y, Z] = findReflectedVector(Vlm, NULC, Zmw);
        wall = vertcat(wall, [X Y Z]);
    end
    
%     New wall surface (limited for 200x200)
    Xn = -Zmw/tand(180-90+maxAngles(2));
    Yn = -Zmw/tand(180-90-22+maxAngles(4));
    NULC = [Xn Yn Zmw];
    [X1, Y1, Z] = findReflectedVector(Vlm, NULC, Zmw);
    
    Xn = -Zmw/tand(180-90);
    Yn = -Zmw/tand(180-90-22+maxAngles(3));
    NULC = [Xn Yn Zmw];
    [X2, Y2, Z] = findReflectedVector(Vlm, NULC, Zmw);
    
    Xn = -Zmw/tand(180-90+maxAngles(1));
    Yn = -Zmw/tand(180-90-22+maxAngles(4));
    NULC = [Xn Yn Zmw];
    [X3, Y3, Z] = findReflectedVector(Vlm, NULC, Zmw);
    
    wallcorner = [X1 Y1 Z;
                  X1 Y2 Z;
                  X3 Y2 Z;
                  X3 Y3 Z];
end

% for aX = -5:delta:5
%     aY = 5;
%     Xn = -Zmw/tand(180-90+aX);
%     Yn = -Zmw/tand(180-90-22+aY);
%     NULC = [Xn Yn Zmw];
%     [X, Y, Z] = findReflectedVector(Vlm, NULC, Zmw);
%     wall = vertcat(wall, [X Y Z]);
% end
    
% % Finding upper left corner coordinates (ULC)
% aY = -5;
% aX = -5;
% Xn = -Zmw/tand(180-90+aX);
% Yn = -Zmw/tand(180-90-22+aY);
% NULC = [Xn Yn Zmw];
% [X, Y, Z] = findReflectedVector(Vlm, NULC, Zmw);
% ULC = [X Y Z];
% 
% % Finding lower left corner coordinates (LLC)
% aY = -5;
% aX = -5;
% Xn = -Zmw/tand(180-90+aX);
% Yn = -Zmw/tand(180-90-22+aY);
% NLLC = [Xn Yn Zmw];
% [X, Y, Z] = findReflectedVector(Vlm, NLLC, Zmw);
% LLC = [X Y Z];
% 
% % Finding lower right corner coordinates (LRC)
% aY = 5;
% aX = -5;
% Xn = -Zmw/tand(180-90+aX);
% Yn = -Zmw/tand(180-90-22+aY);
% NLRC = [Xn Yn Zmw];
% [X, Y, Z] = findReflectedVector(Vlm, NLRC, Zmw);
% LRC = [X Y Z];
% 
% % Finding upper right corner coordinates (URC)
% aY = 5;
% aX = 5;
% Xn = -Zmw/tand(180-90+aX);
% Yn = -Zmw/tand(180-90-22+aY);
% NURC = [Xn Yn Zmw];
% [X, Y, Z] = findReflectedVector(Vlm, NURC, Zmw);
% URC = [X Y Z];
% 
% wall =[URC;
%         ULC;
%         LLC;
%         LRC];
%fill3(wall(:,1)', wall(:,2)', wall(:,3)', 'yellow');

end