function [x,y] = angle2XY(aX, aY, z, d, memsAngles)
    %syms k x y
    
    theta_x = deg2rad(aX);
    theta_y = deg2rad(aY);
    alpha = deg2rad(memsAngles(2))- theta_y + deg2rad(90);
    beta = deg2rad(memsAngles(1)) + theta_x;
    zn = sin(alpha)*cos(beta);
    xn = cos(alpha)*cos(beta);
    yn = sin(beta);
    N = [xn,yn,zn];
    n = N/norm(N);
    
    % reflexion
    r = d-2*dot(d,n)*n;
    k = z/r(3);
    x = k*r(1);
    y = k*r(2);
end

