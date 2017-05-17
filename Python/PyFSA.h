#ifndef PYFSA_H
#define	PYFSA_H

#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)

extern "C" void INITNAME();

PyObject* TrainLinFSA(PyObject *self, PyObject *args);

#endif	/* PYCONVNET3_CUH */

