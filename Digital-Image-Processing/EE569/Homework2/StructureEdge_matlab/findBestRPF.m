
function [bstR,bstP,bstF,bstT] = findBestRPF(T,R,P)
% linearly interpolate to find best thr for optimizing F
if(numel(T)==1), bstT=T; bstR=R; bstP=P;
  bstF=2*P.*R./max(eps,P+R); return; end
A=linspace(0,1,100); B=1-A; bstF=-1;
for j = 2:numel(T)
  Rj=R(j).*A+R(j-1).*B; Pj=P(j).*A+P(j-1).*B; Tj=T(j).*A+T(j-1).*B;
  Fj=2.*Pj.*Rj./max(eps,Pj+Rj); [f,k]=max(Fj);
  if(f>bstF), bstT=Tj(k); bstR=Rj(k); bstP=Pj(k); bstF=f; end
end
end