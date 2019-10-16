function keypressed( H,E )
aX = evalin('base', 'aX');
aY = evalin('base', 'aY');
Vlm = evalin('base', 'Vlm');
Zmw = evalin('base', 'Zmw');
maxAngles = evalin('base', 'maxAngles');

deltaAngle = 0.5;

switch E.Key
    case 'rightarrow'
        aY = aY + deltaAngle;
    case 'leftarrow'
        aY = aY - deltaAngle;
    case 'uparrow'
        aX = aX + deltaAngle;
    case 'downarrow'
        aX = aX - deltaAngle;
    otherwise  
end
     aX = min(3.7, max(-3.7, aX));
     aY = min(4.5, max(-3.5, aY));
%    Default Normal of the mems
    N = memsNorm(aX, aY);
    normX = [0 N(1)];
    normY = [0 N(2)];
    normZ = [0 N(3)];
    
    % Default reflected vector
    [Xmw, Ymw, Zmw] = findReflectedVector(Vlm, N, Zmw);
    laserX = [0 Xmw];
    laserY = [0 Ymw];
    laserZ = [0 Zmw];
    
    assignin('base', 'laserX', laserX)
    assignin('base', 'laserY', laserY)
    assignin('base', 'laserZ', laserZ)
    
    assignin('base', 'normX', normX)
    assignin('base', 'normY', normY)
    assignin('base', 'normZ', normZ)
    
    assignin('base', 'aX', aX)
    assignin('base', 'aY', aY)
    
    %Added for MEMS orientation in second plot option
    mems = surfaceMatrix(N, 5, 5, [0 0 0]);
    memsMatX = mems.X;
    memsMatY = mems.Y;
    memsMatZ = mems.Z;
    
    assignin('base', 'memsMatX', memsMatX)
    assignin('base', 'memsMatY', memsMatY)
    assignin('base', 'memsMatZ', memsMatZ)
    
    refreshdata
    drawnow limitrate
    
    
end

