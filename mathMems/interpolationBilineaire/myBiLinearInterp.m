function value = myBiLinearInterp(point,corners, cornersValues)
matCorners = cell2mat(corners);
x = point(1);
y = point(2);
x1 = min(matCorners(1:2:end));
x2 = max(matCorners(1:2:end));
y1 = min(matCorners(2:2:end));
y2 = max(matCorners(2:2:end));

value = 1/((x2-x1)*(y2-y1)) * [x2-x, x-x1] * [cornersValues(1), cornersValues(2) ; ...
                                              cornersValues(3), cornersValues(4)] ... 
                                              * [y2-y;y-y1];
end

