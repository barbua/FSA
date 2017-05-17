#include <math.h>
#include <stdlib.h>
#include "mex.h"
#include "fsa.h"
using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	if (nrhs < 2)
		mexErrMsgTxt("usage: [b,b0,sel,loss] = trainLinFSA(x,y,loss,nsel,ridge,[eta=1.,nEpochs=100,mu=100.,mom=0.9,minibatch=32,verbosity=0]))");

	if (!mxIsDouble(prhs[0]) || !mxIsDouble(prhs[1]))
		mexErrMsgTxt("Expected double precision arguments.");

	int n0=mxGetNumberOfDimensions(prhs[0]);
	int n1=mxGetNumberOfDimensions(prhs[1]);
	if ( n0!= 2 || n1 != 2)
		mexErrMsgTxt("First argument expected to be two dimensional.");

	int N = (int)mxGetM(prhs[0]);
	int p = (int)mxGetN(prhs[0]);
	
	if (nlhs > 4)
		mexErrMsgTxt("Expected four returns.");


	DataMemory<float,float> data;
	int nEpochs=100,minibatch=32,verb=0;
	float eta=1.f,mu=100,mom=0.9f;
	double *x = mxGetPr(prhs[0]);
	double *y = mxGetPr(prhs[1]);
	int loss = (int)mxGetScalar(prhs[2]);
	int kstar = (int)mxGetScalar(prhs[3]);
	float ridge = (float)mxGetScalar(prhs[4]);
	if (nrhs>5)
		eta = (float)mxGetScalar(prhs[5]);
	if (nrhs>6)
		nEpochs = (int)mxGetScalar(prhs[6]);
	if (nrhs>7)
		mu = (float)mxGetScalar(prhs[7]);
	if (nrhs>8)
		mom = (float)mxGetScalar(prhs[8]);
	if (nrhs>9)
		minibatch = (int)mxGetScalar(prhs[9]);
	if (nrhs>10)
		verb = (int)mxGetScalar(prhs[10]);

	if (verb>0)
		printf("\nTraining N=%d,p=%d,loss=%d,nsel=%d,ridge=%g,eta=%g,nEpochs=%d,mu=%g,mom=%g,batch=%d,verb=%d\n",
		N,p,loss,kstar,ridge,eta,nEpochs,mu,mom,minibatch,verb);

	CopyDataT(data._x,x,N,p);
	CopyData(data._y,y,N);
	vector<float> b;
	float b0,v;
	vector<int> sel;
	switch(loss){
	case 0:
		v=TrainLinFsa<LogisticLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	case 1:
		v=TrainLinFsa<LorenzLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	case 2:
		v=TrainLinFsa<HuberSVMLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	case 3:
		v=TrainLinFsa<HingeLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	case 4:
		v=TrainLinFsa<SquareLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	}

	int n=(int)b.size();
	plhs[0] = mxCreateDoubleMatrix(1, n, mxREAL);
	plhs[1] = mxCreateDoubleScalar(b0);
	plhs[2] = mxCreateNumericMatrix(1, n, mxINT32_CLASS, mxREAL);
	plhs[3] = mxCreateDoubleScalar(v);
	double *pb = mxGetPr(plhs[0]);
	int *psel = (int *)mxGetPr(plhs[2]);
	for (int i=0;i<n;i++)
		pb[i]=b[i];
	for (int i=0;i<n;i++)
		psel[i]=sel[i]+1;
	//	pix[3*i]=pts[i].x();
	//	pix[3*i+1]=pts[i].y();
	//	pix[3*i+2]=pts[i].z();
	//}
}
