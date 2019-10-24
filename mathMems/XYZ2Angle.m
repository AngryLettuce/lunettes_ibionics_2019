function [solx, soly] = XYZ2Angle(x,y,z,d, memsAngle)

    syms theta_x theta_y k
    alpha = deg2rad(memsAngle)-theta_x + deg2rad(90);
    beta = -theta_y;
    zn = sin(alpha)*cos(beta);
    xn = cos(alpha)*cos(beta);
    yn = sin(beta);
    N = [xn,yn,zn];
    n = N/norm(N);
    
    % reflexion
    r = d-2*dot(d,n)*n;
    eq1 = z == k*r(3);
    eq2 = x == k*r(1);
    eq3 = y == k*r(2);
    [solx,soly,solk] = vpasolve([eq1,eq2,eq3],[theta_x,theta_y,k]);
    solx = rad2deg(double(solx));
    soly = rad2deg(double(soly));
    
end