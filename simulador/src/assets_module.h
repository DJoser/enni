#pragma once
#include "common.h"
//------------------------------------------------------------------------------------------
// Assets Data
//------------------------------------------------------------------------------------------
bool loaderComplete = true;
minko::file::Loader::Ptr loader;
//------------------------------------------------------------------------------------------
// Assets eventos
//------------------------------------------------------------------------------------------
Signal<file::Loader::Ptr>::Slot  eventLoaderComplete;
//------------------------------------------------------------------------------------------
// C API Assets
//------------------------------------------------------------------------------------------
void assets_init() {
	loader = sceneManager->assets()->loader();
	// Cargar Assets
	loader->options()
		->generateMipmaps(true)
		->registerParser<file::OBJParser>("obj")
		->registerParser<file::ColladaParser>("dae")
		->registerParser<file::BlenderParser>("blend")
		->registerParser<file::PNGParser>("png")
		->registerParser<file::JPEGParser>("jpg");

	eventLoaderComplete = loader->complete()->connect([&](file::Loader::Ptr loader)
	{
		loaderComplete = true;
	});
}
void assets_queue(std::string file) {
	loaderComplete = false;
	loader->queue(file);
}
void assets_load() {
	loader->load();
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
// TODO:
// Generar la estructura para los tipos de assets
// -Shaders
// -Objects
// -Images
// -Etc

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
		assets_init();
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

static PyObject * Assets_queue(Assets* self, PyObject* args)
{
	const char *file;
	if (!PyArg_ParseTuple(args, "s", &file))
		return NULL;
	
	assets_queue(std::string(file));
	return PyUnicode_FromString(file);
}
static PyObject * Assets_load(Assets* self)
{
	assets_load();
	return PyUnicode_FromString("loading queue... ");
}


static PyMethodDef Assets_methods[] = {
	{ "queue", (PyCFunction)Assets_queue, METH_VARARGS,"Add a file to the load queue." },
	{ "load", (PyCFunction)Assets_load, METH_NOARGS ,"Start the load of the assets queue." },
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
