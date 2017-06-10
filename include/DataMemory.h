#ifndef DATA_MEMORY_H
#define DATA_MEMORY_H

#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <typeinfo>
#include "DataSource.h"


template<class Tp, class TpY>
class DataMemory:public DataSource<std::vector<Tp>, TpY>{
public:
	typedef DataSource<std::vector<Tp>, TpY> SuperClass;
	DataMemory();
	DataMemory(int nVar);
	DataMemory(int nVar,int nObs);
	void operator =(const DataMemory &a);
	virtual void Init(int nVar,int nObs);
	void Resize(int nObs);

	virtual void SetFirstObs();
	virtual bool SetNextObs();
	virtual std::vector<Tp> *GetObs(TpY &y);
	virtual std::vector<Tp> *GetObs(TpY &y, int i);
	int	 nObs() const {return (int)_x.size();}
	int	 nObs(TpY label) const {return CountOccurences(_y,label);}
	virtual int  nVar() const {return (int)_x[0].size();}
	void KeepOnly(std::vector<int> &vars){KeepOnly(vars,true);}
	virtual void KeepOnly(std::vector<int> &vars, bool resize);
	void MakeSquare();

	void AddObs(std::vector<Tp> &x, TpY y);
	void SetObs(Tp *x, TpY y);
	void SetObs(std::vector<Tp> &x, TpY y);
	void CopyObs(int i, std::vector<Tp> &x, TpY y);
	void SwapObs(int i, std::vector<Tp> &x, TpY y);
	//bool SetRow(const int i, const std::vector<Tp> &row, const int posY);
	//void Set(const std::vector<std::vector<Tp> > &m,const  int posY);
	void ClearObs();
	bool ReadBinary(const char *name);
	bool ReadBinary(const char *name, TpY y);
	bool ReadBinary(const char *name, TpY y, int nAllVar, std::vector<int> &vars);
	bool ReadBinary(const char *name0, const char *name1, float npp);
	bool SaveBinary(const char *name);
	bool ReadSvmX(const char *name,const char *dlm, bool append=false);
	bool ReadSvm(const char *name, const TpY valY, const char *dlm, bool append=false);
	bool ReadSvm(const char *name,const char *namey, const char *dlm, bool append=false);
	bool ReadText(const char *name, const int posY, const char *dlm);
	bool ReadText(const char *name, const int posY, const int ignore, const char *dlm);
	bool ReadText(const char *nameX, const char *nameY, const char * dlm);
	bool ReadY(const char *namey);
	bool SaveX(const char *name);
	bool SaveY(const char *name);
	bool SaveText(const char *name, int header=0);
	bool SaveVW(const char *name);
	bool SaveVW(const char *name, int nBins);
	void GetMinMax(Tp &vmin,Tp &vmax, int i); 
	virtual void GetMinMax(std::vector<Tp> &vmin,std::vector<Tp> &vmax); 
	//void GetRange(std::vector<Tp> &minx,std::vector<Tp> &binLenInv, int nBins);
	void Shuffle(std::vector<int> &idx);
	void Shuffle(std::vector<int> &idx, std::vector<Tp> &param);
	void CopyTransposed(DataMemory<Tp,TpY> &from);

	template<class Tp2>
	void Set(const std::vector<std::vector<Tp2> > &m,const int &posY);

	template<class Tp2>
	bool SetRow(const int &idx, const std::vector<Tp2> &v, const int &posYz);

	std::vector<TpY> _y;
	std::vector<std::vector<Tp> > _x;
protected:
	int  IsColInteger(int i);

	int _idxObs;
};

#endif
