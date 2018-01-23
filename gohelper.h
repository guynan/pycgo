
#ifndef __GO_HELPER_PYCGO__
#define __GO_HELPER_PYCGO__

/* C Scoped helper functions */
GoSlice _construct_go_int_slice(PyObject* list);
PyObject* _construct_pylist_from_goslice(struct unsafearray data);
uint64_t isPrime(Py_ssize_t s);

#endif

