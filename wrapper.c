
/* Python.h impiles `stdio, stdlib, etc` */
#include <Python.h>
#include "libgoprime.h"


/* Function definitions */

/* Python Initialisation */
PyMODINIT_FUNC initcmod(void);


/* C Scoped helper functions */
GoSlice _construct_go_int_slice(PyObject* list);
PyObject* _construct_pylist_from_goslice(struct unsafearray data);
uint64_t isPrime(Py_ssize_t s);


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


static PyObject* C_GoisPrime(PyObject* self, PyObject* args)
{
        int x = 0;

        if(!PyArg_ParseTuple(args, "i", &x))
                return 0;

        return PyLong_FromLong((long) GoisPrime(x));
}


static PyObject* C_returnslice(PyObject* self, PyObject* args)
{
       return _construct_pylist_from_goslice(returnArray());
}


static PyObject* C_parseArray(PyObject* self, PyObject* args)
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
PyObject* _construct_pylist_from_goslice(struct unsafearray arr)
{
        /* Assumes that our data coming to us is of byte type */
        char* data = arr.data;
        int err;

        PyObject* list = NULL;
        PyObject* item = NULL;

        list = PyList_New(arr.length);

        for(Py_ssize_t i = 0; i < arr.length; i++){

                item = Py_BuildValue("C", data[i]);

                if(!item)
                        goto fuck;

                err = PyList_SetItem(list, i, item);

                if(err)
                        goto fuck;

        }

fuck:

        if(item)
                Py_XDECREF(item);

        free(data);
        data = NULL;

        return list;

}


static PyObject* C_large_init(PyObject* self, PyObject* args)
{
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

/* Note that this is considerably faster than calling out to Go. */
uint64_t isPrime(Py_ssize_t s)
{
        if (s < 2) return 0;

        /* Preventing rounding errors */
        Py_ssize_t top = (Py_ssize_t) round(sqrt(s) + 1);

        for(Py_ssize_t i = 2; i < top; i++){

                if(s % i == 0)
                        return 0;
        }

        return 1;
}

