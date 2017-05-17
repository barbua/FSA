#ifndef LOSS_FUNCTIONS_H
#define LOSS_FUNCTIONS_H
#include <vector>
#include <string>

#include "DataSource.h"


class HingeLoss{
public:
	HingeLoss(){}
	static float Value(float x, float y){float u=x*y; return (u<=1 ? 1-u : 0);}
	static float Derivative(float x, float y){return (x*y<=1 ? -y : 0);}
	static bool useObsWeights(){return false;}
	static bool isBinary(){return true;}
	static std::string name(){return std::string("Hinge");}
};

/* ++++++ HuberLoss ++++++ 
   This structures stores the definition of the Huberised SVM loss and its 
   derivative 
*/
class HuberSVMLoss{
public:
	HuberSVMLoss(){}
	static float Value(float x, float y){float h=0.1f;float u=x*y; return (u<=1-h ? 1-u : (u<=1+h ? pow(1+h-u,2.0f)/(4*h) : 0));}
	static float Derivative(float x, float y){float h=0.1f;float u=x*y; return y*(u<=1-h ? -1 : (u<=1+h ? -(1+h-u)/(2*h) : 0));}
	static bool useObsWeights(){return false;}
	static bool isBinary(){return true;}
	static std::string name(){return std::string("HuberSVM");}
};
/* ++++++ LogisticLoss ++++++ 
   This structures stores the definition of the Logistic loss as well as 
   the definition of the derivative of the Logistic loss. 
*/
class LogisticLoss{
public:
	LogisticLoss(){}
	static float Value(float x, float y){float u=x*y; return (u>0 ? log(1+exp(-u)) : log(1+exp(u))-u);}
	static float Derivative(float x, float y){return -y/(1+exp(x*y));}
	static bool useObsWeights(){return true;}
	static bool isBinary(){return true;}
	static std::string name(){return std::string("Logistic");}
};

/* ++++++ LorenzLoss ++++++ 
   This structures stores the definition of the Lorenz loss and its 
   derivative 
*/
class LorenzLoss{
public:
	LorenzLoss(){}
	static float Value(float x, float y){float u=x*y; return (u<1 ? log(1+pow(u-1,2.0f)) : 0);}
	static float Derivative(float x, float y){float u=x*y;return (u<1 ? 2*y*(u-1)/(1+pow(u-1,2.0f)) : 0);}
	static bool useObsWeights(){return false;}
	static bool isBinary(){return true;}
	static std::string name(){return std::string("Lorenz");}
};

class SquareLoss{
public:
	SquareLoss(){}
	static float Value(float x, float y){return (x-y)*(x-y);}
	static float Derivative(float x, float y){return 2*(x-y);}
	static bool useObsWeights(){return false;}
	static bool isBinary(){return false;}
	static std::string name(){return std::string("L2");}
};

template<class Loss>
class VapnikLoss{
public:
	VapnikLoss(){}
	static float Value(std::vector<float> &x, int y){
		int n=(int)x.size();float sum=0;
		for (int i=0;i<n;i++)
			sum+=(i!=y?Loss::Value(x[y]-x[i],1.f):0);
		return sum;
	}
	static void Derivative(std::vector<float> &dx, std::vector<float> &x, int y){
		int n=(int)x.size();
		dx.assign(n,0);
		for (int i=0;i<n;i++)
			if (i!=y){
				dx[i]=-Loss::Derivative(x[y]-x[i],1.f);
				dx[y]+=-dx[i];
			}}
	static bool useObsWeights(){return Loss::useObsWeights();}
	static bool isBinary(){return false;}
	static std::string name(){return std::string("Vapnik")+Loss::name();}
};

template<class LossFunction, class ClfType, class DataType>
float SumLoss(ClfType &clf, DataType &data){
	typename ClfType::DataType sum=0,c,wp,wn,lb;
	int i,y,n=data.nObs();

	if (LossFunction::useObsWeights()){
		int np=data.nObs(1);
		int nn=n-np;
		wp=(typename ClfType::DataType)0.5/np;
		wn=(typename ClfType::DataType)0.5/nn;
	}else
		wp=wn=1;

	data.SetFirstObs();
	for (i=0;i<n;i++){
		typename DataType::Container *x=data.GetObs(y);
		c=clf.Response(*x);
		lb=LossFunction::Value(c,(float)y);
		sum+=(y>0 ? wp : wn)*lb;
		data.SetNextObs();
	}
	return sum;
}

template<class LossFunction, class Tp, class TpY>
float SumLoss(std::vector<Tp> &c, std::vector<TpY> &y){
	Tp sump=0,sumn=0,np=0,nn=0,lb;
	int i,n=(int)c.size();

	for (i=0;i<n;i++){
		lb=LossFunction::Value(c[i],(float)y[i]);
		if (y[i]>0){
			sump+=lb;
			np++;
		}
		else{
			sumn+=lb;
			nn++;
		}
	}
	if (LossFunction::useObsWeights()){
		Tp wp=(Tp)0.5/np;
		Tp wn=(Tp)0.5/nn;
		return sump*wp+sumn*wn;
	}else
		return sump+sumn;
}

template<class LossFunction, class Tp>
float SumVapnikLoss(std::vector<std::vector<Tp>> &c, std::vector<int> &y){
	Tp sum=0;
	int n=(int)c.size();
	for (int i=0;i<n;i++){
		sum+=VapnikLoss<LossFunction>::Value(c[i],y[i]);
	}
	return sum;
}

//template<class LossFunction, class Tp>
//float SumLoss(std::vector<Tp> &cy){
//	Tp sum=0;
//	int n=(int)cy.size();
//
//	for (int i=0;i<n;i++){
//		sum+=LossFunction::Value(cy[i]);
//	}
//	return sum;
//}

template<class Tp, class TpY>
float DetRate(std::vector<Tp> &c, std::vector<TpY> &y, double thr){
	Tp sum=0,sum1=0;
	int n=(int)c.size();
	for (int i=0;i<n;i++)
		if (y[i]>0){
			sum+=(c[i]>=thr?1:0);
			sum1++;
		}
	return sum/sum1;
}

template<class Tp, class TpY>
float FpRate(std::vector<Tp> &c, std::vector<TpY> &y, double thr){
	Tp sum=0,sum1=0;
	int n=(int)c.size();
	for (int i=0;i<n;i++)
		if (y[i]<=0){
			sum+=(c[i]>=thr?1:0);
			sum1++;
		}
	return sum/sum1;
}
template<class Tp>
float MisClfError(std::vector<Tp> &cy){
	Tp sum=0;
	int n=(int)cy.size();

	for (int i=0;i<n;i++){
		sum+=(cy[i]<0 ? 1 : 0);
	}
	return sum/n;
}

template<class Tp>
float MisClfErrorMC(std::vector<std::vector<Tp>> &c, const std::vector<int> &y){
	Tp sum=0;
	int py,n=(int)y.size();
	for (int i=0;i<n;i++){
		py=argmax(c[i]);
		sum+=(py!=y[i]);
	}
	return sum/n;
}

template<class Tp, class TpY>
float MisClfError(std::vector<Tp> &c, std::vector<TpY> &y){
	Tp sum=0,cy;
	int i,n=(int)y.size();

	for (i=0;i<n;i++){
		cy=(y[i]>0 ? c[i] : -c[i]);
		sum+=(cy<0 ? 1 : 0);
	}
	return sum/n;
}

template<class Tp, class TpY>
float MisClfError(std::vector<Tp> &c, const std::vector<TpY> &y){
	Tp sum=0,cy;
	int i,n=(int)y.size();

	for (i=0;i<n;i++){
		cy=(y[i]>0 ? c[i] : -c[i]);
		sum+=(cy<0 ? 1 : 0);
	}
	return sum/n;
}

template<class Tp>
int NumSV(std::vector<Tp> &cy, Tp h){
	Tp hr=1+h;
	int sum=0,n=(int)cy.size();

	for (int i=0;i<n;i++){
		sum+=(cy[i]<=hr? 1: 0);
	}
	return sum;
}

template<class Tp, class TpY>
int NumSV(std::vector<Tp> &c, std::vector<TpY> &y, Tp h){
	Tp cy,hr=1+h;
	int sum=0,i,n=(int)y.size();

	for (i=0;i<n;i++){
		cy=(y[i]>0 ? c[i] : -c[i]);
		sum+=(cy<=hr? 1: 0);
	}
	return sum;
}

template<class ClfType,class DataSource>
void GetPredictions(std::vector<float> &out, ClfType &clf, DataSource &data, int nObs){
	typename DataSource::Y_type y;
	out.assign(nObs,0);
	data.SetFirstObs();
	for (int n=0;n<nObs;n++){
		typename DataSource::Container *x=data.GetObs(y);
		out[n]=clf.Response(*x);
		data.SetNextObs();
	}
}

template<class ClfType,class DataSource>
void GetPredictions(std::vector<float> &out,std::vector<int> &y, ClfType &clf, DataSource &data, int nObs){
	out.assign(nObs,0);
	y.resize(nObs);
	data.SetFirstObs();
	for (int n=0;n<nObs;n++){
		typename DataSource::Container *x=data.GetObs(y[n]);
		out[n]=clf.Response(*x);
		data.SetNextObs();
	}
}

template<class ClfType, class DataType>
float DetRate(ClfType &clf, DataType &data, float thr){
	typename ClfType::DataType sum=0,c;
	int i,n=(int)data.nObs(),y,np=0;

	data.SetFirstObs();
	for (i=0;i<n;i++){
		typename DataType::Container *x=data.GetObs(y);
		if (y>0){
			c=clf.Response(*x);
			sum+=(c>=thr? 1:0);
			np++;
		}
		data.SetNextObs();
	}
	return sum/np;
}

template<class ClfType, class DataType, class LossFunction>
float DetFpRate(float &fpRate, ClfType &clf, DataType &data, LossFunction &fn, float thr){
	float sumdet=0,sumfp=0,npos=0, nneg=0,c;
	int i,n=(int)data.nObs(),y;

	data.SetFirstObs();
	for (i=0;i<n;i++){
		typename DataType::Container *x=data.GetObs(y);
		c=clf.Response(*x);
		if (y>0){
			npos++;
			sumdet+=(c>thr);
		}
		else{
			nneg++;
			sumfp+=(c>thr);
		}
		data.SetNextObs();
	}
	fpRate=sumfp/nneg;
	return sumdet/npos;
}

template<class ClfType, class DataType>
float MisClfError(ClfType &clf, DataType &data, int nObs){
	typedef typename ClfType::DataType Tp;
	Tp sum=0,c,cy;
	int i,y;

	data.SetFirstObs();
	for (i=0;i<nObs;i++){
		typename DataType::Container *x=data.GetObs(y);
		c=clf.Response(*x);
		cy=(y>0 ? c : -c);
		sum+=(cy<0 ? 1 : 0);
		data.SetNextObs();
	}
	return sum/nObs;
}


template<class Tp, class bin_container>
Tp MSE(std::vector<Tp> &beta, DataSource<bin_container,Tp> &data){
	Tp sum=0;
	Tp c,dif;
	int i,n=(int)data.nObs();
	
	for (i=0;i<n;i++){
		bin_container *x=data.GetObs(dif,i);
		c=DotProduct(beta,*x);
		dif-=c;
		sum+=dif*dif;
	}
	return sum/n;
}

template<class ClfType, class Tp, class bin_container>
Tp MSE(ClfType &clf, DataSource<bin_container,Tp> &data){
	Tp sum=0;
	Tp c,dif;
	int i,n=(int)data.nObs();
	
	for (i=0;i<n;i++){
		bin_container *x=data.GetObs(dif,i);
		c=clf.Response(*x);
		dif-=c;
		sum+=dif*dif;
	}
	return sum/n;
}

template<class ClfType, class Tp, class bin_container>
std::vector<Tp> MSE(ClfType &clf, DataSource<bin_container,std::vector<Tp> > &data){
	std::vector<Tp> y,dif,sumdd;
	int i,n=(int)data.nObs(),nd;	
	for (i=0;i<n;i++){
		bin_container *x=data.GetObs(y,i);
		clf.Response(dif,*x);
		dif-=y;
		if (i==0){
			sumdd.assign(dif.size(),(Tp)0);
		}
		dif*=dif;
		sumdd+=dif;
	}
	nd=(int)dif.size();
	sumdd/=(Tp)n;
	return sumdd;
}

template<class ClfType, class DataType, class Tp>
int NumberSV(ClfType &clf, DataType &data, Tp h){
	Tp yxb,hr=1+h;
	int i,n=(int)data.nObs(),y,nsv=0;
	data.SetFirstObs();
	for (i=0;i<n;i++){
		Tp r=clf.Response(*data.GetObs(y));
		yxb=r*(y>0 ? 1 : -1);
		nsv+=(yxb<=hr? 1: 0);
		data.SetNextObs();
	}
	return nsv;
}

template<class ClfType, class DataType, class Tp>
int NumberSVU(ClfType &clf, DataType &data, Tp h){
	Tp yxb,hr=1+h;
	int i,n=(int)data.nObs(),y,nsv=0;
	std::vector<Tp> x;
	data.SetFirstObs();
	for (i=0;i<n;i++){
		Bin2Obs(x,*data.GetObs(y),data._bins);
		yxb=clf.Response(x)*(y>0 ? 1 : -1);
		nsv+=(yxb<=hr? 1: 0);
		data.SetNextObs();
	}
	return nsv;
}

template<class ClfType, class DataType, class Tp>
int NumberSV(ClfType &clf, DataType &data, Tp h, int y){
	Tp yxb,hr=1+h;
	int i,n=(int)data.nObs(),y1,nsv=0;
	for (i=0;i<n;i++){
		Tp r=clf.Response(*data.GetObs(y1,i));
		if (y1!=y)
			continue;
		yxb=r*(y>0 ? 1 : -1);
		nsv+=(yxb<=hr? 1: 0);
	}
	return nsv;
}

template<class ClfType, class DataType, class Tp>
float ParDetFpRate(float &fpRate, ClfType &clf, DataType &data, std::vector<Tp> &params, float thr){
	float sumdet=0,sumfp=0,npos=0, nneg=0,c;
	int i,n=(int)data.nObs(),y;

	data.SetFirstObs();
	for (i=0;i<n;i++){
		typename DataType::Container *x=data.GetObs(y);
		c=clf.Response(*x,params[i]);
		if (y>0){
			npos++;
			sumdet+=(c>thr);
		}
		else{
			nneg++;
			sumfp+=(c>thr);
		}
		data.SetNextObs();
	}
	fpRate=sumfp/nneg;
	return sumdet/npos;
}

template<class LossFunction,class ClfType, class DataType, class Tp>
float SumParamLoss(ClfType &clf, DataType &data, std::vector<Tp> &params){
	float sum=0,r,c,wp,wn,lb;
	int i,n=(int)data.nObs(),y;

	if (LossFunction::useObsWeights()){
		int np=data.nObs(1);
		int nn=n-np;
		wp=(typename ClfType::DataType)0.5/np;
		wn=(typename ClfType::DataType)0.5/nn;
	}else
		wp=wn=1;

	data.SetFirstObs();
	for (i=0;i<n;i++){
		typename DataType::Container *x=data.GetObs(y);
		c=clf.Response(*x,params[i]);
		r=(y>0 ? c : -c);//*(Tp)0.5;
		lb=LossFunction::Value(r,(float)y);
		sum+=(y>0 ? wp : wn)*lb;
		data.SetNextObs();
	}
	return sum;
}

template<class DataType, class LossFunction, class Tp>
float SumParamTrueLoss(std::vector<std::pair <int,std::vector<Tp>>> weights, DataType &data, LossFunction &fn, bool beforeSel){
	Tp sum=0,r,c,wp,wn,lb;
	int i,n=(int)data.nObs(),y;
	int numDays = (int)weights[0].second.size();
	int numObsPerDay = (int)n/numDays;

	if (fn.useObsWeights){
		int np=data.nObs(1);
		int nn=n-np;
		wp=(Tp)0.5/np;
		wn=(Tp)0.5/nn;
	}else
		wp=wn=1;

	data.SetFirstObs();
	for (i=0;i<n;i++){
		typename DataType::Container *x=data.GetObs(y);
		c=ResponseForTrueWeights(*x,weights,(int)i/numObsPerDay,beforeSel);
		r=(y>0 ? c : -c);
		lb=fn(r);
		sum+=(y>0 ? wp : wn)*lb;
		data.SetNextObs();
	}
	return sum;
}

#endif

