
/* Python.h impiles `stdio, stdlib, etc` */

#include <Python.h>
#include "libgoprime.h"
#include "wrapper.h"
#include "gohelper.h"


/* Initialises the Python module, this is then referenced in the PyModuleDef
 * structure */
PyMODINIT_FUNC PyInit_goprime(void)
{
        Py_Initialize();

        return PyModule_Create(&module);
}


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

