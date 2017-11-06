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

PyObject* matrixCalc(PyObject* self, PyObject* args) {
	int len;
	std::vector<std::vector<double>> vect;
	PyObject* listIn;


	PyObject* returnList;

	if (PyArg_ParseTuple(args, "O!", &PyList_Type, &listIn)) {

		len = PyList_Size(listIn);

		vect = std::vector<std::vector<double>>(len);

		for (int i = 0; i < len; i++) {
			PyObject* listInIn = PyList_GetItem(listIn, i);
			//if (!PyArg_ParseTuple(PyList_GetItem(listIn,i), "O!", &PyList_Type, &listInIn)) return NULL;

			int lenIn = PyList_Size(listInIn);
			std::vector<double> inList(lenIn);
			for (int j = 0; j < lenIn; j++) {
				inList[j] = PyFloat_AsDouble(PyList_GetItem(listInIn, j));
			}
			vect[i] = inList;
		}

		//while (len--) {
		//	vect[len] = PyFloat_AsDouble(PyList_GetItem(listIn, len));
		//	vect[len] = pow(vect[len], 2);
		//}



		returnList = PyList_New(vect.size());

		for (int i = 0; i < vect.size(); i++) {
			PyObject* returnListIn = PyList_New(vect[i].size());
			for (int j = 0; j < vect[i].size(); j++) {
				PyList_SET_ITEM(returnListIn, j, PyFloat_FromDouble(vect[i][j]));
			}
			PyList_SET_ITEM(returnList, i, returnListIn);
		}
		return (PyObject*)returnList;
	}
	else
		return 0;
}

PyObject* sqrArr(PyObject* self, PyObject* args) {
	int len;
	std::vector<double> vect;
	PyObject* listIn;


	PyListObject* returnTuple;

	if (PyArg_ParseTuple(args, "O!", &PyList_Type, &listIn)) {

		len = PyList_Size(listIn);

		vect = std::vector<double>(len);
		while (len--) {
			vect[len] = PyFloat_AsDouble(PyList_GetItem(listIn, len));
			vect[len] = pow(vect[len], 2);
		}



		returnTuple = (PyListObject*)PyList_New(vect.size());

		for (int i = 0; i < vect.size(); i++) {
			PyList_SET_ITEM(returnTuple, i, PyFloat_FromDouble(vect[i]));
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
	{ "matrix_calc", (PyCFunction)matrixCalc, METH_VARARGS, 0 },
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