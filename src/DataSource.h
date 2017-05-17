#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include <vector>
#include <set>
#include <cmath>
//#include "Matrix.h"
//#include "ScsUtil.h"

template<class Tp, class TpY>
class DataSource {
public:
	typedef Tp Container;
	typedef TpY Y_type;
	int _nVar; // number of variables

	DataSource(){_nVar=0;}
	DataSource(int nVar){_nVar=nVar;}
	virtual int nVar() const {return _nVar;}
	virtual int nObs() const =0;
	virtual int nObs(TpY label) const =0;
	virtual void SetFirstObs()=0;
	virtual bool SetNextObs()=0;
	virtual Tp *GetObs(TpY &y)=0;
	virtual Tp *GetObs(TpY &y, int i)=0;
	virtual void KeepOnly(std::vector<int> &vars, bool resize=false)=0;
};

inline float GetYLinear(std::vector<float> &x, int kStar){
	float sum=0;
	for (int m=0;m<kStar;m++)
		sum+=x[m*10]; // linear response
	return sum;
}

inline float GetYLinear(std::vector<float> &x, std::vector<float> &beta){
	float sum=0;
	int kStar=(int)beta.size();
	for (int m=0;m<kStar;m++)
		sum+=x[m*10]*beta[m]; // linear response
	return sum;
}

inline int GetYLine(std::vector<float> &x, int kStar){
	float sum=0;
	for (int m=0;m<kStar;m++)
		sum+=x[m*10]; // line
	return (sum<0);
}

inline int GetYLine(std::vector<float> &x, std::vector<float> &beta){
	float sum=0;
	int kStar=(int)beta.size();
	for (int m=0;m<kStar;m++)
		sum+=x[m*10]*beta[m]; // line
	return (sum<0);
}

inline int GetYLineNoFeatSlc(std::vector<float> &x, int kStar){
	float sum=0;
	for (int m=0;m<kStar;m++)
		sum+=x[m]; // line
	return (sum<0);
}

inline int GetYSphere(std::vector<float> &x, int kStar){
	float sum=0;
	for (int m=0;m<kStar;m++)
		sum+=pow(abs(x[m*10]),2.f); // sphere
	return (sum<kStar);
}

inline int GetYRing(std::vector<float> &x, int kStar){
	float sum=0;
	for (int m=0;m<kStar;m++)
		sum+=pow(abs(x[m*10]),2.f); // ring
	return (sum<kStar&&sum>=0.5f*kStar);
}

inline int GetYStar(std::vector<float> &x, int kStar){
	float sum=0;
	for (int m=0;m<kStar;m++)
		sum+=pow(abs(x[m*10]),0.4f); // star
	return (sum<0.85*kStar);
}

#endif

