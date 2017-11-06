#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif

#include <Python.h>

#ifdef _DEBUG_WAS DEFINED
#define _DEBUG 1
#endif

#include <vector>
#include <math.h>

PyObject* sqrArr(PyObject* self, PyObject* args) {
	int len;
	std::vector<long> vect;
	PyObject* tuple;
	

	PyListObject* returnTuple;
	if (PyArg_ParseTuple(args, "O!", &PyList_Type, &tuple)){
		len = PyList_Size(tuple);
		vect = std::vector<long>(len);
		while (len--) {
			vect[len] = PyInt_AsLong(PyList_GetItem(tuple, len));
			vect[len] = pow(vect[len],2);
		}
		returnTuple = (PyListObject*)PyList_New(vect.size());
		for (int i = 0; i < vect.size(); i++) {
			PyList_SET_ITEM(returnTuple, i, PyInt_FromLong(vect[i]));
		}
		return (PyObject*)returnTuple;
	}
	else
		return 0;
}

PyObject* some_function(PyObject* self, PyObject* args) {
	int i,j;
	if (PyArg_ParseTuple(args, "ii", &i, &j))
		return PyInt_FromLong(i+j);
	else
		return 0;
}

PyMethodDef SpamMethods[] =
{
	{ "add_one", (PyCFunction)some_function, METH_VARARGS, 0 },
	{ "square_array", (PyCFunction)sqrArr , METH_VARARGS, 0 },
	{0,0,0,0}
};

PyMODINIT_FUNC initspam(void)
{
	PyObject *m;

	m = Py_InitModule("spam", SpamMethods);
	if (m == NULL)
		return;
}