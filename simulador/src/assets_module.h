#pragma once
#include "common.h"
//------------------------------------------------------------------------------------------
// Assets Data
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
// C API Assets
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
// Assets eventos
//------------------------------------------------------------------------------------------
void defaulLoader_complete(file::Loader::Ptr loader)
{
	
}
//------------------------------------------------------------------------------------------
// Py Html Module
//------------------------------------------------------------------------------------------
typedef struct {
	PyObject_HEAD
		PyObject *first; /* first name */
	PyObject *last;  /* last name */
	int number;
} Assets;

static void
Assets_dealloc(Assets* self)
{
	Py_XDECREF(self->first);
	Py_XDECREF(self->last);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Assets_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Assets *self;

	self = (Assets *)type->tp_alloc(type, 0);
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

static int Assets_init(Assets *self, PyObject *args, PyObject *kwds)
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


static PyMemberDef Assets_members[] = {
	{ "first", T_OBJECT_EX, offsetof(Assets, first), 0,"first name" },
	{ "last", T_OBJECT_EX, offsetof(Assets, last), 0,"last name" },
	{ "number", T_INT, offsetof(Assets, number), 0,"Assets number" },
	{ NULL }  /* Sentinel */
};

static PyObject * Assets_name(Assets* self)
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

static PyMethodDef Assets_methods[] = {
	{ "name", (PyCFunction)Assets_name, METH_NOARGS,"Return the name, combining the first and last name" },
	{ NULL }  /* Sentinel */
};

static PyTypeObject AssetsType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"enni.Assets",             /* tp_name */
	sizeof(Assets),             /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)Assets_dealloc, /* tp_dealloc */
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
	"Assets objects",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	Assets_methods,             /* tp_methods */
	Assets_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)Assets_init,      /* tp_init */
	0,                         /* tp_alloc */
	Assets_new,                 /* tp_new */
};
