function [aX, aY] = getAngle(X,Y,Z,table)

       xTol = 3;
       yTol = 5;
        
      %find all the index in tolerance of yTol
      idx = find(abs(table(:,2) - Y) < yTol);
      
      %keep only the index in the ytol
        table = table(idx, :);
      
      %find all the index in the tolerance of xTol
      idx = find(abs(table(:,1) - X) < xTol);
      
      %get the middle index
      middleIdx = round(length(idx) / 2);
      
      %keep the middle value of the all the values in the xTol and yTol
      aX = table(idx(middleIdx), 4);
      aY = table(idx(middleIdx), 5);


end

