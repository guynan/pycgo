
/* Python.h impiles `stdio, stdlib, etc` */
#include <Python.h>
#include "libgoprime.h"


/* Function definitions */
PyMODINIT_FUNC initcmod(void);
static PyObject * C_GoisPrime(PyObject* self, PyObject* args);


/* Begin Python module setup */


/* Any Go function that intends to be called from Python has a partner C 
 * function wrapper to interface Go types with the Python-C API */
static PyMethodDef ModuleMethods[] = {

        {"isprime",  C_GoisPrime, METH_VARARGS,
        "Call our Go function through the C wrapper."},

        /* Sentinel */
        {NULL, NULL, 0, NULL}
};


static struct PyModuleDef module = {

        PyModuleDef_HEAD_INIT,

        /* name of module */
        "goprime",

        /* module documentation, may be NULL */
        "C module wrapper for go code in Python",

        /* size of per-interpreter state of the module, or -1 if the module 
         * keeps state in global variables. */
        -1,             
                 
        /* The table containing the information about our methods */
        ModuleMethods
};


/* Initialises the Python module, this is then referenced in the PyModuleDef
 * structure */
PyMODINIT_FUNC PyInit_goprime(void)
{
        Py_Initialize();

        return PyModule_Create(&module);
}


/* End Python module setup */


/* Convention here is that we prefix the Go function with `C_` so we avoid
 * namespace collisions but still show clearly the relationship between the two
 * functions */
static PyObject* C_GoisPrime(PyObject* self, PyObject* args)
{
        int x = 0;

        if(!PyArg_ParseTuple(args, "i", &x))
                return 0;

        return PyLong_FromLong((long) GoisPrime(x));
}


