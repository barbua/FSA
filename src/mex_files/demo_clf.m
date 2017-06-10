% number of observations to be generated
N=1000;
% number of predictors to be used
p=1000;
% number of predictors to be selected
kstar=10;
% number of test iterations
nruns=100;
% initialize the sigma matrix (i.e. convariance matrix). Note that the
% non-zero elements are actually
eta=.01;
nEpochs=100;
mu=30;
momentum=0.9;
lossfn=0;
%loss: 0=Logistic, 1=Lorenz (default), 2=smooth SVM, 3=hinge, 4=L2
k=kstar;
btrue=zeros(p,1);
ks=10*(1:kstar);
btrue(ks)=1;
% 
a=[];
name=sprintf('fsa_%dx%d_%d.txt',N,p,kstar);

for r=1:nruns
    rng(r);
    % generate design matrix with correlated predictors
    x=make_x_unifcor(N,p,0.5);
    % generate design matrix with correlated predictors
    xt=make_x_unifcor(N,p,0.5);
    % generate responses with the correlated design matrices
    y=ones(1,N);
    y((x*btrue)<0)=-1;
    yt=ones(1,N);
    yt((xt*btrue)<0)=-1;
    % keep track of how long 'fsa_gd' takes
    tic;
    [b,b0,ib]=trainLinFSA(x,y,lossfn,kstar,0.001,eta,nEpochs,mu,momentum,32,0);
    % trainLinFSA(x,y,loss,nsel,ridge,[eta=1.,nEpochs=100,mu=100.,mom=0.9,minibatch=32,verbosity=0]))
    % loss: 0=Logistic, 1=Lorenz (default), 2=smooth SVM, 3=hinge, 4=L2
    tim=toc;
    res=length(intersect(ib,ks));
    xb=xt(:,ib)*b'+b0;
    py=ones(1,N);
    py(xb<0)=-1;
    err=sum(py~=yt)*1./N;
    s=[r,res,err,tim]
    a=[a;s];
    if (r==1)
        ma=a;
    else
        ma=mean(a);
    end
    fprintf('avg: var det rate=%1.1f%%, err=%1.2f%%, time=%1.2fs\n', ma(2)/kstar*100,ma(3)*100,ma(4));    
end
dlmwrite(name,a);