#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif

#include <Python.h>

#ifdef _DEBUG_WAS DEFINED
#define _DEBUG 1
#endif

PyObject* some_function(PyObject* self, PyObject* args) {
	int i;
	if (PyArg_ParseTuple(args, "i", &i))
		return PyInt_FromLong(i+1);
	else
		return 0;
}

PyMethodDef SpamMethods[] =
{
	{"add_one", (PyCFunction) some_function, METH_VARARGS, 0},
	{0,0,0,0}
};

PyMODINIT_FUNC initspam(void)
{
	PyObject *m;

	m = Py_InitModule("spam", SpamMethods);
	if (m == NULL)
		return;
}