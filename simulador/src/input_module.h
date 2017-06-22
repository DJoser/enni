#pragma once
#include "common.h"
//------------------------------------------------------------------------------------------
// Input Data
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
// Input eventos
//------------------------------------------------------------------------------------------
Signal<minko::input::Keyboard::Ptr>::Slot keydown;
//------------------------------------------------------------------------------------------
// C API Input
//------------------------------------------------------------------------------------------
void keyboard_keyDown(input::Keyboard::Ptr k) {

	if (k->keyIsDown(input::Keyboard::ESCAPE)) {
		canvas->quit();
	}
	if (k->keyIsDown(input::Keyboard::M)) {
		world->paused(false);
	}

	// Cambio de robot
	if (k->keyIsDown(input::Keyboard::M)) {
		tituloPagina->textContent(TITULO_VENTANA + " : Real");
	}
	if (k->keyIsDown(input::Keyboard::N)) {
		tituloPagina->textContent(TITULO_VENTANA + " : Virtual");
	}
	if (k->keyIsDown(input::Keyboard::Y)) {
		PyRun_SimpleString(
			"from time import time,ctime\n"
			"print('Today is', ctime(time()))\n"
		);
	}
	if (k->keyIsDown(input::Keyboard::Z)) {
		PyRun_SimpleString(
			"print(enni.zen())"
		);
	}
	if (k->keyIsDown(input::Keyboard::H)) {
		size_t size;
		auto programName = std::string("__debug__");
		Py_SetProgramName(Py_DecodeLocale(programName.c_str(), &size));

		FILE* file;
		file = fopen("./asset/config/debug.py", "r");
		PyRun_SimpleFile(file, "./config/debug.py");
		fclose(file);
	}
}
void input_init() {
	keydown = canvas->keyboard()->keyDown()->connect(keyboard_keyDown);
}

//------------------------------------------------------------------------------------------
// Py Input Module
//------------------------------------------------------------------------------------------

typedef struct {
	PyObject_HEAD
	PyObject *first; /* first name */
	PyObject *last;  /* last name */
	int number;
} Input;

static void
Input_dealloc(Input* self)
{
	Py_XDECREF(self->first);
	Py_XDECREF(self->last);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Input_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Input *self;

	self = (Input *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->first = PyUnicode_FromString("");
		if (self->first == NULL) {
			Py_DECREF(self);
			return NULL;
		}

		self->last = PyUnicode_FromString("");
		if (self->last == NULL) {
			Py_DECREF(self);
			return NULL;
		}

		self->number = 0;
	}

	return (PyObject *)self;
}

static int Input_init(Input *self)
{
	input_init();
	return 0;
}

static PyMemberDef Input_members[] = {
	{ "first", T_OBJECT_EX, offsetof(Input, first), 0,"first name" },
	{ "last", T_OBJECT_EX, offsetof(Input, last), 0,"last name" },
	{ "number", T_INT, offsetof(Input, number), 0,"Input number" },
	{ NULL }  /* Sentinel */
};

static PyObject * Input_name(Input* self)
{
	if (self->first == NULL) {
		PyErr_SetString(PyExc_AttributeError, "first");
		return NULL;
	}

	if (self->last == NULL) {
		PyErr_SetString(PyExc_AttributeError, "last");
		return NULL;
	}

	return PyUnicode_FromFormat("%S %S", self->first, self->last);
}

static PyMethodDef Input_methods[] = {
	{ "name", (PyCFunction)Input_name, METH_NOARGS,"Return the name, combining the first and last name" },
	{ NULL }  /* Sentinel */
};

static PyTypeObject InputType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"enni.Input",             /* tp_name */
	sizeof(Input),             /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)Input_dealloc, /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_reserved */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash  */
	0,                         /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
	Py_TPFLAGS_BASETYPE,   /* tp_flags */
	"Input objects",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	Input_methods,             /* tp_methods */
	Input_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)Input_init,      /* tp_init */
	0,                         /* tp_alloc */
	Input_new,                 /* tp_new */
};
