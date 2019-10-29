function N = memsNorm(aX, aY, memsAngle)

% r = 1;
% alpha = deg2rad(90 + memsAngle - aX);
% beta  = deg2rad(aY);
% 
% [x,y,z] = sph2cart(alpha,beta,r);
% N = [x,y,z];
% N = N ./ norm(N);
% MEMS default position (22 degrees, Zn = 40)
Zn = 47.65947;
Yn = -Zn/tand(180-90+aX);
Xn = -Zn/tand(180-90-memsAngle+aY);
N = [Xn Yn Zn];
N = N./norm(N);

end
