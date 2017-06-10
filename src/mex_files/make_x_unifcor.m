function x=make_x_unifcor(N,M,cor)
% make N observations (rows) of dimension M uniformly correlated with
% correlation cor
ro=sqrt(cor/(1-cor));
x=randn(N,M);
z=randn(N,1)*ro;
x=x+repmat(z,1,M);
