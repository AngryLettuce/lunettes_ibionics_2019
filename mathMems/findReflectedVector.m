function [X, Y, Z] = findReflectedVector(V, N, Zdist)
% V = V./(norm(V));
% d = V - 2 * dot(V,N) * N;
% X = d(1);
% Y = d(2);
% Z = d(3);
% Takes 2 vectors and makes the first reflect on the perpenticular surface
% of the second. Returns reflected vector scaled according to given wall 
% distance
    lenC = dot(V, N, 2) ./ sum(N .* N, 2);
    Vpar = lenC .* N;
%     Vpar = bsxfun(@times, lenC, N);

    Vper = V-Vpar;
    Vref = Vpar - Vper;
% 
    fact = Zdist/Vref(3); %find scale to apply to vector for wall distance
    Vref = fact*Vref;

    X = Vref(1);
    Y = Vref(2);
    Z = Vref(3);

end