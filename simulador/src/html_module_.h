#pragma once
#include "common.h"
//------------------------------------------------------------------------------------------
// HTML Data
//------------------------------------------------------------------------------------------
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
	overlay->load(uri);
}


//------------------------------------------------------------------------------------------
// Py Html Module
//------------------------------------------------------------------------------------------
typedef struct {
	PyObject_HEAD
	PyObject *first; /* first name */
	PyObject *last;  /* last name */
	int number;
} Html;

static void
Html_dealloc(Html* self)
{
	Py_XDECREF(self->first);
	Py_XDECREF(self->last);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Html_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Html *self;

	self = (Html *)type->tp_alloc(type, 0);
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

static int Html_init(Html *self, PyObject *args, PyObject *kwds)
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


static PyMemberDef Html_members[] = {
	{ "first", T_OBJECT_EX, offsetof(Html, first), 0,"first name" },
	{ "last", T_OBJECT_EX, offsetof(Html, last), 0,"last name" },
	{ "number", T_INT, offsetof(Html, number), 0,"noddy number" },
	{ NULL }  /* Sentinel */
};

static PyObject * Html_name(Html* self)
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

static PyObject* Html_loadPage(PyObject *self, PyObject *args)
{
	const char *uri;
	if (!PyArg_ParseTuple(args, "s", &uri))
		return NULL;

	html_load_page(std::string(uri));
	return PyUnicode_FromString("html cargado");
}

static PyMethodDef Html_methods[] = {
	{ "name", (PyCFunction)Html_name, METH_NOARGS,"Return the name, combining the first and last name" },
	{ "loadPage", Html_loadPage, METH_VARARGS,"Load a web page in the front of camera" },
	{ NULL }  /* Sentinel */
};

static PyTypeObject HtmlType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"enni.Html",             /* tp_name */
	sizeof(Html),             /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)Html_dealloc, /* tp_dealloc */
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
	"Html objects",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	Html_methods,             /* tp_methods */
	Html_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)Html_init,      /* tp_init */
	0,                         /* tp_alloc */
	Html_new,                 /* tp_new */
};