#pragma once
#include "common.h"
//------------------------------------------------------------------------------------------
// Scene Data
//------------------------------------------------------------------------------------------
SceneManager::Ptr sceneManager;
scene::Node::Ptr root = nullptr;
scene::Node::Ptr camera = nullptr;

// Robot
const std::string ROBOT = std::string("asset/robot/Tyson/tyson.dae");
scene::Node::Ptr Rob = nullptr;
scene::Node::Ptr Base = nullptr;
scene::Node::Ptr Cabeza = nullptr;
scene::Node::Ptr Extensor = nullptr;
scene::Node::Ptr Piso = nullptr;
scene::Node::Ptr Rotor = nullptr;

//------------------------------------------------------------------------------------------
// C API Scene
//------------------------------------------------------------------------------------------

void scene_load_plane(std::string name) {
	// Crear el plano de simulacion
	float GROUND_WIDTH = 5.f;
	float GROUND_DEPTH = 5.f;
	float GROUND_THICK = 0.1f;
	auto planoMatrix = math::scale(math::vec3(GROUND_WIDTH, GROUND_THICK, GROUND_DEPTH)) * math::mat4();
	auto plano = scene::Node::create("plano")
		->addComponent(Transform::create(planoMatrix))
		->addComponent(Surface::create(
			geometry::CubeGeometry::create(sceneManager->assets()->context()),
			material::Material::create()
			->set({ { "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) } }),
			sceneManager->assets()->effect("effect/Phong.effect")
		))
		->addComponent(bullet::Collider::create(
			bullet::ColliderData::create(
				0.f, // static object (no mass)
				bullet::BoxShape::create(GROUND_WIDTH * 0.5f, GROUND_THICK * 0.5f, GROUND_DEPTH * 0.5f)
			)
		))
		->addComponent(bullet::ColliderDebug::create(sceneManager->assets()));
	root->addChild(plano);
}
void scene_load_robot() {
	auto Rob = sceneManager->assets()->symbol(ROBOT);

	for (auto node : Rob->children())
	{
		std::string nombre = node->name();

		if (nombre == "Base") { Base = node; }
		if (nombre == "Cabeza") { Cabeza = node; }
		if (nombre == "Extensor") { Extensor = node; }
		if (nombre == "Piso") { Piso = node; }
		if (nombre == "Rotor") { Rotor = node; }
	}

	// Agregar el robot a la simulacion
	root->addChild(Rob);
}
void scene_add_ligth() {
	// Agregar Luces
	auto lights = scene::Node::create("lights")
		->addComponent(DirectionalLight::create())
		->addComponent(AmbientLight::create())
		->addComponent(Transform::create(math::inverse(math::lookAt(
			math::vec3(0.f, 2.f, 5.f),
			math::vec3(0.f, 0.f, 0.f),
			math::vec3(0.f, 1.f, 0.f)
		))));
	root->addChild(lights);
}
void scene_remove_element();
void scene_load_model();

//------------------------------------------------------------------------------------------
// Py Scene Module
//------------------------------------------------------------------------------------------
typedef struct {
	PyObject_HEAD
		PyObject *first; /* first name */
	PyObject *last;  /* last name */
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

static PyObject * Scene_name(Scene* self)
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

static PyMethodDef Scene_methods[] = {
	{ "name", (PyCFunction)Scene_name, METH_NOARGS,"Return the name, combining the first and last name" },
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

