function [array] = genXYZ(Vlm, maxAngles, Z, delta)
array = single(zeros(calcArraySize(maxAngles, delta), 5));
i = 1;
for aX = maxAngles(1) : delta : maxAngles(2) 
    for aY = maxAngles(3) : delta : maxAngles(4) 
        
        %Default Normal of the mems
        N = memsNorm(aX, aY);
        
        % Default reflected vector
        [X, Y, Z] = findReflectedVector(Vlm, N, Z);
        
        array(i, :) = single([X, Y, Z, aX, aY]);
        i = i + 1;
    end
end

%save(strcat('XYZ', '_', num2str(Z), '_', num2str(delta), '.mat'), 'array', '-v7.3')

end

