function N = memsNorm(aX, aY )
memsAngle = 21;
% MEMS default position (22 degrees, Zn = 40)
Zn = 47.65947;
Xn = -Zn/tand(180-90+aX);
Yn = -Zn/tand(180-90-memsAngle+aY);
N = [Xn Yn Zn];

end
