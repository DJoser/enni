#pragma once
#include "common.h"

//------------------------------------------------------------------------------------------
// C API PyNode
//------------------------------------------------------------------------------------------
//void PyNode_add_cube(scene::PyNode::Ptr PyNode) {
//	PyNode->addComponent(Surface::create(
//		geometry::CubeGeometry::create(sceneManager->assets()->context()),
//		material::Material::create()->set({
//			{ "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) }
//	}),
//		sceneManager->assets()->effect("effect/Phong.effect")
//	));
//}
//void PyNode_add_directionalLigth(scene::PyNode::Ptr PyNode) {
//	PyNode->addComponent(DirectionalLight::create())
//		->addComponent(AmbientLight::create());
//}
//void PyNode_add_camera(scene::PyNode::Ptr PyNode) {
//	PyNode->addComponent(Renderer::create(0x7f7f7fff))
//		->addComponent(Camera::create(
//			math::perspective(
//				.785f,
//				sceneManager->canvas()->aspectRatio(),
//				0.1f, 1000.f)
//		));
//}
//void PyNode_transform_rotate(scene::PyNode::Ptr PyNode) {
//	PyNode->component<Transform>()->matrix(
//		PyNode->component<Transform>()->matrix() *
//		math::rotate(.01f, math::vec3(0.f, 1.f, 0.f))
//	);
//}
//void PyNode_transform_lookat(scene::PyNode::Ptr PyNode, math::vec3 target) {
//	PyNode->component<Transform>()->matrix(
//		math::inverse(math::lookAt(
//			target,
//			math::vec3(0.f, 0.f, 0.f),
//			math::vec3(0.f, 1.f, 0.f)
//		))
//	);
//}
//------------------------------------------------------------------------------------------
typedef struct {
	PyObject_HEAD
	PyObject *name;
	PyObject *components;
} PyNode;

static void
PyNode_dealloc(PyNode* self)
{
	Py_XDECREF(self->components);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
PyNode_new(PyTypeObject *type, PyObject *args)
{
	PyNode* self = (PyNode *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->name = PyUnicode_FromString("");
		if (self->name == NULL) {
			Py_DECREF(self);
			return NULL;
		}

		self->components = PyTuple_New(0);
		if (self->components == NULL) {
			Py_DECREF(self);
			return NULL;
		}
	}

	return (PyObject *)self;
}

static int PyNode_init(PyNode *self, PyObject *args)
{
	PyObject *name = NULL, *components = NULL, *tmp;

	if (!PyArg_ParseTuple(args, "OO",  &name, &components))
		return -1;

	if (name) {
		tmp = self->name;
		Py_INCREF(name);
		self->name = name;
		Py_XDECREF(tmp);
	}

	if (components) {
		tmp = self->components;
		Py_INCREF(components);
		self->components = components;
		Py_XDECREF(tmp);
	}
	return 0;
}


static PyMemberDef PyNode_members[] = {
	{ "name", T_STRING , offsetof(PyNode, name), 0,"PyNode name" },
	{ "components", T_OBJECT_EX , offsetof(PyNode, components), 0,"list of PyNode components" },
	{ NULL }  /* Sentinel */
};

static PyObject * PyNode_addComponent(PyNode* self,PyNode* args)
{
	if (self->name == NULL) {
		PyErr_SetString(PyExc_AttributeError, "name");
		return NULL;
	}
	return PyUnicode_FromFormat("%S", self->name);
}

static PyMethodDef PyNode_methods[] = {
	{ "addComponent", (PyCFunction)PyNode_addComponent, METH_VARARGS,"add a new component to the PyNode" },
	{ NULL }  /* Sentinel */
};

static PyTypeObject PyNodeType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"enni.PyNode",				/* tp_name		*/
	sizeof(PyNode),				/* tp_basicsize	*/
	0,								/* tp_itemsize		*/
	(destructor)PyNode_dealloc,	/* tp_dealloc	*/
	0,								/* tp_print			*/
	0,								/* tp_getattr		*/
	0,								/* tp_setattr		*/
	0,								/* tp_reserved		*/
	0,								/* tp_repr			*/
	0,								/* tp_as_number		*/
	0,								/* tp_as_sequence	*/
	0,								/* tp_as_mapping		*/
	0,								/* tp_hash			*/
	0,								/* tp_call			*/
	0,								/* tp_str			*/
	0,								/* tp_getattro		*/
	0,								/* tp_setattro		*/
	0,								/* tp_as_buffer		*/
	Py_TPFLAGS_DEFAULT |
	Py_TPFLAGS_BASETYPE,			/* tp_flags */
	"PyNode objects",			/* tp_doc */
	0,								/* tp_traverse */
	0,								/* tp_clear */
	0,								/* tp_richcompare */
	0,								/* tp_weaklistoffset */
	0,								/* tp_iter */
	0,								/* tp_iternext */
	PyNode_methods,				/* tp_methods */
	PyNode_members,				/* tp_members */
	0,								/* tp_getset */
	0,								/* tp_base */
	0,								/* tp_dict */
	0,								/* tp_descr_get */
	0,								/* tp_descr_set */
	0,								/* tp_dictoffset */
	(initproc)PyNode_init,		/* tp_init */
	0,								/* tp_alloc */
	(newfunc)PyNode_new					/* tp_new */
};
