
#include <Python.h>
#include "libadd.h"


PyMODINIT_FUNC initcmod(void);
static PyObject * Cprime(PyObject* self, PyObject* args);




static PyObject * Cprime(PyObject* self, PyObject* args)
{
        int x = 0;

        if(!PyArg_ParseTuple(args, "i", &x))
                return 0;

        return PyLong_FromLong((long) GoisPrime(x));
}


static PyMethodDef ModuleMethods[] = {

    {"isprime",  Cprime, METH_VARARGS,
     "Call our Go function through the C wrapper."},

    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static struct PyModuleDef module = {
        PyModuleDef_HEAD_INIT,
        "goprime",              /* name of module */
        "C module wrapper for go code in Python",/* module documentation, may be NULL */
                -1,             /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
        ModuleMethods
};



PyMODINIT_FUNC PyInit_goprime(void)
{
        Py_Initialize();
        return PyModule_Create(&module);
}





