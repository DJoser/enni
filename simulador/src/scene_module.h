#pragma once
#include "common.h"
//------------------------------------------------------------------------------------------
// Scene Data
//------------------------------------------------------------------------------------------
scene::Node::Ptr root = nullptr;
//------------------------------------------------------------------------------------------
// Scene Events
//------------------------------------------------------------------------------------------
Signal<AbstractCanvas::Ptr, float, float, bool>::Slot enterFrame;
//------------------------------------------------------------------------------------------
// C API Scene
//------------------------------------------------------------------------------------------
void scene_init(std::string name) {
	canvas = Canvas::create(name, 960, 540);
	sceneManager = SceneManager::create(canvas);
	root = scene::Node::create("root")->addComponent(sceneManager);

	enterFrame = sceneManager->canvas()->enterFrame()->connect([&](AbstractCanvas::Ptr canvas, float time, float deltaTime, bool flag)
	{
		sceneManager->nextFrame(time, deltaTime);
	});
}
scene::Node::Ptr scene_create_node(std::string name) {
	auto node = scene::Node::create(name);
	node->addComponent(Transform::create());
	root->addChild(node);
	return node;
}
void scene_next_frame() {
	canvas->step();
}
//------------------------------------------------------------------------------------------
// Py Scene Module
//------------------------------------------------------------------------------------------
typedef struct {
	PyObject_HEAD
	PyObject *first;
	PyObject *last;
	int number;
} Scene;

static void
Scene_dealloc(Scene* self)
{
	Py_XDECREF(self->first);
	Py_XDECREF(self->last);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Scene_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Scene *self;

	self = (Scene *)type->tp_alloc(type, 0);
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
	scene_init(TITULO_VENTANA);
	return (PyObject *)self;
}

static int Scene_init(Scene *self, PyObject *args, PyObject *kwds)
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


static PyMemberDef Scene_members[] = {
	{ "first", T_OBJECT_EX, offsetof(Scene, first), 0,"first name" },
	{ "last", T_OBJECT_EX, offsetof(Scene, last), 0,"last name" },
	{ "number", T_INT, offsetof(Scene, number), 0,"Scene number" },
	{ NULL }  /* Sentinel */
};

static PyObject * Scene_crete_node(Scene* self,PyObject *args)
{
	const char *name;
	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	scene_create_node(std::string(name));
	return PyUnicode_FromString(name);

}

static PyObject * Scene_next_frame(Scene* Self) {
	scene_next_frame();
	return Py_None;
}

static PyMethodDef Scene_methods[] = {
	{ "createNode", (PyCFunction)Scene_crete_node, METH_VARARGS, "add a new node to the scene" },
	{ "nextFrame", (PyCFunction)Scene_next_frame, METH_NOARGS, "add a new node to the scene" },
	{ NULL }  /* Sentinel */
};

static PyTypeObject SceneType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"enni.Scene",             /* tp_name */
	sizeof(Scene),             /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)Scene_dealloc, /* tp_dealloc */
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
	"Scene objects",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	Scene_methods,             /* tp_methods */
	Scene_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)Scene_init,      /* tp_init */
	0,                         /* tp_alloc */
	Scene_new,                 /* tp_new */
};

