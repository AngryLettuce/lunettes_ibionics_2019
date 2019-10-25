function iterations = calcArraySize(maxAngles,delta)
    maxAngles = abs(maxAngles);
    iterations = ceil(((maxAngles(1) + maxAngles(2))/delta +1) * ((maxAngles(3) + maxAngles(4))/delta +1)) ;
end

