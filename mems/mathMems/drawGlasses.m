function drawGlasses(head)
% Draws the glasses, the laser diode and, depending on the value of Head,
% adds a circle(1) or cylinder(2) or does nothing(0)
% 
%    % Draw Right side of glasses
%    w = null([0 1 0]); % Find two orthonormal vectors which are orthogonal to N
%    [P,Q] = meshgrid(-20:20,-75:75); % Provide a gridwork (you choose the size)
%    Xr = 0 + w(1,1)*P+w(1,2)*Q; % Compute the corresponding cartesian coordinates
%    Yr = 10 + w(2,1)*P+w(2,2)*Q; %   using the two vectors in w
%    Zr = -35 + w(3,1)*P+w(3,2)*Q;
%    surf(Xr,Yr,Zr,'FaceColor','green','EdgeColor','none')
%    
%    % Draw Left side of glasses
%    w = null([0 1 0]); % Find two orthonormal vectors which are orthogonal to N
%    [P,Q] = meshgrid(-20:20,-75:75); % Provide a gridwork (you choose the size)
%    Xl = 0 + w(1,1)*P+w(1,2)*Q; % Compute the corresponding cartesian coordinates
%    Yl = -160 + w(2,1)*P+w(2,2)*Q; %   using the two vectors in w
%    Zl = -35 + w(3,1)*P+w(3,2)*Q;
%    surf(Xl,Yl,Zl,'FaceColor','blue','EdgeColor','none')
%    
%    % Draw Front part of glasses
%    w = null([0 0 1]); % Find two orthonormal vectors which are orthogonal to N
%    [P,Q] = meshgrid(-85:85,-20:20); % Provide a gridwork (you choose the size)
%    Xf = 0 + w(1,1)*P+w(1,2)*Q; % Compute the corresponding cartesian coordinates
%    Yf = -75 + w(2,1)*P+w(2,2)*Q; %   using the two vectors in w
%    Zf = 40 + w(3,1)*P+w(3,2)*Q;
%    surf(Xf,Yf,Zf,'FaceColor','magenta','EdgeColor','none')
%    
%    % Draw Laser diode
%    w = null([0 0 1]); % Find two orthonormal vectors which are orthogonal to N
%    [P,Q] = meshgrid(-7:7); % Provide a gridwork (you choose the size)
%    Xf = 0 + w(1,1)*P+w(1,2)*Q; % Compute the corresponding cartesian coordinates
%    Yf = 0 + w(2,1)*P+w(2,2)*Q; %   using the two vectors in w
%    Zf = 13.5 + w(3,1)*P+w(3,2)*Q;
%    surf(Xf,Yf,Zf,'FaceColor','red')
%    
%    % Draw Front hole
%    w = null([0 0 1]); % Find two orthonormal vectors which are orthogonal to N
%    [P,Q] = meshgrid(-15:15); % Provide a gridwork (you choose the size)
%    Xf = 0 + w(1,1)*P+w(1,2)*Q; % Compute the corresponding cartesian coordinates
%    Yf = -50 + w(2,1)*P+w(2,2)*Q; %   using the two vectors in w
%    Zf = 40 + w(3,1)*P+w(3,2)*Q;
%    surf(Xf,Yf,Zf,'FaceColor','red')
   
   % Glasses coordinates
   LeftSide = [20 10 45;
               20 10 -110;
              -20 10 -110;
              -20 10 45];
   fill3(LeftSide(:,1)', LeftSide(:,2)', LeftSide(:,3)', 'blue');
   
   RightSide = [20 -160 -110;
                20 -160 45;
               -20 -160 45;
               -20 -160 -110];
   fill3(RightSide(:,1)', RightSide(:,2)', RightSide(:,3)', 'green');
   
   FrontSide = [20 -160 45;
                20 10 45;
               -20 10 45;
               -20 -160 45];
   fill3(FrontSide(:,1)', FrontSide(:,2)', FrontSide(:,3)', 'magenta');
   
   FrontHole = [7.5 -56.5 45;
                7.5 -31.5 45;
               -7.5 -31.5 45;
               -7.5 -56.5 45];
   fill3(FrontHole(:,1)', FrontHole(:,2)', FrontHole(:,3)', 'red');        
   
   LaserDiode = [7 -7 18.5;
                7 7 18.5;
               -7 7 18.5;
               -7 -7 18.5];
   fill3(LaserDiode(:,1)', LaserDiode(:,2)', LaserDiode(:,3)', 'red'); 
   
   
   %Draw head approximation used in calculation
   switch head
       case 1 %Circle added
           th = linspace(0,2*pi,100);
           normalVec = [1 0 0];
           xyz = [0 -70 -20];
           R = 68;
           P = null(normalVec);
           circ = bsxfun(@plus,xyz',R*P*[cos(th); sin(th)]);
           plot3(circ(1,:),circ(2,:),circ(3,:));
           quiver3(xyz(1),xyz(2),xyz(3), normalVec(1),normalVec(2),normalVec(3),'r');
       case 2 %Cylinder added
           [y, z, x] = cylinder(71, 40);
           surf((x*160-80),(y-70),(z-20));
       otherwise %Nothing added     
   end
   
end