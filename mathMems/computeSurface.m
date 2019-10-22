function [X, Y, Z] = computeSurface(N, width, height, Origin)
   w = null(N); % Find two orthonormal vectors which are orthogonal to N
   [P,Q] = meshgrid(-width:width, -height:height); % Provide a gridwork (you choose the size)
   X = Origin(1) + w(1,1)*P+w(1,2)*Q; % Compute the corresponding cartesian coordinates
   Y = Origin(2) + w(2,1)*P+w(2,2)*Q; %   using the two vectors in w
   Z = Origin(3) + w(3,1)*P+w(3,2)*Q;
end