function posSequence = angleSequence2Pos(sequence)

    %% constant % Laser's position relative to MEMS
    Xlm = 0;
    Ylm = 0;
    Zlm = 18.5;

    Vlm = [Xlm Ylm Zlm];

    Zmw = 1000;

    posSequence = zeros(length(sequence), 2);
    %Default Normal of the mems
    for i = [1:length(sequence)]

        N = memsNorm(sequence(i, 1), sequence(i, 2));

        % Default reflected vector
        [Xmw, Ymw, Zmw] = findReflectedVector(Vlm, N, Zmw);

        posSequence(i, :) = [Xmw ;  Ymw];

    end
end
