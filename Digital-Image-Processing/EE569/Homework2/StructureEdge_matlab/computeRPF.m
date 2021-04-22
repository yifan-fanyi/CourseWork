function [R,P,F] = computeRPF(cntR,sumR,cntP,sumP)
% compute precision, recall and F measure given cnts and sums
R=cntR./max(eps,sumR); P=cntP./max(eps,sumP); F=2*P.*R./max(eps,P+R);
end

