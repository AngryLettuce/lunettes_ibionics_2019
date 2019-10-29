function wallcorners = findWallCorners(d, Z, maxAngles, memsAngles)
    %top left (max aX, min aY)
    [X1,Y1] = angle2XY(maxAngles(2), maxAngles(3), Z, d, memsAngles);
    %left bottom (min aX, min aY)
    [X2,Y2] = angle2XY(maxAngles(1), maxAngles(3), Z, d, memsAngles);
    %right bottom (min aX, max aY)
    [X3,Y3] = angle2XY(maxAngles(1), maxAngles(4), Z, d, memsAngles);
    %top right (max aX, max aY)
    [X4,Y4] = angle2XY(maxAngles(2), maxAngles(4), Z, d, memsAngles);
    
    wallcorners = [X1 Y1 Z;
                  X2 Y2 Z;
                  X3 Y3 Z;
                  X4 Y4 Z];
end