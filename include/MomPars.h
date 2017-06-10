#ifndef _MOM_PARS_H
#define _MOM_PARS_H
#pragma warning(disable:4786)
#pragma warning(disable:4996)

#include <vector>
#include "VectorUtil.h"

template<class Tp>
struct MomPars0d{
	Tp g0,b0inc;
	Tp  _mom;
	MomPars0d(Tp mom){_mom=mom;}
	void Update(Tp &b0){
		b0inc*=_mom;g0*=(1-_mom);
		b0inc+=g0;
		b0+=b0inc;
		g0=0;
	}
	void Init(){
		g0=0;
		b0inc=0;	
	}
};

template<class Tp>
struct MomPars1d{
	std::vector<Tp> grad,binc;
	Tp g0,b0inc;
	int _iter, _minibatchsize;
	Tp  _mom;
	MomPars1d(){_iter=0;_minibatchsize=0;_mom=0;}
	MomPars1d(int minibatchsize, Tp mom){_iter=0;_minibatchsize=minibatchsize;_mom=mom;}
	void Init(int minibatchsize, Tp mom){_iter=0;_minibatchsize=minibatchsize;_mom=mom;}
	bool Update(std::vector<Tp> &b){
		_iter++;
		if (_iter%_minibatchsize==0){
			binc*=_mom;
			grad*=(1-_mom);
			binc+=grad;
			b+=binc;
			Set(grad,(Tp)0);
			return true;
		}
		return false;
	}
	bool Update(std::vector<Tp> &b, Tp &b0){
		_iter++;
		if (_iter%_minibatchsize==0){
			binc*=_mom;b0inc*=_mom;
			grad*=(1-_mom);g0*=(1-_mom);
			binc+=grad;b0inc+=g0;
			b+=binc;b0+=b0inc;
			Set(grad,(Tp)0);g0=0;
			//if (_ridge>0)
			//	clf.MultiplyBy(1-_ridge*_eta);
			return true;
		}
		return false;
	}
	void Init(std::vector<Tp> &b){
		grad.resize(b.size());
		binc.resize(b.size());
		g0=0;
		b0inc=0;	
	}
	void KeepOnly(std::vector<int> &sel){
		::KeepOnly(grad,sel);
		::KeepOnly(binc,sel);
	}
};


template<class Tp>
struct MomPars2d{
	std::vector<std::vector<Tp>> grad,binc;
	Tp g0,b0inc;
	int _iter, _minibatchsize;
	Tp _mom;

	MomPars2d(){_iter=0;_minibatchsize=0;_mom=0;}
	MomPars2d(int minibatchsize, Tp mom){_iter=0;_minibatchsize=minibatchsize;_mom=mom;}
	void Init(int minibatchsize, Tp mom){_iter=0;_minibatchsize=minibatchsize;_mom=mom;}

	bool Update(std::vector<std::vector<Tp>> &b){
		_iter++;
		if (_iter%_minibatchsize==0){
			binc*=_mom;	grad*=(1-_mom);
			binc+=grad;	b+=binc;
			Set(grad,(Tp)0);
			return true;
		}
		return false;
	}

	bool Update(std::vector<std::vector<Tp>> &b, Tp &b0){
		_iter++;
		if (_iter%_minibatchsize==0){
			binc*=_mom;b0inc*=_mom;
			grad*=(1-_mom);g0*=(1-_mom);
			binc+=grad;b0inc+=g0;
			b+=binc;b0+=b0inc;
			Set(grad,(Tp)0);g0=0;
			return true;
		}
		return false;
	}

	void Init(std::vector<std::vector<Tp>> &b){
		grad.resize(b.size());
		binc.resize(b.size());
		g0=0;
		b0inc=0;
		for (int i=0;i<b.size();i++){
			grad[i].assign(b[i].size(),0);
			binc[i].assign(b[i].size(),0);
		}	
	}
	void KeepOnly(std::vector<int> &sel){
		::KeepOnly(grad,sel);
		::KeepOnly(binc,sel);
	}
	void KeepOnly(int i, std::vector<int> &sel){
		::KeepOnly(grad[i],sel);
		::KeepOnly(binc[i],sel);
	}
};

template<class Tp>
struct MomPars3d{
	std::vector<std::vector<std::vector<Tp>>> grad,binc;
	int _iter, _minibatchsize;
	Tp _mom;

	MomPars3d(int minibatchsize, Tp mom){_iter=0;_minibatchsize=minibatchsize;_mom=mom;}

	bool Update(std::vector<std::vector<Tp>> &b){
		_iter++;
		if (_iter%_minibatchsize==0){
			binc*=_mom;	grad*=(1-_mom);
			binc+=grad;	b+=binc;
			Set(grad,(Tp)0);
			return true;
		}
		return false;
	}

	void Init(std::vector<std::vector<Tp>> &b){
		grad.resize(b.size());
		binc.resize(b.size());
		for (int i=0;i<b.size();i++){
			grad[i].assign(b[i].size(),0);
			binc[i].assign(b[i].size(),0);
		}	
	}
	void KeepOnly(std::vector<int> &sel){
		::KeepOnly(grad,sel);
		::KeepOnly(binc,sel);
	}
};

template<class Tp, class Tp2>
void AddXIdx(std::vector<Tp> &grad, Tp &g0, std::vector<Tp2> &idx, Tp &coef) {
	int M=(int)idx.size();
	g0+=coef;
	for (int i=0;i<M;++i){
		grad[idx[i]]+=coef;
	}
}

template<class Tp>
void AddX(std::vector<Tp> &grad, std::vector<Tp> &x, Tp &coef) {
	int M=(int)x.size();
	for (int i=0;i<M;++i){
		grad[i]+=coef*x[i];
	}
}

template<class Tp>
void AddX(std::vector<Tp> &grad, std::vector<Tp> &x,std::vector<int> &sel, Tp &coef) {
	int M=(int)sel.size();
	for (int i=0;i<M;++i){
		grad[i]+=coef*x[sel[i]];
	}
}

template<class Tp>
Tp DotProduct(const std::vector<Tp> &x, const std::vector<Tp> &y, const std::map<int,int> &selidx){
	int n=(int)x.size();
	std::map<int,int>::const_iterator mi,me=selidx.end();
	Tp sum=0;
	for (int i=0;i<n;++i){
		mi=selidx.find(i);
		if (mi!=me){
			sum+=y[mi->second]*x[i];
		}
	}
	return sum;
};

template<class Tp>
void AddX(std::vector<Tp> &grad, const std::vector<Tp> &x, const std::map<int,int> &selidx, Tp &coef){
	int n=(int)x.size();
	std::map<int,int>::const_iterator mi,me=selidx.end();
	for (int i=0;i<n;++i){
		mi=selidx.find(i);
		if (mi!=me){
			grad[mi->second]+=coef*x[i]; 
		}
	}
}

template<class Tp>
void AddX(std::vector<std::vector<Tp>> &grad, std::vector<Tp> &x, std::vector<std::vector<int>> &sel, std::vector<Tp> &coef) {
	int nc=(int)coef.size();
	for (int j=0;j<nc;j++){
		std::vector<Tp> &gj=grad[j];
		std::vector<int> &sj=sel[j];
		int ns=(int)sj.size();
		for (int i=0;i<ns;++i)
			gj[i]+=coef[j]*x[sj[i]];
	}
}

template<class Tp, class TpX>
Tp ResponseIdx(std::vector<TpX> &idx, std::vector<std::vector<Tp>> &b){
	int n=(int)idx.size();
	Tp sum=0;
	for (int i=0;i<n;i++)
		sum+=b[i][idx[i]];
	return sum;
}

template<class Tp, class TpX>
Tp ResponseIdx(const std::vector<TpX> &idx, const std::vector<int> &sel, const std::vector<std::vector<Tp>> &b){
	int n=(int)sel.size();
	Tp sum=0;
	for (int i=0;i<n;i++)
		sum+=b[i][idx[sel[i]]];
	return sum;
}

template<class Tp, class TpX>
void AddXIdx(std::vector<std::vector<Tp>> &grad, std::vector<TpX> &idx, Tp &coef) {
	int n=(int)idx.size();
	for (int i=0;i<n;++i){
		grad[i][idx[i]]+=coef;
	}
}
template<class Tp, class TpX>
void AddXIdx(std::vector<std::vector<Tp>> &grad, std::vector<TpX> &idx, const std::vector<int> &sel, Tp &coef) {
	int n=(int)sel.size();
	for (int i=0;i<n;++i){
		grad[i][idx[sel[i]]]+=coef;
	}
}


#endif

