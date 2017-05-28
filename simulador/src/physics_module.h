#pragma once
#include "common.h"

//------------------------------------------------------------------------------------------
// Physics Data
//------------------------------------------------------------------------------------------
PhysicsWorld::Ptr world;

//------------------------------------------------------------------------------------------
// C API Html Physics
//------------------------------------------------------------------------------------------
void world_load_model();
void world_play();
void world_pause();
void world_set_gravity();
void world_show_debug_data();
//------------------------------------------------------------------------------------------
// Py Physics Module
//------------------------------------------------------------------------------------------
typedef struct {
	PyObject_HEAD
	PyObject *first; /* first name */
	PyObject *last;  /* last name */
	int number;
} Physics;

static void
Physics_dealloc(Physics* self)
{
	Py_XDECREF(self->first);
	Py_XDECREF(self->last);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Physics_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Physics *self;

	self = (Physics *)type->tp_alloc(type, 0);
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

static int Physics_init(Physics *self, PyObject *args, PyObject *kwds)
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


static PyMemberDef Physics_members[] = {
	{ "first", T_OBJECT_EX, offsetof(Physics, first), 0,"first name" },
	{ "last", T_OBJECT_EX, offsetof(Physics, last), 0,"last name" },
	{ "number", T_INT, offsetof(Physics, number), 0,"noddy number" },
	{ NULL }  /* Sentinel */
};

static PyObject * Physics_name(Physics* self)
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

static PyMethodDef Physics_methods[] = {
	{ "name", (PyCFunction)Physics_name, METH_NOARGS,"Return the name, combining the first and last name" },
	{ NULL }  /* Sentinel */
};

static PyTypeObject PhysicsType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"enni.Physics",             /* tp_name */
	sizeof(Physics),             /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)Physics_dealloc, /* tp_dealloc */
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
	"Physics objects",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	Physics_methods,             /* tp_methods */
	Physics_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)Physics_init,      /* tp_init */
	0,                         /* tp_alloc */
	Physics_new,                 /* tp_new */
};
