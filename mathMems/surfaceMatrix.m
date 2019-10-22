classdef surfaceMatrix
    %surface contains and compute surfaces
    %   X,Y and Z properties are 3 matrices for the surf function to plot
    %   correctly
    
    properties
        X
        Y
        Z
    end
    
    methods
        function obj = surfaceMatrix(N, width, height, Origin)
           %UNTITLED6 Construct an instance of this class
           %   Detailed explanation goes here
           w = null(N); % Find two orthonormal vectors which are orthogonal to N
           [P,Q] = meshgrid(-width:width, -height:height); % Provide a gridwork (you choose the size)
           obj.X = Origin(1) + w(1,1)*P+w(1,2)*Q; % Compute the corresponding cartesian coordinates
           obj.Y = Origin(2) + w(2,1)*P+w(2,2)*Q; %   using the two vectors in w
           obj.Z = Origin(3) + w(3,1)*P+w(3,2)*Q;
        end
        function obj = computeSurface(N, width, height, Origin)
           w = null(N); % Find two orthonormal vectors which are orthogonal to N
           [P,Q] = meshgrid(-width:width, -height:height); % Provide a gridwork (you choose the size)
           obj.X = Origin(1) + w(1,1)*P+w(1,2)*Q; % Compute the corresponding cartesian coordinates
           obj.Y = Origin(2) + w(2,1)*P+w(2,2)*Q; %   using the two vectors in w
           obj.Z = Origin(3) + w(3,1)*P+w(3,2)*Q;
        end
    end
end

