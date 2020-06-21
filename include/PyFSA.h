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

#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)

extern "C" void INITNAME();

extern "C" PyObject* TrainLinFSA(PyObject *self, PyObject *args);

#endif	/* PYCONVNET3_CUH */

#if PY_VERSION_HEX >= 0x03000000
#define PYINT_OR_PYLONG_OBJ_CODE PyLongObject *fo = (PyLongObject*)PyList_GET_ITEM(pyList, i);
#define PYLONG_OR_PYINT_CONVERSION_CODE PyObject* f = PyLong_FromLong((long)sel[i]);

#else
#define PYINT_OR_PYLONG_OBJ_CODE PyIntObject *fo = (PyIntObject*)PyList_GET_ITEM(pyList, i);
#define PYLONG_OR_PYINT_CONVERSION_CODE PyObject* f = PyInt_FromLong((long)sel[i]);
#endif