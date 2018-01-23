#include <Python.h>
#include "libgoprime.h"
#include "gohelper.h"


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
                Py_XDECREF(item);

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

/* Parses a Python *int* array, constructs a C array with the corresponding
 * values and subsequently assuages this into a Go []int slice */
GoSlice _construct_go_int_slice(PyObject* list)
{
        Py_ssize_t len = PyList_Size(list);
        PyObject* item = NULL;

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
                item = PyList_GetItem(list, i);
                if(!item)
                        goto cleanup;

                data[i] = PyLong_AsLongLong(item);
        }

        /* initialise and return a Go Slice */
        GoSlice slice = {.data = data, .len = len, .cap = len,};

cleanup:
        
        Py_XDECREF(item);

        return slice;

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


