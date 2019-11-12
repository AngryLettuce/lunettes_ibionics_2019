function [corners, cornerIndexes] = findNearestCorners(gridDataX,gridDataY, point)
distances = zeros(length(gridDataX),length(gridDataY));
%calculate the distance between the point and the known points of the grid
for i = 1:length(gridDataX)
    for j = 1:length(gridDataY)
        distances(i,j) = pdist([gridDataX(i,j), gridDataY(i,j); point(1), point(2)]);
    end
end

smallestSum = inf;
for i = 1:length(gridDataX)-1
    for j = 1:length(gridDataY)-1
        squareSum = sum(sum(distances(i:i+1, j:j+1)));
        if squareSum < smallestSum
            smallestSum = squareSum;
            corners = [{[gridDataX(i,j),gridDataY(i,j)]}, {[gridDataX(i+1,j),gridDataY(i+1,j)]},...
                       {[gridDataX(i,j+1),gridDataY(i,j+1)]}, {[gridDataX(i+1,j+1),gridDataY(i+1,j+1)]}];
            cornerIndexes = [{[i,j]}, {[i+1,j]}, ...
                             {[i,j+1]}, {[i+1,j+1]}];
        end
    end
end


end

