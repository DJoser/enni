#pragma once
#include "common.h"
#include "noddy_module.h"
#include "scene_module.h"
#include "html_module_.h"
#include "physics_module.h"
#include "assets_module.h"

static PyObject* enni_zen(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return PyUnicode_FromString(
		"1. Integridad : Los guerreros se comprometen plenamente con sus decisiones.\n"
		"2. Respeto : La verdadera fuerza de un guerrero aparece en tiempos de dificultad, ellos no necesitan probar su fuerza.\n"
		"3. Coraje : El coraje heroico no es ciego, es inteligente y fuerte.\n"
		"4. Honor : No te puedes esconder de ti mismo.\n"
		"5. Compasion : Si la oportunidad de mejorar no aparece, sal del camino a buscar una.\n"
		"6. Sinceridad : Hablar y hacer es la misma accion.\n"
		"7. Deber y Lealtad	: Los guerreros son responsables de que han dicho y an hecho asi como sus consecuencias.\n"
	);
}
static PyMethodDef EnniMethods[] = {
	{ "zen", enni_zen, METH_VARARGS, "Retorna el Zen de E.N.N.I." },
	{ NULL, NULL, 0, NULL }
};
static PyModuleDef EnniModule = {
	PyModuleDef_HEAD_INIT, "enni", "API de E.N.N.I.", -1, EnniMethods,NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_enni(void)
{
	PyObject* m;

	if (PyType_Ready(&NoddyType) < 0)
		return NULL;
	if (PyType_Ready(&HtmlType) < 0)
		return NULL;
	if (PyType_Ready(&PhysicsType) < 0)
		return NULL;
	if (PyType_Ready(&SceneType) < 0)
		return NULL;
	if (PyType_Ready(&AssetsType) < 0)
		return NULL;

	m = PyModule_Create(&EnniModule);
	if (m == NULL)
		return NULL;

	Py_INCREF(&NoddyType);
	PyModule_AddObject(m, "Noddy", (PyObject *)&NoddyType);

	Py_INCREF(&HtmlType);
	PyModule_AddObject(m, "Html", (PyObject *)&HtmlType);

	Py_INCREF(&PhysicsType);
	PyModule_AddObject(m, "Physics", (PyObject *)&PhysicsType);

	Py_INCREF(&SceneType);
	PyModule_AddObject(m, "Scene", (PyObject *)&SceneType);

	Py_INCREF(&AssetsType);
	PyModule_AddObject(m, "Assets", (PyObject *)&AssetsType);

	return m;
}