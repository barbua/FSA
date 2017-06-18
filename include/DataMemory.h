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
	//Args take float vectors now instead of ints now!
	void KeepOnly(std::vector<float> &vars){KeepOnly(vars,true);}
	virtual void KeepOnly(std::vector<float> &vars, bool resize);
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

template<class Tp>
bool ReadMinMaxText(std::vector<Tp> &minx, std::vector<Tp> &maxx, const char *name, int posY) {
	std::vector<Tp> x;
	int i, y, nVar;
	FILE *f = fopen(name, "rt");
	if (f == 0)
		return false;
	while (!feof(f)) {
		if (ReadTextLine(x, y, f, " ", posY, 1000000)) {
			if (minx.empty()) {
				minx = x;
				nVar = (int)x.size();
			}
			else
				for (i = 0;i<nVar;i++)
					minx[i] = (minx[i]<x[i] ? minx[i] : x[i]);
			if (maxx.empty())
				maxx = x;
			else
				for (i = 0;i<nVar;i++)
					maxx[i] = (maxx[i]>x[i] ? maxx[i] : x[i]);
		}
	}
	fclose(f);
	return true;
}

template<class Tp>
bool ReadToBinsText(DataMemory<unsigned char, int> &data, std::vector<Tp> &minx, std::vector<Tp> &maxx, const char *name, int nBins, int posY) {
	std::vector<Tp> x, binLenInv;
	int i, y, nVar, b;
	std::vector<unsigned char> bin;

	if (!ReadMinMaxText(minx, maxx, name, posY))
		return false;
	nVar = (int)minx.size();
	binLenInv.assign(nVar, 0);
	bin.resize(nVar);
	for (i = 0;i<nVar;i++)
		if (maxx[i]>minx[i])
			binLenInv[i] = nBins / (maxx[i] - minx[i]);
	FILE *f = fopen(name, "rt");
	if (f == 0)
		return false;
	while (!feof(f)) {
		if (ReadTextLine(x, y, f, " ", posY, 1000000)) {
			for (i = 0;i<nVar;i++) {
				b = std::max(std::min(int((x[i] - minx[i])*binLenInv[i]), (int)nBins - 1), 0);
				bin[i] = (unsigned char)b;
			}
			data.AddObs(bin, y);
		}
	}
	fclose(f);
	return true;
}

template<class Tp, class TpQ>
bool ReadPairsText(DataMemory<Tp, Tp> &data, std::vector<TpQ> &qid, const char *name) {
	std::vector<Tp> x;
	Tp y;
	TpQ q;

	FILE *f = fopen(name, "rt");
	if (f == 0)
		return false;
	while (!feof(f)) {
		if (ReadLineOfPairs(x, y, q, f)) {
			data.AddObs(x, y);
			qid.push_back(q);
		}
	}
	fclose(f);
	return true;
}

template<class Tp>
bool ReadMinMaxPairs(std::vector<Tp> &minx, std::vector<Tp> &maxx, const char *name) {
	std::vector<Tp> x;
	Tp y;
	int i, q, nVar;
	FILE *f = fopen(name, "rt");
	if (f == 0)
		return false;
	while (!feof(f)) {
		if (ReadLineOfPairs(x, y, q, f)) {
			if (minx.empty()) {
				minx = x;
				nVar = (int)x.size();
			}
			else
				for (i = 0;i<nVar;i++)
					minx[i] = std::min(minx[i], x[i]);
			if (maxx.empty())
				maxx = x;
			else
				for (i = 0;i<nVar;i++)
					maxx[i] = std::max(maxx[i], x[i]);
		}
	}
	fclose(f);
	return true;
}

template<class Tp>
bool ReadToBinsPairs(DataMemory<unsigned char, Tp> &data, std::vector<Tp> &minx, std::vector<Tp> &maxx, std::vector<int> &qid, const char *name, int nBins) {
	std::vector<Tp> x, binLenInv;
	Tp y;
	int i, q, nVar, b;
	std::vector<unsigned char> bin;

	if (!ReadMinMaxPairs(minx, maxx, name))
		return false;
	nVar = (int)minx.size();
	binLenInv.assign(nVar, 0);
	bin.resize(nVar);
	for (i = 0;i<nVar;i++)
		if (maxx[i]>minx[i])
			binLenInv[i] = nBins / (maxx[i] - minx[i]);
	FILE *f = fopen(name, "rt");
	if (f == 0)
		return false;
	qid.clear();
	while (!feof(f)) {
		if (ReadLineOfPairs(x, y, q, f)) {
			for (i = 0;i<nVar;i++) {
				b = std::max(std::min(int((x[i] - minx[i])*binLenInv[i]), (int)nBins - 1), 0);
				bin[i] = (unsigned char)b;
			}
			data.AddObs(bin, y);
			qid.push_back(q);
		}
	}
	fclose(f);
	return true;
}

template<class Tp, class Source>
void CopyData(DataMemory<Tp, int> &to, Source &from, int nObs) {
	int y;
	to.Init(from.NumVar(), nObs);
	to.SetFirstObs();
	from.SetFirstObs();
	for (int n = 0;n<nObs;n++) {
		Tp *x = from.GetObs(y);
		to.SetObs(x, y);
		to.SetNextObs();
		from.SetNextObs();
	}
}

template<class Tp, class TpY>
void CopySubset(DataMemory<Tp, TpY> &to, DataMemory<Tp, TpY> &from, std::vector<int> &idx) {
	TpY y;
	int nObs = (int)idx.size();
	to.Init((int)from._x[0].size(), nObs);
	for (int i = 0;i<nObs;i++) {
		std::vector<Tp> *x = from.GetObs(y, idx[i]);
		to.CopyObs(i, *x, y);
	}
}

template<class Tp, class TpY>
void CopyAllButOne(DataMemory<Tp, TpY> &to, DataMemory<Tp, TpY> &from, int iSkip) {
	TpY y;
	int nObs = from.nObs();
	to.Init((int)from._x[0].size(), nObs - 1);
	for (int i = 0;i<nObs;i++) {
		if (i == iSkip)
			continue;
		std::vector<Tp> *x = from.GetObs(y, i);
		if (i<iSkip)
			to.CopyObs(i, *x, y);
		else
			to.CopyObs(i - 1, *x, y);
	}
}

template<class Tp, class TpY>
void CopyOneObs(DataMemory<Tp, TpY> &to, DataMemory<Tp, TpY> &from, int i) {
	TpY y;
	to.Init((int)from._x[i].size(), 1);
	std::vector<Tp> *x = from.GetObs(y, i);
	to.CopyObs(0, *x, y);
}

template<class TpY>
void CopyToGivenBins(DataMemory<unsigned char, TpY> &to, std::vector<float> &minx, std::vector<float> &binLenInv, DataMemory<float, TpY> &data, int nBins) {
	int i, M = data._nVar, b, nObs = data.nObs();
	TpY y;
	std::vector<unsigned char> bin(M);

	to.SetFirstObs();
	data.SetFirstObs();
	for (int n = 0;n<nObs;n++) {
		std::vector<float> *x = data.GetObs(y);
		for (i = 0;i<M;i++) {
			b = std::min(int((x->at(i) - minx[i])*binLenInv[i]), nBins - 1);
			bin[i] = (unsigned char)b;
		}
		to.AddObs(bin, y);
		to.SetNextObs();
		data.SetNextObs();
	}
}


template<class Tp, class Source>
void CopySelected(DataMemory<Tp, int> &to, Source &from, int nObs, std::vector<int> &selected) {
	int y, ns = (int)selected.size();
	to.Init((int)selected.size(), nObs);
	to.SetFirstObs();
	from.SetFirstObs();
	for (int n = 0;n<nObs;n++) {
		std::vector<Tp> x(ns);
		std::vector<Tp> *x0 = from.GetObs(y);
		CopySelected(x, &x0->at(0), selected);
		to.SetObs(x, y);
		to.SetNextObs();
		from.SetNextObs();
	}
}


template<class Tp, class TpY>
void ComputeMeanVar(std::vector<Tp> &mean, std::vector<Tp> &var, DataMemory<Tp, TpY> &data) {
	int i, j, nf = (int)data.nVar();
	int	n = data.nObs();
	Tp feat;

	mean.assign(nf, 0);
	var.assign(nf, 0);

	for (i = 0; i<n; i++) {
		std::vector<Tp> &x = data._x[i];
		for (j = 0; j<nf; j++) {
			feat = x[j];
			mean[j] += feat;
			var[j] += feat*feat;
		}
	}
	for (j = 0; j<nf; j++) {
		mean[j] /= n;
		// The Population Variance is computed here
		var[j] = var[j] / n - mean[j] * mean[j];
	}
}

template<class Tp, class TpY>
void RescaleData(DataMemory<Tp, TpY> &data, std::vector<Tp> &mean, std::vector<Tp> &var) {
	int i, j, nf = (int)data.nVar();
	int	n = data.nObs();
	std::vector<Tp> is;

	is.resize(nf);
	for (j = 0; j<nf; j++) {
		if (var[j]>0)
			is[j] = 1 / sqrt(var[j]);
		else
			is[j] = 0;
	}
	for (i = 0; i<n; i++) {
		std::vector<Tp> &x = data._x[i];
		for (j = 0; j<nf; j++) {
			x[j] = (x[j] - mean[j])*is[j];
		}
	}
}

template<class Tp, class TpY>
DataMemory<Tp, TpY>::DataMemory() {
	Init(1, 0);
}

template<class Tp, class TpY>
DataMemory<Tp, TpY>::DataMemory(int nVar) {
	Init(nVar, 0);
}

template<class Tp, class TpY>
DataMemory<Tp, TpY>::DataMemory(int nVar, int nObs) {
	Init(nVar, nObs);
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::Init(int nVar, int nObs) {
	DataSource<std::vector<Tp>, TpY>::_nVar = nVar;
	_idxObs = -1;
	std::vector<Tp> x(nVar);
	_x.assign(nObs, x);
	_y.resize(nObs);
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::operator =(const DataMemory &a) {
	if (this != &a) {
		DataSource<std::vector<Tp>, TpY>::_nVar = a._nVar;
		_y = a._y;_x = a._x;
	}
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::Resize(int nObs) {
	std::vector<Tp> x(DataSource<std::vector<Tp>, TpY>::_nVar);
	_x.resize(nObs, x);
	_y.resize(nObs);
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::MakeSquare() {
	// fills in with zeros to make all rows have the same number of elements
	int i, j, n = (int)_x.size(), m = 0;
	for (i = 0;i<n;i++)
		m = std::max(m, (int)_x[i].size());
	for (i = 0;i<n;i++) {
		for (j = (int)_x[i].size();j<m;j++)
			_x[i].push_back(0);
	}
	DataSource<std::vector<Tp>, TpY>::_nVar = m;
}


template<class Tp, class TpY>
/*Changed std::vector<int> &selected to std::vector<float> &selected*/
void DataMemory<Tp, TpY>::KeepOnly(std::vector<float> &selected, bool resize) {
	int nobs = nObs();
	DataSource<std::vector<Tp>, TpY>::_nVar = (int)selected.size();
	for (int n = 0;n<nobs;n++) {
		//std::vector<Tp> x0=_x[n];
		//CopySelected(_x[n],&x0[0],selected,resize);
		KeepOnly(_x[n]);
		if (resize)
			_x[n].shrink_to_fit();
	}
}


template<class Tp, class TpY>
void DataMemory<Tp, TpY>::Shuffle(std::vector<int> &idx) {
	Shuffle(_x, idx);
	Shuffle(_y, idx);
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::Shuffle(std::vector<int> &idx, std::vector<Tp> &param) {
	Shuffle(_x, idx);
	Shuffle(_y, idx);
	Shuffle(param, idx);
}

/*

template<class Tp,class TpY>
bool DataMemory<Tp,TpY>::ReadSvmX(const char *name, const char *dlm, bool append){
char line[100000];
std::vector<std::string> v,v2;
FILE *f=fopen(name,"rt");
if (f==0)
return false;
if (!append){
_x.clear();
_y.clear();
SuperClass::_nVar=0;
}
while(!feof(f)){
std::vector<Tp> tmp;
tmp.reserve(1000);
tmp.assign(SuperClass::_nVar,0);
if (fgets(line, 100000, f)==0)
break;
PutIntoVector(v, line,dlm);
for (int i=0;i<(int)v.size();i++){
PutIntoVector(v2, v[i].c_str(),":");
int idx=atoi(v2[0].c_str());
Tp val=(Tp)atof(v2[1].c_str());
while (idx>=tmp.size())
tmp.push_back(0);
tmp[idx]=val;
}
SuperClass::_nVar=std::max((int)tmp.size(),SuperClass::_nVar);
_x.push_back(tmp);
}
fclose(f);
return true;
}
*/

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadSvm(const char *name, const TpY valY, const char *dlm, bool append) {
	if (!ReadSvmX(name, dlm, append))
		return false;
	_y.assign(_x.size(), valY);
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadSvm(const char *name, const char *namey, const char *dlm, bool append) {
	if (!ReadSvmX(name, dlm, append))
		return false;
	if (!ReadY(namey))
		return false;
	return _y.size() == _x.size();
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadText(const char *name, const int posY, const char *dlm) {
	std::vector<std::vector<Tp> > m;

	if (!DlmRead(name, m, dlm))
		return false;

	Set(m, posY);
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadText(const char *name, const int colY, const int colIgnore, const char *dlm) {
	std::vector<std::vector<Tp> > m;

	if (!DlmRead(name, m, dlm))
		return false;

	int i, n = (int)m.size();
	_x.resize(n);
	_y.resize(n);
	for (i = 0;i<n;i++) {
		if (colIgnore >= 0)
			RemoveAtPosition(m[i], colIgnore);
		if (colY<colIgnore || colIgnore<0)
			SetRow(i, m[i], colY);
		else
			SetRow(i, m[i], colY - 1);
	}
	if (_x.size()>0)
		DataSource<std::vector<Tp>, TpY>::_nVar = (int)_x[0].size();
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadY(const char *nameY) {
	TpY y;
	char line[1000];
	FILE *f = fopen(nameY, "rt");
	if (f == 0)
		return false;
	_y.clear();
	while (!feof(f)) {
		if (fgets(line, 1000, f) == 0)
			break;
		y = (TpY)atof(line);
		_y.push_back(y);
	}
	fclose(f);
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadText(const char *nameX, const char *nameY, const char *dlm) {
	char *line = new char[100000];

	if (!DlmRead(nameX, _x, dlm))
		return false;

	if (!ReadY(nameY))
		return false;
	DataSource<std::vector<Tp>, TpY>::_nVar = (int)_x[0].size();
	return _y.size() == _x.size();
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::SaveX(const char *name) {
	FILE *f = fopen(name, "wt");
	int i, N = (int)_y.size();
	if (f == NULL)
		return false;
	for (i = 0;i<N;i++) {
		DlmWrite(f, &_x[i][0], (int)_x[i].size(), "%g", ',', 0);
		fprintf(f, "\n");
	}
	fclose(f);
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::SaveY(const char *name) {
	FILE *f = fopen(name, "wt");
	int i, N = (int)_y.size();
	if (f == NULL)
		return false;
	for (i = 0;i<N;i++) {
		fprintf(f, "%g\n", (double)_y[i]);
	}
	fclose(f);
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::SaveText(const char *name, int header) {
	FILE *f = fopen(name, "wt");
	int i, N = (int)_y.size();
	if (f == NULL)
		return false;
	if (header != 0) {
		int n = (int)_x[0].size();
		for (i = 0;i<n;i++)
			fprintf(f, "x%d,", i);
		fprintf(f, "y\n");
	}
	for (i = 0;i<N;i++) {
		DlmWrite(f, &_x[i][0], (int)_x[i].size(), "%g", ',', 0);
		fprintf(f, ",%g\n", (double)_y[i]);
	}
	fclose(f);
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::SaveVW(const char *name) {
	FILE *f = fopen(name, "wt");
	int i, N = (int)_y.size(), M = DataSource<std::vector<Tp>, TpY>::nVar();
	if (f == NULL)
		return false;
	for (i = 0;i<N;i++) {
		fprintf(f, "%d |features", (_y[i]>0 ? _y[i] : -1));//
		for (int j = 1;j<M;j++)
			fprintf(f, " %d:%g", j, (double)_x[i][j]);
		fprintf(f, "\n");
	}
	fclose(f);
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::SaveVW(const char *name, int nBins) {
	FILE *f = fopen(name, "wt");
	int i, N = (int)_y.size(), M = DataSource<std::vector<Tp>, TpY>::nVar();
	if (f == NULL)
		return false;
	for (i = 0;i<N;i++) {
		fprintf(f, "%d |features", (_y[i]>0 ? _y[i] : -1));//
		for (int j = 1;j<M;j++)
			fprintf(f, " %d", j*nBins + (int)_x[i][j]);
		fprintf(f, "\n");
	}
	fclose(f);
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::SaveBinary(const char *name) {
	std::ofstream strm;
	int i, N = (int)_y.size();
	strm.open(name, std::ios::out | std::ios::binary);
	if (strm.fail())
		return false;
	for (i = 0;i<N;i++) {
		strm.write((char*)&_x[i][0], sizeof(Tp)*DataSource<std::vector<Tp>, TpY>::_nVar);
		strm.write((char*)&_y[i], sizeof(int) * 1);
	}
	strm.close();
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadBinary(const char *name) {
	std::vector<Tp> data(DataSource<std::vector<Tp>, TpY>::_nVar);
	TpY y;
	std::ifstream strm;
	strm.open(name, std::ios::in | std::ios::binary);
	if (strm.fail())
		return false;
	while (!strm.eof()) {
		strm.read((char*)&data[0], sizeof(Tp)*DataSource<std::vector<Tp>, TpY>::_nVar);
		strm.read((char*)&y, sizeof(int) * 1);
		if (!strm.fail()) {//!strm.eof()&&
			_x.push_back(data);
			_y.push_back(y);
		}
	}
	strm.close();
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadBinary(const char *name, TpY y) {
	std::vector<Tp> data(DataSource<std::vector<Tp>, TpY>::_nVar);
	std::ifstream strm;
	strm.open(name, std::ios::in | std::ios::binary);
	if (strm.fail())
		return false;
	while (!strm.eof()) {
		strm.read((char*)&data[0], sizeof(Tp)*DataSource<std::vector<Tp>, TpY>::_nVar);
		if (!strm.fail()) {//!strm.eof()&&
			_x.push_back(data);
			_y.push_back(y);
		}
	}
	strm.close();
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadBinary(const char *name, TpY y, int nAllVar, std::vector<int> &vars) {
	int nv = (int)vars.size();
	std::vector<Tp> data(nAllVar), tmp(nv);
	std::ifstream strm;
	strm.open(name, std::ios::in | std::ios::binary);
	if (strm.fail())
		return false;
	while (!strm.eof()) {
		strm.read((char*)&data[0], sizeof(Tp)*nAllVar);
		if (!strm.eof()) {
			for (int i = 0;i<nv;i++)
				tmp[i] = data[vars[i]];
			_x.push_back(tmp);
			_y.push_back(y);
		}
	}
	strm.close();
	DataSource<std::vector<Tp>, TpY>::_nVar = nv;
	return true;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::ReadBinary(const char *name0, const char *name1, float npp) {
	std::vector<Tp> data(DataSource<std::vector<Tp>, TpY>::_nVar);
	std::ifstream strm0, strm1;
	int n0 = 0, n1 = 0;
	strm0.open(name0, std::ios::in | std::ios::binary);
	strm1.open(name1, std::ios::in | std::ios::binary);
	while (!strm0.eof() || !strm1.eof()) {
		if (n0<n1*npp) {
			strm0.read((char*)&data[0], sizeof(Tp)*DataSource<std::vector<Tp>, TpY>::_nVar);
			if (!strm0.eof()) {
				_x.push_back(data);
				_y.push_back(0);
			}
			n0++;
		}
		else {
			strm1.read((char*)&data[0], sizeof(Tp)*DataSource<std::vector<Tp>, TpY>::_nVar);
			if (!strm1.eof()) {
				_x.push_back(data);
				_y.push_back(1);
			}
			n1++;
		}
	}
	strm0.close();
	strm1.close();
	return true;
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::SetFirstObs() {
	_idxObs = 0;
}

template<class Tp, class TpY>
bool DataMemory<Tp, TpY>::SetNextObs() {
	_idxObs++;
	return true;
}

template<class Tp, class TpY>
std::vector<Tp> *DataMemory<Tp, TpY>::GetObs(TpY &y, int i) {
	y = _y[i];
	return &_x[i];
}

/* ****** DataMemory() ******
This function returns a pointer to the observation '_x[_idxObs]'. Recall
that the observation index '_idxObs' is updated internally.
*/
template<class Tp, class TpY>
std::vector<Tp> *DataMemory<Tp, TpY>::GetObs(TpY &y) {
	y = _y[_idxObs];
	return &_x[_idxObs];
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::ClearObs() {
	_x[_idxObs].clear();
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::SetObs(Tp *x, TpY y) {
	_y[_idxObs] = y;
	_x[_idxObs].resize(DataSource<std::vector<Tp>, TpY>::_nVar);
	for (int i = 0;i<DataSource<std::vector<Tp>, TpY>::_nVar;i++)
		_x[_idxObs][i] = x[i];
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::SetObs(std::vector<Tp> &x, TpY y) {
	_y[_idxObs] = y;
	_x[_idxObs].swap(x);
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::CopyObs(int i, std::vector<Tp> &x, TpY y) {
	_y[i] = y;
	_x[i] = x;
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::SwapObs(int i, std::vector<Tp> &x, TpY y) {
	// Assign zero to the ith entry in '_y'
	_y[i] = y;
	// Since '_x' is a vector of vectors, the 'swap()' function replaces the ith 
	// vector/row in '_x' with the passed in vector, 'x' 
	_x[i].swap(x);//_x[i]=x ?
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::AddObs(std::vector<Tp> &x, TpY y) {
	_y.push_back(y);
	_x.push_back(x);
	DataSource<std::vector<Tp>, TpY>::_nVar = (int)x.size();
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::GetMinMax(Tp &vmin, Tp &vmax, int j) {
	int i, n = nObs();
	if (n == 0)
		return;
	vmin = _x[0][j];
	vmax = vmin;
	for (i = 1;i<n;i++) {
		vmin = std::min(vmin, _x[i][j]);
		vmax = std::max(vmax, _x[i][j]);
	}
}

template<class Tp, class TpY>
void DataMemory<Tp, TpY>::GetMinMax(std::vector<Tp> &vmin, std::vector<Tp> &vmax) {
	int i, j, n = nObs();
	if (n == 0)
		return;
	vmin = _x[0];
	vmax = vmin;
	int M = (int)vmin.size();
	for (i = 1;i<n;i++)
		for (j = 0;j<M;j++) {
			vmin[j] = std::min(vmin[j], _x[i][j]);
			vmax[j] = std::max(vmax[j], _x[i][j]);
		}
}

template<class Tp, class TpY>
int DataMemory<Tp, TpY>::IsColInteger(int j) {
	int n = (int)_x.size(), ix;
	for (int i = 0;i<n;i++) {
		ix = (int)_x[i][j];
		if (ix != _x[i][j])
			return 0;
	}
	return 1;
}

template<class Tp, class TpY> template<class Tp2>
void DataMemory<Tp, TpY>::Set(const std::vector<std::vector<Tp2> > &m, const int &posY) {
	int i, n = (int)m.size();
	_x.resize(n);_y.resize(n);
	for (i = 0;i<n;i++)
		SetRow(i, m[i], posY);
	if (_x.size()>0)
		DataSource<std::vector<Tp>, TpY>::_nVar = (int)_x[0].size();
}

template<class Tp, class TpY> template <class Tp2>
bool DataMemory<Tp, TpY>::SetRow(const int &idx, const std::vector<Tp2> &v, const int &posYz) {
	int i, n = (int)v.size(), posy = posYz;
	if (n <= 1)	return false;
	if (posy<0)
		posy = n - 1;
	std::vector<Tp> &x = _x[idx];
	x.resize(n - 1);
	for (i = 0;i<posy;i++)
		x[i] = (Tp)v[i];
	if (typeid(int) == typeid(TpY))
		_y[idx] = (TpY)(v[posy] + 0.5);
	else
		_y[idx] = (TpY)v[posy];
	for (i = posy + 1;i<n;i++)
		x[i - 1] = (Tp)v[i];
	return true;
}

#endif
