#ifndef FSA_H
#define FSA_H
#include <vector>
#include <map>
#include <ctime>
#include <string>
#include <cstdarg>
#include "VectorUtil.h"
#include "FsaSchedule.h"
#include "MomPars.h"
#include "LossFunctions.h"
#include "DataMemory.h"

char fnGlobalLogfile[261] = "printf.txt";

/*
void printOut(const char * format, ...){
	FILE *pfile;
	pfile = fopen(fnGlobalLogfile, "at");
	if (pfile != NULL){
		va_list arguments;
		va_start(arguments, format);
		vfprintf(pfile,format, arguments);
		va_end(arguments);
		fclose(pfile);
	}
}
*/

template<class LossFunction, class Tp, class TpVecX>
float TrainLinFsa(std::vector<Tp> &b, Tp &b0, std::vector<int> &selected, DataSource<TpVecX, float> &data, int N,
	int kStar, Tp percExp, int nExp, Tp mu, int nEpochs, Tp eta, int minibatchsize, float mom, Tp ridge, int verbosity, int nEpNoSel = 10, int useSelected = 0) {
	// Generic linear FSA
	// N: Number of observations
	int M = data.nVar(), M0 = M;
	// npos,nneg: Store the number of positive and negative examples
	int npos = data.nObs(1.), nneg = N - npos;
	// etapos,etaneg: Weights for the positive and negative examples
	Tp etapos, etaneg, loss;
	if (LossFunction::useObsWeights()) {
		etapos = N*eta / (2 * npos);
		etaneg = N*eta / (2 * nneg);
	}
	else
		etapos = etaneg = eta;
	int nsv;
	float eta1 = minibatchsize*eta, err;
	std::vector<Tp> xb(N), sumSq;
	std::vector<float> y(N);
	MomPars1d<float> pars(minibatchsize, mom);
	std::map<int, int> selidx;

	time_t timeStart, timeEnd;
	time(&timeStart);
	if (verbosity>0) {
		printf("\nTraining FSA %s, npos=%d,nneg=%d,nVar=%d,k=%d,nEpochs=%d, eta=%g,mom=%g, par=%g\n", LossFunction::name().c_str(), npos, nneg, M, kStar, nEpochs, eta, mom, ridge);
		printf("Training starts at %s", ctime(&timeStart));
		//PrintOut("\nTraining %s, npos=%d,nneg=%d,nVar=%d,k=%d,nEpochs=%d, eta=%g,mom=%g, par=%g\n", LossFunction::name().c_str(), npos, nneg, M, kStar, nEpochs, eta, mom, ridge);
		//PrintOut("Training starts at %s", ctime(&timeStart));
	}
	b0 = 0;
	if (selected.empty()) {
		selected.resize(M);
		for (int i = 0; i<M; i++)
			selected[i] = i;
	}
	M = (int)selected.size();
	if (useSelected) {
		for (int i = 0; i<M; i++)
			selidx.insert(std::map<int, int>::value_type(selected[i], i));
	}
	b.assign(M, 0);
	pars.Init(b);
	xb.assign(N, 0);
	for (int e = -nEpNoSel; e<nEpochs; e++) {
		if (verbosity>0)
			printf("%d: ", e);
		int e10 = e % 10;
		for (int n = 0; n<N; n++) {
			TpVecX &x = *data.GetObs(y[n], n);
			if (useSelected)
				xb[n] = DotProduct(x, b, selidx) + b0;
			else
				xb[n] = DotProduct(x, b) + b0;
			Tp c = LossFunction::Derivative(xb[n], y[n]);
			if (c != 0) {
				Tp ec = -c*(y[n]>0 ? etapos : etaneg);
				if (useSelected)
					AddX(pars.grad, x, selidx, ec);
				else
					AddX(pars.grad, x, ec);
				pars.g0 += ec;
			}
			if (pars.Update(b, b0))
				b *= (1 - ridge*eta1);
		}

		if (verbosity>0) {
			loss = SumLoss<LossFunction>(xb, y);
			nsv = NumSV(xb, y, .01f);
			err = MisClfError(xb, y) * 100;
		}
		M = (int)b.size();
		int ns = GetFsaScheduleE(e + 1, percExp, nExp, (int)(nEpochs*0.7), kStar, M0, mu);
		if (M>ns) {
			std::vector<int> lnz, tmp;
			sumSq.resize(M);
			for (int i = 0; i<M; i++)
				sumSq[i] = fabs(b[i]);
			if (ns <= kStar)
				GetFsaIndices(lnz, sumSq, ns, 1);
			else
				GetFsaIndices(lnz, sumSq, ns, 0);
			for (int i = 0; i<(int)lnz.size(); i++)
				tmp.push_back(selected[lnz[i]]);
			selected = tmp;
			KeepOnly(b, lnz);
			if (useSelected) {
				data.KeepOnly(selected);
				selidx.clear();
				for (int i = 0; i<(int)selected.size(); i++)
					selidx.insert(std::map<int, int>::value_type(selected[i], i));
			}
			else
				data.KeepOnly(lnz);
			pars.KeepOnly(lnz);
		}
		if (verbosity>0)
			printf("Loss=%1.3f, err=%1.2f, nsv=%d, %1.1f, ns=%d,%d\n", loss, err, nsv, nsv / (float)N*100.f, ns, (int)selected.size());
		//if (verbosity>1 || (verbosity == 1 && e % 10 == 0)) {
		//	PrintOut("%d\t%1.3f\t%1.2f\t%d\t%1.1f\t%d\n", e, loss, err, nsv, nsv / (float)N*100.f, ns);
		//}
	}
	loss = SumLoss<LossFunction>(xb, y);
#ifdef PRINT_LOSS
	detr = DetRate(yxb, data._y, 0);
	fpr = FpRate(yxb, data._y, 0);
	for (int i = 0; i<N; i++)
		yxb[i] *= (data._y[i]>0 ? 1 : -1);
	GetROC(det, fp, yxb, data._y, 1000);
	Log2File("c:/tmp/detfp.txt", det);
	Log2File("c:/tmp/detfp.txt", fp);
	double auc = AUC(det, fp);
	printf("Loss=%1.3f, det=%1.2f, fp=%1.3f, AUC=%1.3f\n", loss, detr * 100, fpr * 100, auc);
	PrintOut("\nLoss=%1.3f, det=%1.2f, fp=%1.3f, AUC=%1.3f\n", loss, detr * 100, fpr * 100, auc);
#endif
	time(&timeEnd);
	if (verbosity>0) {
		printf("Training ends at %s, duration %g\n", ctime(&timeEnd), difftime(timeEnd, timeStart));
		//PrintOut("Training ends at %s, duration %g\n", ctime(&timeEnd), difftime(timeEnd, timeStart));
	}
	return loss;
}


#endif

