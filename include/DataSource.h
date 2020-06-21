#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include <vector>
#include <set>
#include <cmath>


template<class Tp, class TpY>
class DataSource {
public:
	typedef Tp Container;
	typedef TpY Y_type;
	int _nVar; // number of variables

	DataSource() { _nVar = 0; }
	DataSource(int nVar) { _nVar = nVar; }
	virtual int nVar() const { return _nVar; }
	virtual int nObs() const = 0;
	virtual int nObs(TpY label) const = 0;
	virtual void SetFirstObs() = 0;
	virtual bool SetNextObs() = 0;
	virtual Tp *GetObs(TpY &y) = 0;
	virtual Tp *GetObs(TpY &y, int i) = 0;
	virtual	void SetObs(Tp &x, TpY y) = 0;
	virtual void Init(int nVar, int nObs) {}
	virtual void KeepOnly(std::vector<int> &vars, bool resize = false) = 0;
};
#endif

