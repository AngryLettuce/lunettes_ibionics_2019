classdef vectorPlot
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        X
        Y
        Z
    end
    
    methods
        function obj = vectorPlot(X, Y, Z)
            %UNTITLED Construct an instance of this class
            %   Detailed explanation goes here
            obj.X = [0 X];
            obj.Y = [0 Y];
            obj.Z = [0 Z];
        end
        
        function obj = changeVectorX(obj,X)
            %METHOD1 Summary of this method goes here
            %   Detailed explanation goes here
            obj.X = [0 X];
        end
        
        function obj = changeVectorY(obj,Y)
            %METHOD1 Summary of this method goes here
            %   Detailed explanation goes here
            obj.Y = [0 Y];
        end
        
        function obj = changeVectorZ(obj,Z)
            %METHOD1 Summary of this method goes here
            %   Detailed explanation goes here
            obj.Z = [0 Z];
        end
    end
end

