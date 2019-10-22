function wallcorners = findWallCorners(Vlm, Zmw, maxAngles)
    memsAngle  = 21;
    Xn = -Zmw/tand(180-90+maxAngles(2));
    Yn = -Zmw/tand(180-90-memsAngle+maxAngles(4));
    NULC = [Xn Yn Zmw];
    [X1, Y1, Z] = findReflectedVector(Vlm, NULC, Zmw);
    
    Xn = -Zmw/tand(180-90);
    Yn = -Zmw/tand(180-90-memsAngle+maxAngles(3));
    NULC = [Xn Yn Zmw];
    [X2, Y2, Z] = findReflectedVector(Vlm, NULC, Zmw);
    
    Xn = -Zmw/tand(180-90+maxAngles(1));
    Yn = -Zmw/tand(180-90-memsAngle+maxAngles(4));
    NULC = [Xn Yn Zmw];
    [X3, Y3, Z] = findReflectedVector(Vlm, NULC, Zmw);
    
    wallcorners = [X1 Y1 Z;
                  X1 Y2 Z;
                  X3 Y2 Z;
                  X3 Y3 Z];
end