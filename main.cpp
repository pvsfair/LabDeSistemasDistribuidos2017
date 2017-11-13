#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <omp.h>

#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif

#include <Python.h>

#ifdef _DEBUG_WAS DEFINED
#define _DEBUG 1
#endif

#define ORDEM 10000
#define N_THREADS 1

std::chrono::system_clock::time_point inicio, fim;

#include <vector>
#include <math.h>
void imprimeMatriz(const std::vector<std::vector<double>> &matriz)
{
	for (auto &linha : matriz)
	{
		for (const double &coluna : linha)
			std::cout << coluna << " ";

		std::cout << std::endl;
	}
}

std::vector<std::vector<double>> criaMatriz(unsigned int ordem)
{
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed1);
	std::uniform_real_distribution<double> distribution(0.0, 100);

	std::vector<std::vector<double>> matriz(ordem);

	omp_set_num_threads(N_THREADS);

#pragma omp parallel for
	for (int i = 0; i < ordem; ++i)
	{
		auto &linha = matriz[i];
		for (int j = 0; j < ordem; ++j)
			linha.push_back(distribution(generator));
	}
	return matriz;
}

double somaElementosMatrizSequencial(const std::vector<std::vector<double>> &matriz)
{
	double soma = 0.0;


	for (auto &linha : matriz)
		for (const double &elem : linha)
			soma += elem;

	return soma;
}

double somaElementosMatrizConcorrente(const std::vector<std::vector<double>> &matriz)
{
	double soma = 0.0;

	omp_set_num_threads(N_THREADS);

#pragma omp parallel for reduction(+:soma)
	for (int i = 0; i < ORDEM; ++i)
		for (unsigned int j = 0; j < ORDEM; ++j)
			soma += matriz[i][j];

	return soma;
}

PyObject* matrixCalc(PyObject* self, PyObject* args) {
	int len;
	std::vector<std::vector<double>> vect;
	PyObject* listIn;


	std::cout << "debug monstro origins" << std::endl;
	PyObject* returnList;

	if (PyArg_ParseTuple(args, "O!", &PyList_Type, &listIn)) {

		len = PyList_Size(listIn);

		vect = std::vector<std::vector<double>>(len);

		std::cout << "debug monstro1" << std::endl;
		for (int i = 0; i < len; i++) {
			std::cout << "debug monstro - o retorno" << std::endl;
			PyObject* listInIn = PyList_GetItem(listIn, i);

			int lenIn = PyList_Size(listInIn);
			std::vector<double> inList(lenIn);
			for (int j = 0; j < lenIn; j++) {
				inList[j] = PyFloat_AsDouble(PyList_GetItem(listInIn, j));
			}
			vect[i] = inList;
		}

		returnList = PyList_New(vect.size());

		std::cout << "a vingança do debug monstro" << std::endl;

		for (int i = 0; i < vect.size(); i++) {
			PyObject* returnListIn = PyList_New(vect[i].size());
			for (int j = 0; j < vect[i].size(); j++) {
				PyList_SetItem(returnListIn, j, PyFloat_FromDouble(vect[i][j]));
			}
			PyList_SetItem(returnList, i, returnListIn);
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
			PyList_SetItem((PyObject*)returnTuple, i, PyFloat_FromDouble(vect[i]));
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

PyObject *somaMatrizSequencial(PyObject *self, PyObject *args)
{
	std::vector<std::vector<double>> matriz = criaMatriz(ORDEM);

	//imprimeMatriz(matriz);

	inicio = std::chrono::system_clock::now();
	double somaMatriz = somaElementosMatrizSequencial(matriz);
	fim = std::chrono::system_clock::now();

    std::cout << "Tempo de processamento sequencial: " << std::chrono::duration_cast< std::chrono::duration<double> >(fim - inicio).count();
    std::cout << std::endl << std::endl;

	return PyFloat_FromDouble(somaMatriz);
}

PyObject *somaMatrizConcorrente(PyObject *self, PyObject *args)
{
	std::vector<std::vector<double>> matriz = criaMatriz(ORDEM);

	//imprimeMatriz(matriz);

	inicio = std::chrono::system_clock::now();
	double somaMatriz = somaElementosMatrizConcorrente(matriz);
    fim = std::chrono::system_clock::now();

    std::cout << "Tempo de processamento concorrente: " << std::chrono::duration_cast< std::chrono::duration<double> >(fim - inicio).count();
    std::cout << std::endl << std::endl;
	return PyFloat_FromDouble(somaMatriz);
}

PyMethodDef SpamMethods[] =
	{
		{"add_one", (PyCFunction)some_function, METH_VARARGS, 0},
		{ "matrix_calc", (PyCFunction)matrixCalc, METH_VARARGS, 0 },
		{ "square_array", (PyCFunction)sqrArr , METH_VARARGS, 0 },
		{"somaSequencial", (PyCFunction)somaMatrizSequencial, METH_VARARGS, 0},
		{"somaConcorrente", (PyCFunction)somaMatrizConcorrente, METH_VARARGS, 0},
		{0, 0, 0, 0}
};

PyMODINIT_FUNC initspam(void)
{
	PyObject *m;

	m = Py_InitModule("spam", SpamMethods);
	if (m == NULL)
		return;
}
