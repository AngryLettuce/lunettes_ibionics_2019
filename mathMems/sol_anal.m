%% Pas bon en 3d
syms theta_x theta_y beta alpha r
alpha = 2*(deg2rad(21)-theta_x) + deg2rad(90);
beta = -2*theta_y;
eq1 = 1000 == r*sin(alpha)*cos(beta);
eq2 = -840.4375 == r*cos(alpha)*cos(beta);
eq3 = -85.8099 == r*sin(beta);
[solr, soltheta_x, soltheta_y] = solve([eq1,eq2,eq3],[r, theta_x, theta_y])

sol = rad2deg(eval(soltheta_x))
%% 
clear all
close all 
clc
syms k x y
theta_x = deg2rad(1);
theta_y = deg2rad(1);
d = [0,0,-1];
alpha = deg2rad(21)-theta_x + deg2rad(90);
beta = -theta_y;
z = sin(alpha)*cos(beta);
x = cos(alpha)*cos(beta);
y = sin(beta);
N = [x,y,z];
n = N/norm(N)
% reflexion
r = d-2*dot(d,n)*n
z = 1000;
k = z/r(3);
x = k*r(1)
y = k*r(2)

%% solution inverse
clear all
close all 
clc
syms theta_x theta_y k
d = [0,0,-1];
alpha = deg2rad(21)-theta_x + deg2rad(90);
beta = -theta_y;
z = sin(alpha)*cos(beta);
x = cos(alpha)*cos(beta);
y = sin(beta);
N = [x,y,z];
n = N/norm(N);
% reflexion
r = d-2*dot(d,n)*n;
z = 1000;
x = -900;
y = 15;
eq1 = z == k*r(3);
eq2 = x == k*r(1);
eq3 = y == k*r(2);
[solx,soly,solk] = solve([eq1,eq2,eq3],[theta_x,theta_y,k]);
solx = rad2deg(double(solx))
soly = rad2deg(double(soly))
%% Correction offset miroir
clear all
close all 
clc
syms offset_x offset_y k
theta_y = 0;
theta_x = 0;
d = [0,0,-1];
alpha = deg2rad(21)-theta_x + deg2rad(90) +offset_x;
beta = -theta_y + offset_y;
z = sin(alpha)*cos(beta);
x = cos(alpha)*cos(beta);
y = sin(beta);
N = [x,y,z];
n = N/norm(N);
% reflexion
r = d-2*dot(d,n)*n;
z = 1000;
x = -900;
y = 0;
eq1 = z == k*r(3);
eq2 = x == k*r(1);
eq3 = y == k*r(2);
[solx,soly,solk] = solve([eq1,eq2,eq3],[offset_x,offset_y,k]);
solx = rad2deg(double(solx))
soly = rad2deg(double(soly))

%% Correction vecteur incident + correction offset miroir
clear all
close all 
clc
syms offset_x offset_y  x y z k k2 k3
theta_y = 0;
theta_x = 0;
d = [x,y,z];
alpha = deg2rad(21)-theta_x + deg2rad(90) +offset_x;
beta = -theta_y + offset_y;
zn = sin(alpha)*cos(beta);
xn = cos(alpha)*cos(beta);
yn = sin(beta);
N = [xn,yn,zn];
n = N/norm(N);
% reflexion
r = d-2*dot(d,n)*n;
z1 = 1000;
x1 = -900.4040;
y1 = 0;
eq1 = z1 == k*r(3);
eq2 = x1 == k*r(1);
eq3 = y1 == k*r(2);
% deuxieme point
theta_y = 1;
theta_x = 0;
d = [x,y,z];
alpha = deg2rad(21)-theta_x + deg2rad(90) +offset_x;
beta = -theta_y + offset_y;
zn = sin(alpha)*cos(beta);
xn = cos(alpha)*cos(beta);
yn = sin(beta);
N = [xn,yn,zn];
n = N/norm(N);
% reflexion
r2 = d-2*dot(d,n)*n;
z2 = 1000;
x2 = -900.7733;
y2 = -43.8741;
eq4 = z2 == k2*r2(3);
eq5 = x2 == k2*r2(1);
eq6 = y2 == k2*r2(2);
% troisieme point
theta_y = 1;
theta_x = 1;
d = [x,y,z];
alpha = deg2rad(21)-theta_x + deg2rad(90) +offset_x;
beta = -theta_y + offset_y;
zn = sin(alpha)*cos(beta);
xn = cos(alpha)*cos(beta);
yn = sin(beta);
N = [xn,yn,zn];
n = N/norm(N);
% reflexion
r3 = d-2*dot(d,n)*n;
z3 = 1000;
x3 = -839.4335;
y3 = -42.8407;
eq7 = z3 == k3*r3(3);
eq8 = x3 == k3*r3(1);
eq9 = y3 == k3*r3(2);
% solve
[soloffsetx,soloffsety,solk,solk2,solk3,solx,soly,solz] = vpasolve([eq1,eq2,eq3,eq4,eq5,eq6,eq7,eq8],[offset_x,offset_y,k,k2,k3,x,y,z])
soloffsetx = rad2deg(double(soloffsetx))
soloffsety = rad2deg(double(soloffsety))

