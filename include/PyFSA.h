#ifndef PYFSA_H
#define	PYFSA_H


#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#include <arrayobject.h>
#include <ndarrayobject.h>
#define _DEBUG
#else
#include <Python.h>
//#include <numpy/arrayobject.h>
//#include <numpy/ndarrayobject.h>
#include <arrayobject.h>
#include <ndarrayobject.h>
#endif
#include <assert.h>
#include <time.h>
#include <vector>

#include "Fsa.h"
#include "DataMemory.h"


#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)

extern "C" void INITNAME();

PyObject* TrainLinFSA(PyObject *self, PyObject *args);

#endif	/* PYCONVNET3_CUH */

