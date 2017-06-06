#pragma once
#include "common.h"
//------------------------------------------------------------------------------------------
// Input Data
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// C API Input
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// Input eventos
//------------------------------------------------------------------------------------------
void keyboard_keyDown(input::Keyboard::Ptr k) {
	// Control Camara
	auto transform = camera->component<Transform>();
	if (k->keyIsDown(input::Keyboard::A)) {
		transform->matrix(translate(math::vec3(-.1f, 0.f, 0.f)) * transform->matrix());
	}
	if (k->keyIsDown(input::Keyboard::D)) {
		transform->matrix(translate(math::vec3(.1f, 0.f, 0.f)) * transform->matrix());
	}
	if (k->keyIsDown(input::Keyboard::DOWN)) {
		transform->matrix(translate(math::vec3(0.f, -.1f, 0.f)) * transform->matrix());
	}
	if (k->keyIsDown(input::Keyboard::UP)) {
		transform->matrix(translate(math::vec3(0.f, .1f, 0.f)) * transform->matrix());
	}
	if (k->keyIsDown(input::Keyboard::W)) {
		transform->matrix(translate(math::vec3(0.f, 0.f, -.1f)) * transform->matrix());
	}
	if (k->keyIsDown(input::Keyboard::S)) {
		transform->matrix(translate(math::vec3(0.f, 0.f, .1f)) * transform->matrix());
	}
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
		auto programName = std::string("Py_Keyboard");
		size_t size;

		Py_SetProgramName(Py_DecodeLocale(programName.c_str(), &size));

		PyRun_SimpleString(
			"from time import time,ctime\n"
			"print('Today is', ctime(time()))\n"
		);
		std::cout << Py_EncodeLocale(Py_GetPath(), &size) << std::endl;
	}
	if (k->keyIsDown(input::Keyboard::U)) {
		PyRun_SimpleString(
			"import enni\n"
			"print(enni.zen())"
		);
	}
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

static int Input_init(Input *self, PyObject *args, PyObject *kwds)
{
	PyObject *first = NULL, *last = NULL, *tmp;

	static char *kwlist[] = { "first", "last", "number", NULL };

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOi", kwlist,
		&first, &last,
		&self->number))
		return -1;

	if (first) {
		tmp = self->first;
		Py_INCREF(first);
		self->first = first;
		Py_XDECREF(tmp);
	}

	if (last) {
		tmp = self->last;
		Py_INCREF(last);
		self->last = last;
		Py_XDECREF(tmp);
	}

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

