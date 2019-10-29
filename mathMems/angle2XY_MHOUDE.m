function [x,y] = angle2XY_MHOUDE(aX, aY, z, d, memsAngle)
    %syms k x y
    N = memsNorm(aX, aY, memsAngle);
   [x,y ,z ] = findReflectedVector(d,N,z);
end

