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
void node_add_cube(scene::Node::Ptr node) {
	node->addComponent(Surface::create(
		geometry::CubeGeometry::create(sceneManager->assets()->context()),
		material::Material::create()->set({
			{ "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) }
	}),
		sceneManager->assets()->effect("effect/Phong.effect")
	));
}
void node_add_directionalLigth(scene::Node::Ptr node) {
	node->addComponent(DirectionalLight::create())
		->addComponent(AmbientLight::create());
}
void node_add_camera(scene::Node::Ptr node) {
	node->addComponent(Renderer::create(0x7f7f7fff))
		->addComponent(Camera::create(
			math::perspective(
				.785f,
				sceneManager->canvas()->aspectRatio(),
				0.1f, 1000.f)
		));
}
void node_transform_rotate(scene::Node::Ptr node) {
	node->component<Transform>()->matrix(
		node->component<Transform>()->matrix() *
		math::rotate(.01f, math::vec3(0.f, 1.f, 0.f))
	);
}
void node_transform_lookat(scene::Node::Ptr node, math::vec3 target) {
	node->component<Transform>()->matrix(
		math::inverse(math::lookAt(
			target,
			math::vec3(0.f, 0.f, 0.f),
			math::vec3(0.f, 1.f, 0.f)
		))
	);
}
//------------------------------------------------------------------------------------------
// Py Scene Module
//------------------------------------------------------------------------------------------
typedef struct {
	PyObject_HEAD
	PyObject *root;
} Scene;

static void
Scene_dealloc(Scene* self)
{
	Py_XDECREF(self->root);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Scene_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Scene *self;

	self = (Scene *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->root = PyUnicode_FromString("");
		if (self->root == NULL) {
			Py_DECREF(self);
			return NULL;
		}
	}
	scene_init(TITULO_VENTANA);
	return (PyObject *)self;
}

static int Scene_init(Scene *self, PyObject *args, PyObject *kwds)
{
	PyObject *root = NULL, *tmp;

	static char *kwlist[] = { "first", "last", "number", NULL };

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist,&root))
		return -1;

	if (root) {
		tmp = self->root;
		Py_INCREF(root);
		self->root = root;
		Py_XDECREF(tmp);
	}

	return 0;
}

static PyMemberDef Scene_members[] = {
	{ "root", T_OBJECT_EX, offsetof(Scene, root), 0,"root node in the node tree" },
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


//------------------------------------------------------------------------------------------
static minko::scene::Node::Ptr findNode(std::string name) {
	auto nodeset = scene::NodeSet::create(root)->descendants(true)->where([](scene::Node::Ptr node)
	{
		return true;
	});

	for (auto& node : nodeset->nodes()) {
		if (node->name().compare(name) == 0)
			return node;
	}

	return nullptr;
}


// Control de modulos del nodo
static PyObject *Node_add_cube(Scene* Self,PyObject* args) {
	node_add_cube(findNode("cube"));
	return Py_None;
}
static PyObject *Node_add_directionalLigth(Scene* Self) {
	auto light = findNode("light");
	node_add_directionalLigth(light);
	node_transform_lookat(light, math::vec3(0.f, 2.f, 5.f));
	return Py_None;
}
static PyObject *Node_add_camera(Scene* Self) {
	auto camera = findNode("camera");
	node_add_camera(camera);
	node_transform_lookat(camera, math::vec3(5.f, 1.5f, 5.f));
	return Py_None;
}
static PyObject *Node_transform_rotate(Scene* Self) {
	return Py_None;
}
static PyObject *Node_transform_lookat(Scene* Self) {
	return Py_None;
}

//------------------------------------------------------------------------------------------


static PyMethodDef Scene_methods[] = {
	{ "createNode", (PyCFunction)Scene_crete_node, METH_VARARGS, "add a new node to the scene" },
	{ "nextFrame", (PyCFunction)Scene_next_frame, METH_NOARGS, "render the next frame in the scene" },

	{ "nodeAddCube", (PyCFunction)Node_add_cube, METH_NOARGS, "TODO" },
	{ "nodeAddDirectionaLight", (PyCFunction)Node_add_directionalLigth, METH_NOARGS, "TODO" },
	{ "nodeAddCamera", (PyCFunction)Node_add_camera, METH_NOARGS, "TODO" },
	{ "nodeTransformRotate", (PyCFunction)Node_transform_rotate, METH_NOARGS, "TODO" },
	{ "nodeTransformLookat", (PyCFunction)Node_transform_lookat, METH_NOARGS, "TODO" },

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

