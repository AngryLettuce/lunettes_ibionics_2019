function wallcorners = findWallCorners(d, Z, maxAngles, memsAngle)
%     Xn = -Zmw/tand(180-90+maxAngles(2));
%     Yn = -Zmw/tand(180-90-memsAngle+maxAngles(4));
%     NULC = [Xn Yn Zmw];
%     [X1, Y1, Z] = findReflectedVector(Vlm, NULC, Zmw);
%     
%     Xn = -Zmw/tand(180-90);
%     Yn = -Zmw/tand(180-90-memsAngle+maxAngles(3));
%     NULC = [Xn Yn Zmw];
%     [X2, Y2, Z] = findReflectedVector(Vlm, NULC, Zmw);
%     
%     Xn = -Zmw/tand(180-90+maxAngles(1));
%     Yn = -Zmw/tand(180-90-memsAngle+maxAngles(4));
%     NULC = [Xn Yn Zmw];
%     [X3, Y3, Z] = findReflectedVector(Vlm, NULC, Zmw);

    %top left (max aX, min aY)
    [X1,Y1] = angle2XY(maxAngles(2), maxAngles(3), Z, d, memsAngle);
    %left bottom (min aX, min aY)
    [X2,Y2] = angle2XY(maxAngles(1), maxAngles(3), Z, d, memsAngle);
    %right bottom (min aX, max aY)
    [X3,Y3] = angle2XY(maxAngles(1), maxAngles(4), Z, d, memsAngle);
    %top right (max aX, max aY)
    [X4,Y4] = angle2XY(maxAngles(2), maxAngles(4), Z, d, memsAngle);
    
    wallcorners = [X1 Y1 Z;
                  X2 Y2 Z;
                  X3 Y3 Z;
                  X4 Y4 Z];
end