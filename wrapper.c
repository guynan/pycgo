
/* Python.h impiles `stdio, stdlib, etc` */
#include <Python.h>
#include "libgoprime.h"
#include "gohelper.h"


/* Function definitions */

/* Python Initialisation */
PyMODINIT_FUNC initcmod(void);


/* Externally visible Python functions */

/* Convention here is that we prefix the Go function with `C_` so we avoid
 * namespace collisions but still show clearly the relationship between the two
 * functions */

static PyObject* C_parseArray(PyObject* self, PyObject* args);
static PyObject* C_large_init(PyObject* self, PyObject* args);
static PyObject * C_GoisPrime(PyObject* self, PyObject* args);
static PyObject* C_returnslice(PyObject* self, PyObject* args);


/* End Function definitions */


/* Begin Python module setup */


/* Any Go function that intends to be called from Python has a partner C 
 * function wrapper to interface Go types with the Python-C API */
static PyMethodDef ModuleMethods[] = {

        {"isprime",  C_GoisPrime, METH_VARARGS,
        "Call our Go function through the C wrapper."},

        {"parse",  C_parseArray, METH_VARARGS,
        "Parse a Python list into a Go Slice."},

        {"large_init",  C_large_init, METH_VARARGS,
        "Generates a big ass list in the C api."},
        
        {"ret",  C_returnslice, METH_VARARGS,
        "Parse Go Slice into python list."},

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
        ModuleMethods,

        /* A set of optional arguments */

        /* An array of slot definitions for multi-phase initialization */
        NULL,

        /* A traversal function to call during GC traversal of the module */
        NULL,

        /* A clear function to call during GC clearing of the module */
        NULL,

        /* A function to call during deallocation of the module */
        NULL,
};


/* Initialises the Python module, this is then referenced in the PyModuleDef
 * structure */
PyMODINIT_FUNC PyInit_goprime(void)
{
        Py_Initialize();

        return PyModule_Create(&module);
}


/* End Python module setup */


static PyObject* C_GoisPrime(PyObject* self, PyObject* args)
{
        (void) self;

        int x = 0;

        if(!PyArg_ParseTuple(args, "i", &x))
                return 0;

        return PyLong_FromLong((long) GoisPrime(x));
}


static PyObject* C_returnslice(PyObject* self, PyObject* args)
{
        (void) self;
        (void) args;

        return _construct_pylist_from_goslice(returnArray());
}


static PyObject* C_parseArray(PyObject* self, PyObject* args)
{
        (void) self;

        PyObject* list;

        if(!PyArg_ParseTuple(args, "O", &list))
                goto cleanup;

        GoSlice slice = _construct_go_int_slice(list);

        /* Calls our Go Code */
        parseArray(slice);

cleanup:

        Py_XDECREF(list);

        return PyLong_FromLong(0L);
}




static PyObject* C_large_init(PyObject* self, PyObject* args)
{
        (void) self;

        PyObject* list = NULL;

        int x = 0;

        if(!PyArg_ParseTuple(args, "i", &x))
                return 0;

        list = PyList_New(100);

        for(Py_ssize_t i = 0; i < x; i++){
                if(isPrime(i)){
                        (void) PyList_Append(list, PyLong_FromSsize_t(i));
                }

        }

        return list;

}

