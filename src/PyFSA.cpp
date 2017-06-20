#include "PyFSA.h"

using namespace std;

#if PY_VERSION_HEX >= 0x03000000
#define PYMETHODTABLE static PyMethodDef myextension_methods[] = { {"TrainLinFSA", (PyCFunction)TrainLinFSA_py, METH_VARARGS },\
{ NULL, NULL, 0}};
#define PYINITIALIZATIONSTRUCTURE static struct PyModuleDef TrainLinFSA_py_module{\
	PyModuleDef_HEAD_INIT,\
	"TrainLinFSA",\
	"Extension module that provides Access to TrainLinFSA Through Python",\
	-1,\
	myextension_methods\
};
#define PYMODULEINITIALIZING PyMODINIT_FUNC PyInit_TrainLinFSA(void){\
	Py_Initialize();\
	return PyModule_Create(&TrainLinFSA_py_module);\
}
#define PYACCESSVALUE int d=fo->ob_digit[1];

#else //If Python is an earlier version than 3.0
#define PYMETHODTABLE static PyMethodDef myextension_methods[] = {  { "TrainLinFSA", TrainLinFSA_py, METH_VARARGS }, \
{ NULL, NULL, 0}};
#define PYINITIALIZATIONSTRUCTURE
#if defined(_WIN64) || defined(_WIN32) 
#define PYMODULEINITIALIZING extern "C" __declspec(dllexport) int initlibPyFSA()\
(void)Py_InitModule("TrainLinFSA_py", myextension_methods);\
import_array();}
#else
#define PYMODULEINITIALIZING PyMODINIT_FUNC initlibPyFSA()\
{\
	(void)Py_InitModule("TrainLinFSA_py", myextension_methods);\
	import_array();\
}
#define PYACCESSVALUE int d=fo->ob_ival;
#endif
#endif

bool GetVector(std::vector<float> &v, const PyObject* pyList) {
    if (pyList == NULL) {
        return false;
    }
	int n=(int)PyList_GET_SIZE(pyList);
    for (int i = 0; i < n; i++) {
		PyFloatObject *fo=(PyFloatObject*)PyList_GET_ITEM(pyList, i);
		float d=(float)fo->ob_fval;
		//printf("%g ", d);
        v.push_back(d);
    }
    return true;
}

bool GetVector(std::vector<int> &v, const PyObject* pyList) {
    if (pyList == NULL) {
        return false;
    }
	int n=(int)PyList_GET_SIZE(pyList);
    for (int i = 0; i < n; i++) {
		PYINT_OR_PYLONG_OBJ_CODE
		PYACCESSVALUE
		//printf("%d ", d);
        v.push_back(d);
    }
    return true;
}

void GetVector(std::vector<float> &v, const PyArrayObject* src) {
    if (src != NULL) {
		int n = (int)PyArray_DIM(src,0);
		v.resize(n);
        for (long int i = 0; i < n; i++) {
			double f=*reinterpret_cast<double*>(PyArray_GETPTR1(src,i));
			v[i]=(float)f;
			//printf("%1.2f,",f);
        }
    }
}

void GetVector(std::vector<int> &v, const PyArrayObject* src) {
    if (src != NULL) {
		int n = (int)PyArray_DIM(src,0);
		v.resize(n);
        for (long int i = 0; i < n; i++) {
			int f=*reinterpret_cast<int*>(PyArray_GETPTR1(src,i));
			v[i]=f;
			//printf("%1.2f,",f);
        }
    }
}

bool GetMatrix(std::vector<std::vector<float>> &M, const PyObject* pyList) {
    if (pyList == NULL) {
        return false;
    }
	int n=(int)PyList_GET_SIZE(pyList);
    for (int i = 0; i < n; i++) {
		std::vector<float> v;
		GetVector(v,(PyArrayObject*)PyList_GET_ITEM(pyList, i));
		printf("%d has size %d: ", i, (int)v.size());
		if (i<10||i>n-10)
			for (int j=0;j<(int)v.size();j++)
				printf("%g ", v[j]);
		printf("\n");
        M.push_back(v);
    }
    return true;
}

void GetVector(std::vector<short> &v, const PyArrayObject* src) {
    if (src != NULL) {
		int n = (int)PyArray_DIM(src,0);
		v.resize(n);
        for (long int i = 0; i < n; i++) {
			int f=*reinterpret_cast<int*>(PyArray_GETPTR1(src,i));
			v[i]=f;
        }
    }
}

void GetVector(std::vector<unsigned short> &v, const PyArrayObject* src) {
    if (src != NULL) {
		int n = (int)PyArray_DIM(src,0);
		v.resize(n);
        for (long int i = 0; i < n; i++) {
			int f=*reinterpret_cast<int*>(PyArray_GETPTR1(src,i));
			v[i]=f;
        }
    }
}

bool GetMatrix(std::vector<std::vector<float>> &M, const PyArrayObject* src) {
    if (src == NULL) {
        return false;
    }
	int nr=(int)PyArray_DIM(src,0);
	int nc=(int)PyArray_DIM(src,1);
	M.resize(nr);
    for (int i = 0; i < nr; i++) {
		vector<float> &v=M[i];
		v.resize(nc);
        for (long int j = 0; j < nc; j++) {
			double f=*reinterpret_cast<double*>(PyArray_GETPTR2(src,i,j));
			v[j]=(float)f;
        }
		if (i<0){
			for (int j=0;j<5;j++)
				printf("%g ", v[j]);
			printf("\n");
		}
    }
    return true;
}

bool GetMatrix(std::vector<std::vector<unsigned short>> &M, const PyArrayObject* src) {
    if (src == NULL) {
        return false;
    }
	int nr=(int)PyArray_DIM(src,0);
	int nc=(int)PyArray_DIM(src,1);
	M.resize(nr);
    for (int i = 0; i < nr; i++) {
		vector<unsigned short> &v=M[i];
		v.resize(nc);
        for (long int j = 0; j < nc; j++) {
			int f=*reinterpret_cast<int*>(PyArray_GETPTR2(src,i,j));
			v[j]=(unsigned short)f;
			//if (j<10||j>nc-10) printf("%d ", v[j]);
        }
		//printf("\n");
    }
    return true;
}

bool GetMatrix(std::vector<std::vector<unsigned short>> &M, const PyObject* pyList) {
    if (pyList == NULL) {
        return false;
    }
	int n=(int)PyList_GET_SIZE(pyList);
    for (int i = 0; i < n; i++) {
		std::vector<unsigned short> v;
		GetVector(v,(PyArrayObject*)PyList_GET_ITEM(pyList, i));
		printf("%d has size %d: ", i, (int)v.size());
		if (i<10||i>n-10){
			for (int j=0;j<(int)v.size();j++)
				printf("%d ", v[j]);
			printf("\n");
		}
        M.push_back(v);
    }
    return true;
}

PyObject* TrainLinFSA_py(PyObject *self, PyObject *args) {
	printf("TrainLinFSA(x,y,loss,nsel,ridge,[eta=1.,nEpochs=100,mu=100.,mom=0.9,minibatch=32,verbosity=0])\n");
	printf("loss: 0=Logistic, 1=Lorenz (default), 2=smooth SVM, 3=hinge, 4=L2 \n");
    PyArrayObject *pyData, *pyY;
	DataMemory<float, float> data;
	int kstar,nEpochs=100,minibatch=32,verb=0,loss;
	float eta=1.f,mu=100,mom=0.9f,ridge;

    if (!PyArg_ParseTuple(args, "O!O!iif|fiffii",
                          &PyArray_Type, &pyData,&PyArray_Type, &pyY,&loss,&kstar,&ridge,&eta,&nEpochs,&mu,&mom,&minibatch,&verb)) {
        return NULL;
    }
	GetMatrix(data._x,pyData);
	GetVector(data._y,pyY);
	vector<float> b;
	float b0,lv=0;
	vector<float> sel;
	switch(loss){
	case 0:
		lv=TrainLinFsa<LogisticLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	case 2:
		lv=TrainLinFsa<HuberSVMLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	case 3:
		lv=TrainLinFsa<HingeLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	case 4:
		lv=TrainLinFsa<SquareLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);break;
	default:
		lv=TrainLinFsa<LorenzLoss>(b,b0,sel, data,data.nObs(),kstar,0.8f,0,mu,nEpochs,eta,minibatch,mom,ridge,verb);
	}
	PyObject* v = PyList_New(0);
	for (int i=0;i<b.size();i++) {
        PyObject* f = PyFloat_FromDouble((double)b[i]);
        PyList_Append(v, f);
    }
	PyObject* sv = PyList_New(0);
	for (int i=0;i<(int)sel.size();i++) {
		//printf("%d,",sel[i]);
		PYLONG_OR_PYINT_CONVERSION_CODE
        PyList_Append(sv, f);
    }
	//printf("\n");
	PyObject* retVal = Py_BuildValue("fNNf",b0, v,sv,lv);
    return retVal;
}

PYMETHODTABLE

PYINITIALIZATIONSTRUCTURE

PYMODULEINITIALIZING
