function N = memsNorm(aX, aY )

% MEMS default position (22 degrees, Zn = 40)
Zn = 45;
Xn = -Zn/tand(180-90+aX);
Yn = -Zn/tand(180-90-22+aY);
N = [Xn Yn Zn];

end
