function pixMat = genPixMat(wallcorners, resolution)

    %add one to the resolution to get a pixMat with the correct dimensions
    %we need for exemple 401 line to get 400 intervals.
    resolution = resolution  + 1;
    pixMat = zeros(max(resolution-1),3);
    
    maxX = max(wallcorners(:,1));
    minX = min(wallcorners(:,1));
    maxY = max(wallcorners(:,2));
    minY = min(wallcorners(:,2));
    
    %width is in y as in the reference frame 
    width  = linspace(minX, maxX, resolution(1));
    height = linspace(maxY, minY, resolution(2));
    
    grid = meshgrid(width, height);
    
    %get delta between each val
    deltaWidth = abs(width(2) - width(1));
    deltaHeigth = abs(height(2) - height(1));
    
    
    width = width(2:end) - (deltaWidth/2);
    height = height(2:end) + (deltaHeigth/2);
    
    pixMat(1:length(width),1) = width;
    pixMat(1:length(height),2) = height;
    %Z is always the same
    pixMat(1:length(width),3) = wallcorners(1,3);
        
end

