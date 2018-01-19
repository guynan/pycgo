
/* Python.h impiles `stdio, stdlib, etc` */
#include <Python.h>
#include "libgoprime.h"
#include <stdlib.h>


/* Function definitions */

/* Python Initialisation */
PyMODINIT_FUNC initcmod(void);


/* C Scoped helper functions */
GoSlice _construct_go_int_slice(PyObject* list);
PyObject* _construct_pylist_from_goslice(GoSlice slice);


/* Externally visible Python functions */
static PyObject* c_parseArray(PyObject* self, PyObject* args);
static PyObject * C_GoisPrime(PyObject* self, PyObject* args);


/* End Function definitions */


/* Begin Python module setup */


/* Any Go function that intends to be called from Python has a partner C 
 * function wrapper to interface Go types with the Python-C API */
static PyMethodDef ModuleMethods[] = {

        {"isprime",  C_GoisPrime, METH_VARARGS,
        "Call our Go function through the C wrapper."},

        {"parse",  c_parseArray, METH_VARARGS,
        "Parse a Python list into a Go Slice."},

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


static PyObject* c_parseArray(PyObject* self, PyObject* args)
{
        PyObject* list;

        uint64_t ret = -1;

        if(!PyArg_ParseTuple(args, "O", &list))
                goto cleanup;

        GoSlice slice = _construct_go_int_slice(list);

        /* Calls our Go Code */
        parseArray(slice);

cleanup:

        Py_XDECREF(list);

        return PyLong_FromLong(0L);
}


/* Parses a Python *int* array, constructs a C array with the corresponding
 * values and subsequently assuages this into a Go []int slice */
GoSlice _construct_go_int_slice(PyObject* list)
{
        Py_ssize_t len = PyList_Size(list);

        /* We don't actually need to free this as it then cleaned up in the go
         * code by the garbage collection */
        Py_ssize_t* data = malloc(len * sizeof(Py_ssize_t));

        /* If we can't initialise some memory we offload this problem to the Go
         * code and send down an empty array */
        /*
        if(!data)
                goto error;
        */

        for(Py_ssize_t i = 0; i < len; i++){
                PyObject* item = PyList_GetItem(list, i);
                data[i] = PyLong_AsLongLong(item);
        }

        /* initialise and return a Go Slice */
        GoSlice slice = {.data = data, .len = len, .cap = len,};

        return slice;

}


/* Creation of a Python list by reading the data from a Go slice. At the moment
 * it only supports the int type which is uint64_t */
PyObject* _construct_pylist_from_goslice(GoSlice slice)
{
        /* Assumes that our data coming to us is of type uint64_t */
        uint64_t* data = slice.data;

        PyObject* list = NULL;
        PyObject* item = NULL;

        list = PyList_New(slice.cap);

        for(Py_ssize_t i = 0; i < slice.cap; i++){

                item = PyLong_FromLongLong(data[i]);

                if(PyList_SetItem(list, i, item) < 0)
                        goto fuck;

        }

        /* BIG question -- Do I free the data??? */

fuck:
        Py_XDECREF(item);

        return list;

}



