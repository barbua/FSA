#ifndef _VECTOR_UTIL_H
#define _VECTOR_UTIL_H
#pragma warning(disable:4786)
#pragma warning(disable:4996)

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <functional>
#include <algorithm>
#include <numeric>
#include <memory>
#include <deque>

template<class Tp>
void Unique(std::vector<Tp> &out, std::vector<Tp> &in){
	out=in;
	std::sort(out.begin(),out.end());
	typename std::vector<Tp>::iterator la=std::unique(out.begin(),out.end());
	out.erase(la,out.end());
}

template<class Tp>
std::vector<Tp> Seq(Tp begin, Tp end, Tp step){
	// the equivalent of seq in R
	int i,n=(int)abs((end-begin)/step);
	std::vector<Tp> v;
	v.resize(n);
	for (i=0;i<n;i++)
		v[i]=begin+i*step;
	return v;
};

template<class Tp>
void CumSum (std::vector<Tp> &out,std::vector<Tp> &v){
	// cummulative sum of v
	size_t i,n=v.size();
	out.resize(n);
	if (n==0)
		return;
	out[0]=v[0];
	for (i=1;i<n;i++)
		out[i]=out[i-1]+v[i];
}

template<class Tp>
inline bool operator ==(std::vector<Tp> &v1,std::vector<Tp> &v2){//true if v1==v2
	size_t i,n=v1.size();
	if (v2.size()!=n)
		return false;
	for (i=0;i<n;i++)
		if (v1[i]!=v2[i])
			return false;
	return true;
}

template<class Tp, class Tp2>
inline void AppendToVector(std::vector<Tp> &dest,std::vector<Tp2> &src){
	size_t i,n=src.size(),n0=dest.size();
	dest.resize(n+n0);
	for (i=0;i<n;i++)
		dest[i+n0]=(Tp)src[i];
}

template<class Tp>
void CopyColumns(std::vector<std::vector<Tp>> &out, std::vector<std::vector<Tp>> &in, std::vector<int> &idx){
	//Copy the columns indexed by idx as rows in out
	int r=(int)in.size(),c=(int)in[0].size(),n=(int)idx.size();
	out.resize(n);
	for (int i=0;i<n;i++){
		int ii=idx[i];
		out[i].resize(r);
		std::vector<Tp> &o=out[i];
		for (int j=0;j<r;j++)
			o[j]=in[j][ii];
	}
}

template<class Tp, class Tp2>
void CopyVector(std::vector<Tp> &to, Tp2 &v2){
	typename Tp2::iterator vi,ve=v2.end();
	int i;
	to.resize(v2.size());
	i=0;
	for (vi=v2.begin();vi!=ve;++vi,++i)
		to[i]=(Tp)(*vi);
}

template<class Tp, class Tp2>
void CopyData(std::vector<Tp> &to, Tp2 *v2, int n){
	Tp2 *ptr=v2;
	to.resize(n);
	for(int i=0;i<n;i++){
		to[i]=(Tp)(*ptr);
		++ptr;
	}
}

template<class Tp, class Tp2>
void CopyData(std::vector<std::vector<Tp> > &to, Tp2 *v2, int nrows, int ncols){
	Tp2 *ptr=v2;
	to.resize(nrows);
	for(int i=0;i<nrows;i++){
		std::vector<Tp> &ti=to[i];
		ti.resize(ncols);
		for (int j=0;j<ncols;j++){
			ti[j]=(Tp)(*ptr);
			++ptr;
		}
	}
}

template<class Tp, class Tp2>
void CopyDataT(std::vector<std::vector<Tp> > &to, Tp2 *v2, int nrows, int ncols){
	Tp2 *ptr=v2;
	to.resize(nrows);
	for(int i=0;i<nrows;i++)
		to[i].resize(ncols);
	for (int j=0;j<ncols;j++){
		for(int i=0;i<nrows;i++){
			to[i][j]=(Tp)(*ptr);
			++ptr;
		}
	}
}

template<class Tp>
std::ostream & operator<<( std::ostream &os, const std::vector<Tp> &v ) {
	unsigned long i, sz = v.size();
	for ( i = 0; i < sz; ++i ) {
		if ( i != 0 ) os << '\t';
		os << v[i];
	}
	return os;
}

template<class Tp>
std::istream & operator>>( std::istream &is, std::vector<Tp> &v ) {
	unsigned long i, sz = v.size();
	for ( i = 0; i < sz; ++i ) is >> v[i];
	return is;
}

template<class Tp>
std::vector<int> operator ==(std::vector<Tp> &x, const Tp &b) { 
	size_t i,n=x.size();
	std::vector<int> out(n);
    for (i = 0; i < n; i++) 
        out[i]=(x[i]==b); 
	return out;
} 

#define DEFOP(OP) \
	template<class Tp>\
    void operator OP (std::vector<Tp> &x, const Tp &b) { \
		size_t i,n=x.size();\
        for (i = 0; i < n; i++) \
            x[i] OP b; \
    } 
DEFOP(+=);
DEFOP(-=);
DEFOP(*=);
DEFOP(/=);
#undef DEFOP

#define DEFOP(OP) \
	template<class Tp, class Tp2>\
    void operator OP (std::vector<std::vector<Tp> > &x, const Tp2 &b) { \
		size_t i,j,n=x.size(),nj;\
        for (i = 0; i < n; i++){nj=x[i].size(); \
		for (j = 0; j < nj; j++)\
			x[i][j] OP b; }\
    } 
DEFOP(+=);
DEFOP(-=);
DEFOP(*=);
DEFOP(/=);
#undef DEFOP

#define DEFOP(OP) \
	template<class Tp, class Tp2>\
    void operator OP (std::vector<Tp> &x, const Tp2 &b) { \
		size_t i,n=x.size();\
        for (i = 0; i < n; i++) \
            x[i] OP b[i]; \
    } 
DEFOP(+=);
DEFOP(-=);
DEFOP(*=);
DEFOP(/=);
#undef DEFOP

#define DEFOP(OP) \
	template<class Tp>\
    void operator OP (std::vector<std::vector<Tp> > &x, const std::vector<std::vector<Tp>> &b) { \
		size_t i,j,n=x.size(),nj;\
        for (i = 0; i < n; i++){nj=x[i].size(); \
		for (j = 0; j < nj; j++)\
			x[i][j] OP b[i][j]; }\
    } 
DEFOP(+=);
DEFOP(-=);
DEFOP(*=);
DEFOP(/=);
#undef DEFOP


//template<class Tp>
//void operator+=(std::vector<Tp> &x, std::vector<Tp> &y){
//	int i,n=(int)x.size();
//	for (i=0;i<n;i++)
//		x[i]+=y[i];
//};

template<class Tp>
void ProdElementwise (std::vector<Tp> &v1,std::vector<Tp> &v2){
	// multiply v1 by v2 elementwise and store result in v1
	size_t i,n=v1.size();
	for (i=0;i<n;i++)
		v1[i]*=v2[i];
}

template<class Tp>
std::vector<Tp> operator *(std::vector<Tp> &x, Tp y){
	int i,n=(int)x.size();
	std::vector<Tp> v;
	v.resize(n);
	for (i=0;i<n;i++)
		v[i]=x[i]*y;
	return v;
};

template<class Tp>
std::vector<Tp> operator +(std::vector<Tp> &x, Tp y){
	int i,n=(int)x.size();
	std::vector<Tp> v;
	v.resize(n);
	for (i=0;i<n;i++)
		v[i]=x[i]+y;
	return v;
};

template<class Tp>
std::vector<Tp> operator +(std::vector<Tp> &x, std::vector<Tp> &y){
	int i,n=(int)x.size();
	std::vector<Tp> v;
	v.resize(n);
	for (i=0;i<n;i++)
		v[i]=x[i]+y[i];
	return v;
};

template<class Tp>
void Add(std::vector<Tp>& x, const std::vector<Tp>& y) {
	if(x.size() < y.size()) {
		x.insert(x.end(), y.size() - x.size(), 0);
	}

	for(int i = 0; i < (int)x.size(); i++)
		x[i] += y[i];
}

template<class Tp>
Tp DotProduct(std::vector<Tp> &x, Tp *y){
	int i,n=(int)x.size();
	Tp sum=0;
	for (i=0;i<n;i++)
		sum+=x[i]*y[i];
	return sum;
};

template<class Tp>
Tp DotProduct(const std::vector<Tp> &x, const std::vector<Tp> &y){
	int i,n=(int)x.size();
	Tp sum=0;
	for (i=0;i<n;i++)
		sum+=x[i]*y[i];
	return sum;
};

template<class Tp>
Tp DotProduct(const std::vector<Tp> &x, const std::vector<Tp> &y, const std::vector<int> &idxy){
	// assumes y is sparse and only idxy are nonzero, represented by y
	int i,n=(int)y.size();
	Tp sum=0;
	for (i=0;i<n;i++)
		sum+=x[idxy[i]]*y[i];
	return sum;
};

template<class Tp>
void DotProduct(std::vector<Tp> &out, const std::vector<std::vector<Tp> > &x, const std::vector<Tp> &y){
	int i,n=(int)x.size();
	out.resize(n);
	for (i=0;i<n;i++)
		out[i]=DotProduct(x[i],y);
};

template<class Tp>
Tp SumSq(const std::vector<Tp> &x){
	size_t i,n=x.size();
	Tp sum=0;
	for (i=0;i<n;i++)
		sum+=x[i]*x[i];
	return sum;
};

template<class Tp>
Tp WtSumSq(const std::vector<Tp> &x, const std::vector<Tp> &wt){
	size_t i,n=x.size();
	Tp sum=0;
	for (i=0;i<n;i++)
		sum+=x[i]*x[i]*wt[i];
	return sum;
};

template<class Tp>
void Set(std::vector<Tp> &x, Tp val){ x.assign(x.size(),val);};

template<class Tp>
void Set(std::vector<std::vector<Tp>> &x, Tp val){
	size_t i,n=x.size();
	for (i=0;i<n;i++) x[i].assign(x[i].size(),val);
}

template<class Tp>
void Set(std::vector<std::vector<std::vector<Tp> > > &x, Tp val){
	size_t i,n=x.size();
	for (i=0;i<n;i++) Set(x[i],val);
}

template<class Tp>
Tp Sum(const std::vector<Tp> &x){return  accumulate(x.begin(), x.end(), (Tp)0);}


template<class Tp>
Tp SumAbs(const std::vector<Tp> &x){
	size_t i,n=x.size();
	Tp sum=0,d;
	for (i=0;i<n;i++){
		d=x[i];
		sum+=fabs(d);
	}
	return sum;
};

template<class Tp>
Tp SumAbs(const std::vector<Tp> &x1, const Tp *x2){
	size_t i,n=x1.size();
	Tp sum=0,d;
	for (i=0;i<n;i++){
		d=x1[i]-x2[i];
		sum+=fabs(d);
	}
	return sum;
};

template<class Tp>
Tp SumSq(const std::vector<Tp> &x1, const Tp *x2){
	size_t i,n=x1.size();
	Tp sum=0,d;
	for (i=0;i<n;i++){
		d=x1[i]-x2[i];
		sum+=d*d;
	}
	return sum;
};

template<class Tp>
Tp SumSq(const std::vector<Tp> &x1, const std::vector<Tp> &x2){
	size_t i,n=x1.size();
	Tp sum=0,d;
	for (i=0;i<n;i++){
		d=x1[i]-x2[i];
		sum+=d*d;
	}
	return sum;
};

template <class Tp>
Tp SumSqDiff(std::vector<Tp> &x){
	//  sum of (x_i-x_i-1)^2
	Tp sum=0;
	int i,n=(int)x.size();
	for (i=1; i<n; ++i)
		sum+= (x[i]-x[i-1])*(x[i]-x[i-1]);
	return sum;
}

template<class Tp>
Tp L1Norm(const Tp *x, size_t n){
	size_t i;
	Tp sum=0;
	for (i=0;i<n;i++)
		sum+=fabs(x[i]);
	return sum;
};

template<class Tp>
Tp L1Norm(const std::vector<Tp> &x){
	size_t i,n=x.size();
	Tp sum=0;
	for (i=0;i<n;i++)
		sum+=fabs(x[i]);
	return sum;
};

template<class Tp>
double NormL2(const std::vector<Tp> &x){
	size_t i,n=x.size();
	Tp sum=0;
	for (i=0;i<n;i++)
		sum+=x[i]*x[i];
	return sqrt((double)sum);
};

template<class Tp>
Tp LinfNorm(const std::vector<Tp> &x1){
	size_t i,n=x1.size();
	Tp max=0,d;
	for (i=0;i<n;i++){
		d=fabs(x1[i]);
		if (d>max)
			max=d;
	}
	return max;
};

template<class Tp>
double L2Dist(const std::vector<Tp>& a, const std::vector<Tp>& b) {
	assert(a.size() == b.size() && a.size() > 0);

	Tp sum = 0;
	for(int i = 0; i < (int)a.size(); i++) {
		sum += (a[i] - b[i]) * (a[i] - b[i]);
	}

	return sqrt(double(sum));
}

template<class Tp>
Tp L1Norm(const std::vector<Tp> &x1, const std::vector<Tp> &x2){
	size_t i,n=x1.size();
	Tp sum=0,d;
	for (i=0;i<n;i++){
		d=x1[i]-x2[i];
		sum+=fabs(d);
	}
	return sum;
};

template<class Tp>
double NormL1(std::vector<Tp> &x){
	int i,n=(int)x.size();
	double sum=0;
	for (i=0;i<n;i++)
		sum+=fabs((double) x[i]);
	return sum;
};

template<class Tp>
Tp LinfNorm(const std::vector<Tp> &x1, const std::vector<Tp> &x2){
	size_t i,n=x1.size();
	Tp max=0,d;
	for (i=0;i<n;i++){
		d=fabs(x1[i]-x2[i]);
		if (d>max)
			max=d;
	}
	return max;
};

template<class Tp>
double Mean(const std::vector<Tp> &x){
	int n=(int)x.size();
	if (n==0)
		return 0;
	else
		return ((double)Sum(x))/n;
}

template<class Tp>
double Var(const std::vector<Tp> &vec){
	// return sigma^2
	size_t i,n=vec.size();
	if (n<2)
		return 0;
	double mean=Mean(vec),sum=0,x;
	for (i=0;i<n;i++){
		x=(double) vec[i]-mean;
		sum+=x*x;
	}
	return sum/(n-1);
}

template<class Tp>
double Var(double &mean, std::vector<Tp> &vec){
	// return sigma^2
	size_t i,n=vec.size();
	if (n<2)
		return 0;
	double sum=0,x;
	mean=Mean(vec);
	for (i=0;i<n;i++){
		x=(double) vec[i]-mean;
		sum+=x*x;
	}
	return sum/(n-1);
}

template<class Tp>
void Normalize(std::vector<Tp> &d){
	int i,n=(int)d.size();
	double sum=0;
	for (i=0;i<n;i++)
		sum+=(double)(d[i]*d[i]);
	sum=sqrt(sum);
	if (sum!=0){
		for (i=0;i<n;i++)
			d[i]/=(Tp)sum;
	}
}

template<class Tp>
double Median(std::vector<Tp> &v){
	if (v.empty())
		return 0;
	int n=(int)v.size();
	if (n<2)
		return v[0];
	std::vector<Tp> v1=v;
	std::sort(v1.begin(),v1.end());
	if (n%2==0)
		return ((double)v1[n/2]+(double)v1[n/2-1])/2.;
	return (double)v1[(n-1)/2];
}

template<class Tp>
Tp Percentile(std::vector<Tp> &v, double perc){
	// Percentile with perc in the range 0-100
	if (v.empty())
		return 0;
	int n=(int)v.size();
	if (n<2)
		return v[0];
	std::vector<Tp> v1=v;
	std::sort(v1.begin(),v1.end());
	int i=(int)(n*perc*0.01);
	return v1[i];
}

template<class Tp>
int argmax(const Tp  *vec, int n){
	if (n==0)
		return -1;
	int i, maxat=0;
	Tp maxv=vec[0];
	for (i=1;i<n;i++){
		if (vec[i]>maxv){
			maxv=vec[i];
			maxat=i;
		}
	}
	return maxat;
}

template<class Tp>
int argmax(const std::vector <Tp> &vec){
	return argmax(&vec[0],(int)vec.size());
}

template<class Tp>
int FindMax(Tp &max, std::vector<Tp> &vec){
	size_t i,n=vec.size();
	if (n==0)
		return -1;
	size_t maxat=0;
	max=vec[0];
	for (i=1;i<n;i++){
		if (vec[i]>max){
			max=vec[i];
			maxat=i;
		}
	}
	return (int) maxat;
}

template<class Tp>
int FindMin(Tp &min, std::vector<Tp> &vec){
	size_t i,n=vec.size();
	if (n==0)
		return -1;
	int minat=0;
	min=vec[0];
	for (i=1;i<n;i++){
		if (vec[i]<min){
			min=vec[i];
			minat=i;
		}
	}
	return minat;
}

template<class Tp>
Tp GetMin(const Tp *vec, const size_t n){
	size_t i;
	Tp d,min;
	if (n==0)
		return 0;
	min=vec[0];
	for (i=1;i<n;i++){
		d=vec[i];
		if (d<min)
			min=d;
	}
	return min;
}

template<class Tp>
Tp GetMax(const Tp *vec, const size_t n){
	size_t i;
	Tp d,max;
	if (n==0)
		return 0;
	max=vec[0];
	for (i=1;i<n;i++){
		d=vec[i];
		if (d>max)
			max=d;
	}
	return max;
}

template<class Tp,class Tp2, typename Operator>
double MinValue(Tp &vec, Tp2 &pt, Operator &op){
	typename Tp::iterator i,ve=vec.end();
	double d,minv;
	i=vec.begin(); 
	if (i==ve)
		return pow(10.,10.);
	minv=op(*i,pt);
	for(;i!=ve;++i){
		d=op(*i,pt);
		if(d<minv)
			minv=d;
	}
	return minv;
}

template<class Tp,class Tp2, typename Operator>
double MaxValue(int &imax, Tp &vec, Tp2 &pt, Operator &op){
	int i,n=(int)vec.size();
	double d,maxv;
	i=0;imax=-1;
	if (i>=n)
		return -pow(10.,10.);
	imax=0;
	maxv=op(vec[i],pt);
	for(;i<n;++i){
		d=op(vec[i],pt);
		if(d>maxv){
			maxv=d;
			imax=i;
		}
	}
	return maxv;
}

template<class Tp,class Tp2, typename Operator>
double MaxValue(Tp &vec, Tp2 &pt, Operator &op){
	typename Tp::iterator i,ve=vec.end();
	double d,maxv;
	i=vec.begin();
	if (i==ve)
		return -pow(10.,10.);
	maxv=op(*i,pt);
	for(;i!=ve;++i){
		d=op(*i,pt);
		if(d>maxv)
			maxv=d;
	}
	return maxv;
}

template<class Tp>
Tp GetMax(const std::vector<Tp> &vec){return GetMax(&vec[0],vec.size());}

template<class Tp>
Tp GetMin(const std::vector<Tp> &vec){return GetMin(&vec[0],vec.size());}

template<class Tp>
void GetMinMax(Tp &min, Tp &max, Tp *vec, size_t n){
	size_t i;
	Tp d;
	if (n==0)
		return;
	min=max=vec[0];
	for (i=1;i<n;i++){
		d=vec[i];
		if (d<min)
			min=d;
		if (d>max)
			max=d;
	}
}

template<class Tp>
void GetMinMax(Tp &min, Tp &max, std::vector<Tp> &vec){
	return GetMinMax(min,max,&vec[0],vec.size());
}

template<class Tp>
int FindElement(std::vector<Tp> &vec, Tp &element){
	size_t i,n=vec.size();
	for (i=0;i<n;i++){
		if (vec[i]==element)
			return (int)i;
	}
	return -1;
}
 
template<class Tp>
int FindElement(Tp *vec, size_t size, Tp &element){
	size_t i;
	for (i=0;i<size;i++){
		if (vec[i]==element)
			return (int)i;
	}
	return -1;
}

template<class Tp1,class Tp2>
int FindKey(std::vector<std::pair<Tp1,Tp2> > &L, Tp1 val){
	//finds val in vector L, return index if found, else returns -1
	int i,n=(int)L.size();
	for (i=0;i<n;++i)
		if (L[i].first==val) 
			return i;
	return -1;
}

template<class Tp>
int CountOccurences(const std::vector<Tp> &vec, Tp element){
	size_t i,n=vec.size();
	int sum=0;
	for (i=0;i<n;i++){
		if (vec[i]==element)
			sum++;
	}
	return sum;
}

template<class Tp>
int CountNonZeroWithoutFirst(std::vector<Tp> &v){
	int j,n=(int)v.size(),sum=0;
	for (j=1;j<n;j++){
		if (v[j]!=0)
			sum++;
	}
	return sum;
}

template<class Tp>
int CountNonZero(std::vector<Tp> &v){
	int j,n=(int)v.size(),sum=0;
	for (j=0;j<n;j++){
		if (v[j]!=0)
			sum++;
	}
	return sum;
}

template<class Tp>
void FindGreater(std::vector<int> &out,std::vector<Tp> &in, Tp thr){
	int i,n=(int)in.size();
	out.clear();
	out.reserve(n);
	for (i = 0; i <n; ++i) {
		if (in[i]>thr)
			out.push_back(i);
	}
}

template<class Tp>
void FindLess(std::vector<int> &out,std::vector<Tp> &in, Tp thr){
	int i,n=(int)in.size();
	out.clear();
	out.reserve(n);
	for (i = 0; i <n; ++i) {
		if (in[i]<thr)
			out.push_back(i);
	}
}

template<class Tp>
void GetNonZeroPositions(std::vector<int> &out,std::vector<Tp> &in){
	int i,n=(int)in.size();
	out.clear();
	for (i = 0; i <n; ++i) {
		if (in[i]!=0)
			out.push_back(i);
	}
}

template<class Tp>
void GetNonZeroWithoutFirstPositions(std::vector<int> &out,std::vector<Tp> &in){
	int i,n=(int)in.size();
	out.clear();
	for (i = 1; i <n; ++i) {
		if (in[i]!=0)
			out.push_back(i-1);
	}
}


template<class Tp>
void GetColumn(std::vector<Tp> &out, std::vector<std::vector<Tp>> &in, int c){
	// Get in[i][c] for all i
	int n=(int)in.size();
	out.resize(n);
	for (int i=0;i<n;i++)
		out[i]=in[i][c];
}

template<class Tp>
void SetColumn(std::vector<std::vector<Tp>> &x, std::vector<Tp> &val,  int c){
	// Set x[i][c]=val[i] for all i
	int n=(int)val.size();
	for (int i=0;i<n;i++)
		x[i][c]=val[i];
}

template<class Tp>
void DeleteFromVector(std::vector<Tp> &v, int idx){
	int i,n=(int)v.size();
	if (idx<0||idx>=n)
		return;
	for (i=idx;i<n-1;i++)
		v[i]=v[i+1];
	v.pop_back();
}

template<class Tp>
void HardThresholding(std::vector<Tp> &v, Tp thr){
	size_t i,n=v.size();
	for (i=0;i<n;++i)
		if (abs(v[i])<thr)
			v[i]=0;
}

template<class Tp>
inline void CleanZerosFromEnd(std::vector<Tp> &v, int nmin){ 
	// delete whatever is empty at the end of v
	// keep at least nmin+1 elements
	int n=(int)v.size();
	while(n>nmin&&v[n-1]==0){
		v.pop_back();
		n--;
	}
}

template<class Tp>
void KeepOnly(std::vector<Tp> &vec, std::vector<int> &selected){
	// delete from vec everything else except selected.
	std::vector<Tp>	tmp;
	int i,n=(int)selected.size();
	tmp.resize(n);
	for (i=0;i<n;i++){
		tmp[i]=vec[selected[i]];
	}
	vec=tmp;
}

template<class Tp>
void KeepInPlace(std::vector<Tp> &vec, std::vector<int> &selected){
	// delete from vec everything else except selected.
	// does not need extra memory
	// assumes selected is increasing
	int i,n=(int)selected.size();
	for (i=0;i<n;i++){
		vec[i]=vec[selected[i]];
	}
	vec.resize(n);
}

template<class Tp>
bool RemoveAtPosition(std::vector<Tp> &x, int pos){
	int i,n=(int)x.size(),posr=pos;
	if (n<1)
		return false;
	if (posr<0)
		posr=n-1;
	for (i=posr+1;i<n;i++)
		x[i-1]=x[i];
	x.pop_back();
	return true;
}

template<class Tp>
void CopySelected(std::vector<Tp> &out, Tp*in, std::vector<int> &selected, bool resize=true){
	// copy from in entries from selected.
	int i,n=(int)selected.size();
	if (resize)
		out.resize(n);
	for (i=0;i<n;i++){
		out[i]=in[selected[i]];
	}
}

template<class Tp>
void Reverse( std::vector<Tp> &out, std::vector<Tp> &in){
	int i,n=(int)in.size();
	out.resize(n);
	for (i=0;i<n;i++)
		out[i]=in[n-1-i];
}

template<class Tp>
void Transpose(std::vector<std::vector<Tp> > &out, std::vector<std::vector<Tp> > &in){
	int i,j,no=(int)in.size(),nv=(int)in[0].size();
	std::vector<Tp> r(no);
	out.assign(nv,r);
	for (i=0;i<no;i++)
		for (j=0;j<nv;j++)
			out[j][i]=in[i][j];
}

template<class Tp>
void TransposeAndClear(std::vector<std::vector<Tp>> &out, std::vector<std::vector<Tp> > &in){
	int i,j,nr=(int)in.size(),nc=(int)in[0].size();
	int nshrink=nc/10;
	out.resize(nc);
	for (j=nc-1;j>=0;j--){
		out[j].resize(nr);
		for (i=0;i<nr;i++){
			out[j][i]=in[i][j];
			in[i].pop_back();
		}
		if (j%nshrink==0)
			for (i=0;i<nr;i++){
				in[i].shrink_to_fit();
			}
	}
}

template<class Tp>
void TransposeAndClear(std::vector<std::deque<Tp>> &out, std::vector<std::vector<Tp> > &in){
	int i,j,nr=(int)in.size(),nc=(int)in[0].size();
	out.resize(nc);
	for (i=nr-1;i>=0;i--){
		for (j=0;j<nc;j++){
			out[j].push_front(in[i][j]);
		}
		in.pop_back();
		in.shrink_to_fit();
	}
}

template<class Tp>
void Shuffle(std::vector<Tp> &v, std::vector<int> &idx){
	// shuffle the vector v based on the permutation idx
	Tp tmp;
	int i,n=(int)idx.size();
	for (i=0;i<n;i++){
		tmp=v[i];
		v[i]=v[idx[i]];
		v[idx[i]]=tmp;
	}
}

template<class Tp>
void Rescale(std::vector<Tp> &in, float scale){
	int i,n=(int)in.size();
	for (i=0;i<n;i++){
		in[i].Rescale(scale);
	}
}

template<class Tp>
void Rescale(std::vector<Tp> &out,std::vector<Tp> &in, float scale){
	int i,n=(int)in.size();
	out=in;
	for (i=0;i<n;i++){
		out[i].Rescale(scale);
	}
}


template<class Tp>
double angular_dist(const std::vector<Tp>&a, const std::vector<Tp>& b, const int exponent) {
	assert(a.size() == b.size() && a.size() > 0);

	double v1 = DotProduct(a, b);
	double v2 = DotProduct(a, a);
	double v3 = DotProduct(b, b);

	if( v2 == 0 || v3 == 0)
		return -1000;
	else {
		return pow(fabs(v1 / sqrt(v2*v3)) , exponent);
	}
}

#endif
