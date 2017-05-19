#pragma once
#include "common.h"
//------------------------------------------------------------------------------------------
// HTML Data
HtmlOverlay::Ptr overlay;

// Interfaz Grafica
dom::AbstractDOM::Ptr gameInterfaceDom;
dom::AbstractDOMElement::Ptr tituloPagina;
dom::AbstractDOMElement::Ptr objectTree;
dom::AbstractDOMElement::Ptr objectProperty;
dom::AbstractDOMElement::Ptr btnControlLeft;

// Eventos interfaz
Signal<minko::dom::AbstractDOM::Ptr, std::string>::Slot onloadSlot;
Signal<minko::dom::AbstractDOMMouseEvent::Ptr>::Slot onclickSlot;
Signal<dom::AbstractDOM::Ptr, std::string>::Slot onmessage;

//------------------------------------------------------------------------------------------
// C API Html
//------------------------------------------------------------------------------------------
void html_map_event() {

}
void html_send_mesage() {

}
void html_load_page(std::string uri) {
	//overlay->load("html/interface.html");
	overlay->load(uri);
}


//------------------------------------------------------------------------------------------
// Py Html Module
//------------------------------------------------------------------------------------------
static PyObject* enni_html_loadPage(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return PyUnicode_FromString("prueba satisfactoria");
}

typedef struct {
	PyObject_HEAD
		PyObject *first; /* first name */
	PyObject *last;  /* last name */
	int number;
} Noddy;

static void
Noddy_dealloc(Noddy* self)
{
	Py_XDECREF(self->first);
	Py_XDECREF(self->last);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Noddy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Noddy *self;

	self = (Noddy *)type->tp_alloc(type, 0);
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

static int Noddy_init(Noddy *self, PyObject *args, PyObject *kwds)
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


static PyMemberDef Noddy_members[] = {
	{ "first", T_OBJECT_EX, offsetof(Noddy, first), 0,"first name" },
	{ "last", T_OBJECT_EX, offsetof(Noddy, last), 0,"last name" },
	{ "number", T_INT, offsetof(Noddy, number), 0,"noddy number" },
	{ NULL }  /* Sentinel */
};

static PyObject * Noddy_name(Noddy* self)
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

static PyMethodDef Noddy_methods[] = {
	{ "name", (PyCFunction)Noddy_name, METH_NOARGS,"Return the name, combining the first and last name" },
	{ NULL }  /* Sentinel */
};

static PyTypeObject NoddyType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"noddy.Noddy",             /* tp_name */
	sizeof(Noddy),             /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)Noddy_dealloc, /* tp_dealloc */
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
	"Noddy objects",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	Noddy_methods,             /* tp_methods */
	Noddy_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)Noddy_init,      /* tp_init */
	0,                         /* tp_alloc */
	Noddy_new,                 /* tp_new */
};