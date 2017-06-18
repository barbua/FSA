#ifndef FSA_SCHEDULE_H
#define FSA_SCHEDULE_H
#include <algorithm>
#include <string>
#include <limits>

template<class Tp>
Tp GetPercFSA(int e, int maxEpoch, Tp mu){
	// get percent to 0 in maxEpoch steps
	Tp p=e/(Tp)maxEpoch;
	return (1-p)/(p*mu+1);
}

template<class Tp>
int GetFsaSchedule(int e, int maxEpoch, int kStar, int M, Tp mu){
	return std::max(kStar+(int)((M-kStar)*GetPercFSA(e,maxEpoch,mu)), kStar);
}

template<class Tp>
int GetFsaScheduleE(int e, Tp percExp, int nExp, int maxEpoch, int kStar, int M, Tp mu){
	if (e<=nExp)
		return (int)(kStar+M*pow(percExp,(Tp)e));
	M=kStar+(int)(M*pow(percExp,(Tp)nExp));
	return GetFsaSchedule(e-nExp,maxEpoch,kStar,M,mu);
}

template<class Tp>
bool GetFsaIndices(std::vector<float> &idx, const std::vector<Tp> &sumSq, int nKeep, int strict=0){
	int n=(int)sumSq.size();
	if (nKeep>=n)
		return false;
	std::vector<Tp> t=sumSq;
	std::sort(t.begin(),t.end(),std::greater<Tp>());
	Tp thr=(t[nKeep]+t[nKeep-1])/2;//printf("t%g ",thr);
	idx.clear();
	if (strict){
		for (int i=0;i<n;++i) {
			if (sumSq[i]>thr)
				idx.push_back(i);
		}	
		for (int i=0;i<n;++i) {
			if (sumSq[i]==thr&&idx.size()<nKeep)
				idx.push_back(static_cast<float>(i));
		}
		std::sort(idx.begin(),idx.end(),std::less<int>());	
	}
	else{
		for (int i=0;i<n;++i) {
			if (sumSq[i]>=thr)
				idx.push_back(i);
		}
	}
	return true;
}

template<class Tp>
class Schedule{
public:
	Schedule<Tp>(){_M=_kStar=_nEpochs=0;_mu=0;_name="Good";}
	Schedule<Tp>(int M, int kstar,int nEpochs,Tp mu){_M=M;_kStar=kstar;_nEpochs=nEpochs;_mu=mu;char name[255];sprintf(name,"Good_%g",_mu);_name=name;}
	virtual int operator()(int e){return _kStar+(int)((_M-_kStar)*GetPercFSA(e,_nEpochs/2,_mu));}
	void	Set_mu(Tp mu){_mu=mu;char name[255];sprintf(name,"Good_%g",_mu);_name=name;}
	int _M,_kStar,_nEpochs;
	Tp _mu;
	std::string _name;
};

template<class Tp>
class ScheduleInv:public Schedule<Tp>{
public:
	ScheduleInv(int M, int kstar,int nEpochs,Tp epStar):Schedule<Tp>(M,kstar,nEpochs,0){this->_mu=M/(kstar*epStar);char name[255];sprintf(name,"inv_%g",this->_mu);this->_name=name;}
	int operator()(int e){ int ns=(int)(this->_M/(1.f+(e+1.f)*this->_mu)-1.f/this->_nEpochs);
		return (this->_kStar>ns?this->_kStar:ns); }
	//int operator()(int e){ return _kStar+(int)((_M-_kStar)/(1.f+(e+1.f)*this->_mu)); }
};

template<class Tp>
class ScheduleInvA:public Schedule<Tp>{
public:
	ScheduleInvA(int M, int kstar,int nEpochs,Tp mu):Schedule<Tp>(M,kstar,nEpochs,mu){this->_mu=mu;char name[255];sprintf(name,"inv_%g",this->_mu);this->_name=name;}
	//int operator()(int e){ return (int)(_kStar+(_M-_kStar)/this->_mu*(1.f/(e+1.f)-1.5f/_nEpochs)); }
	int operator()(int e){ int ns=(int)(this->_M/this->_mu*(1.f/(1.f+e)-1.f/this->_nEpochs));
		return (this->_kStar>ns?this->_kStar:ns); }
};

template<class Tp>
class ScheduleLin:public Schedule<Tp>{
public:
	ScheduleLin(int M, int kstar,int nEpochs):Schedule<Tp>(M,kstar,nEpochs,0){char name[255];sprintf(name,"lin");this->_name=name;}
	int operator()(int e){ return (int)(std::max(this->_kStar,this->_M-(e*2*this->_M)/this->_nEpochs)); }
};

template<class Tp>
class ScheduleExp:public Schedule<Tp>{
public:
	ScheduleExp(int M, int kstar,int nEpochs,Tp mu):Schedule<Tp>(M,kstar,nEpochs,mu){this->_mu=mu;char name[255];sprintf(name,"exp_%g",this->_mu);this->_name=name;}
//	int operator()(int e){ return (int)(_kStar+(_M-_kStar)*(pow(this->_mu,e+1)-pow(this->_mu,_nEpochs))); }
	int operator()(int e){int ns=(int)(this->_M*(pow(this->_mu,e+1)-pow(this->_mu,this->_nEpochs)));
		return (this->_kStar>ns?this->_kStar:ns);}
};

#endif

