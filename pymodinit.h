#ifndef __PYMOD_INIT__
#define __PYMOD_INIT__


/* Convention here is that we prefix the Go function with `C_` so we avoid
 * namespace collisions but still show clearly the relationship between the two
 * functions 
 *
 * For each of these methods, they must be declared in the ModuleMethods table
 * in the pymodinit.c whereby *only* then can it be registered to the Python
 * interpreter as an executable function. All Python functions must have the
 * same signature of 
 *
 * static PyObject* method(PyObject* self, PyObject* args);
 *
 * in order to be able to be run by the interpreter. 
 *
 * These C functions are designed to be a wrapper around the input values to
 * convert them into either C types or Go types and the converse being true to
 * ensure that the return values of the Go or C functions can be sent to the
 * interpreter with no problems. The philosophy here is that the C is the
 * `glue` in the code and so neither the Python nor the Go code must be aware
 * of the coersions to ensure the marshalling of data is consistent. This keeps
 * the interface clean and means that Python and Go can continue operating at a
 * high level -- which is why you would write in them in the first place */
 

/* Externally visible Python functions */
static PyObject* C_parseArray(PyObject* self, PyObject* args);
static PyObject* C_large_init(PyObject* self, PyObject* args);
static PyObject * C_GoisPrime(PyObject* self, PyObject* args);
static PyObject* C_returnslice(PyObject* self, PyObject* args);


/* Python Initialisation */
PyMODINIT_FUNC initcmod(void);


/* Begin Python module setup */


/* Any Go function that intends to be called from Python has a partner C 
 * function wrapper to interface Go types with the Python-C API.
 *
 * The members of this structure correspond to:
 *
 * {
 *      "module_name",
 *      C_function_pointer_for_method,
 *      VARIABLE_ARGS_FLAG
 *      "Python documentation string, NULL if unwanted",
 * }, 
 * */
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


/* End Python module setup */


#endif

