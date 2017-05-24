#include "minko/Minko.hpp"
#include "minko/MinkoSDL.hpp"
#include "minko/MinkoHtmlOverlay.hpp"
#include "minko/MinkoBullet.hpp"
#include "minko/MinkoASSIMP.hpp"
#include "minko/MinkoJPEG.hpp"
#include "minko/MinkoPNG.hpp"
#include "Python.h"

using namespace minko;
using namespace minko::component;
using namespace minko::component::bullet;

// Constantes
CONST std::string TITULO_VENTANA = "E.N.N.I.";

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
//------------------------------------------------------------------------------------
// Data
Canvas::Ptr canvas;

HtmlOverlay::Ptr overlay;
PhysicsWorld::Ptr world;
SceneManager::Ptr sceneManager;

scene::Node::Ptr root = nullptr;
scene::Node::Ptr plano = nullptr;
scene::Node::Ptr camera = nullptr;
//------------------------------------------------------------------------------------
// Operadores C Api

// Cargar Robot
const std::string ROBOT = std::string("asset/robot/Tyson/tyson.dae");
// Root
scene::Node::Ptr Rob = nullptr;

// Partes
scene::Node::Ptr Base = nullptr;
scene::Node::Ptr Cabeza = nullptr;
scene::Node::Ptr Extensor = nullptr;
scene::Node::Ptr Piso = nullptr;
scene::Node::Ptr Rotor = nullptr;
//------------------------------------------------------------------------------------
// Operadores Python Apy
static PyObject* enni_zen(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return PyUnicode_FromString(
		"1. Integridad : Los guerreros se comprometen plenamente con sus decisiones.\n"
		"2. Respeto : La verdadera fuerza de un guerrero aparece en tiempos de dificultad, ellos no necesitan probar su fuerza.\n"
		"3. Coraje : El coraje heroico no es ciego, es inteligente y fuerte.\n"
		"4. Honor : No te puedes esconder de ti mismo.\n"
		"5. Compasion : Si la oportunidad de mejorar no aparece, sal del camino a buscar una.\n"
		"6. Sinceridad : Hablar y hacer es la misma accion.\n"
		"7. Deber y Lealtad	: Los guerreros son responsables de que han dicho y an hecho asi como sus consecuencias.\n"
	);
}
static PyObject* enni_arbol(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return PyUnicode_FromString("arbol");
}
//------------------------------------------------------------------------------------
// pyConfig.h
static PyMethodDef EnniMethods[] = {
	{ "zen", enni_zen, METH_VARARGS, "Retorna el Zen de E.N.N.I." },
	{ NULL, NULL, 0, NULL }
};

static PyModuleDef EnniModule = {
	PyModuleDef_HEAD_INIT, "enni", NULL, -1, EnniMethods,NULL, NULL, NULL, NULL
};

static PyObject* PyInit_enni(void)
{
	return PyModule_Create(&EnniModule);
}
//------------------------------------------------------------------------------------
void defaulLoader_complete(file::Loader::Ptr loader)
{
	// Cargar default shader
	sceneManager->assets()->loader()->options()->effect(
		sceneManager->assets()->effect("effect/Phong.effect")
	);

	// Crear el plano de simulacion
	float GROUND_WIDTH = 5.f;
	float GROUND_DEPTH = 5.f;
	float GROUND_THICK = 0.1f;
	auto planoMatrix = math::scale(math::vec3(GROUND_WIDTH, GROUND_THICK, GROUND_DEPTH)) * math::mat4();
	plano = scene::Node::create("plano")
		->addComponent(Transform::create(planoMatrix))
		->addComponent(Surface::create(
			geometry::CubeGeometry::create(sceneManager->assets()->context()),
			material::Material::create()->set({
				{ "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) }
	}),
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

	// Agregar Camara
	camera = scene::Node::create("camera")
		->addComponent(Renderer::create(0x7f7f7fff))
		->addComponent(Transform::create(
			math::inverse(math::lookAt(math::vec3(5.f, 1.5f, 5.f), math::vec3(), math::vec3(0.f, 1.f, 0.f))
			)))
		->addComponent(Camera::create(math::perspective(.785f, canvas->aspectRatio(), 0.1f, 1000.f)));
	root->addChild(camera);

	// Agregar Camara
	auto lights = scene::Node::create("lights")
		->addComponent(DirectionalLight::create())
		->addComponent(AmbientLight::create())
		->addComponent(Transform::create(math::inverse(math::lookAt(
			math::vec3(0.f, 2.f, 5.f),
			math::vec3(0.f, 0.f, 0.f),
			math::vec3(0.f, 1.f, 0.f)
		))));
	root->addChild(lights);


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

void overlay_onload(minko::dom::AbstractDOM::Ptr dom, std::string page)
{
	if (!dom->isMain())
		return;

	// Objetos del simulador
	//gameInterfaceDom = dom;
	tituloPagina = dom::AbstractDOMElement::Ptr(dom->getElementById("logo-container").get());
	objectTree = dom::AbstractDOMElement::Ptr(dom->getElementById("objectTree").get());
	objectProperty = dom::AbstractDOMElement::Ptr(dom->getElementById("objectProperty").get());

	btnControlLeft = dom::AbstractDOMElement::Ptr(dom->getElementById("menuControl").get());
	btnControlLeft->onclick()->connect([=](dom::AbstractDOMMouseEvent::Ptr event)
	{
		//tituloPagina->textContent("Control Cliked");
	});

	onclickSlot = dom->document()->onclick()->connect([=](dom::AbstractDOMMouseEvent::Ptr event)
	{
		//tituloPagina->textContent("Clicked");
	});

	onmessage = dom->onmessage()->connect([=](dom::AbstractDOM::Ptr dom, std::string string) {
		std::cout << "Ejecutar codigo: "<< std::endl << string <<std::endl;
		PyRun_SimpleString(string.c_str());
	});
}

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
		auto programName  = std::string("Py_Keyboard");
		size_t size;

		Py_SetProgramName(Py_DecodeLocale(programName.c_str(), &size));


		PyRun_SimpleString(
			"from time import time,ctime\n"
			"print('Today is', ctime(time()))\n"
		);
		std::cout << Py_EncodeLocale(Py_GetPath(),&size) << std::endl;
	}
	if (k->keyIsDown(input::Keyboard::U)) {
		PyRun_SimpleString(
			"import enni\n"
			"print(enni.zen())"
		);
	}
}

void canvas_enterFrame(AbstractCanvas::Ptr canvas, float time, float deltaTime, bool visible)
{
	sceneManager->nextFrame(time, deltaTime);
};
//------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	// Inicializar la interfaz del programa
	PyImport_AppendInittab("enni", &PyInit_enni);

	//----------------------------------------------------------------------------------
	// Interfaz grafica HTML
	overlay = HtmlOverlay::create(argc, argv);

	// Simulador Fisica (Bullet)
	world = bullet::PhysicsWorld::create();
	world->paused(true);

	// Canvas de dibujo
	canvas = Canvas::create(TITULO_VENTANA, 960, 540);
	sceneManager = SceneManager::create(canvas);
	auto assets = sceneManager->assets();
	auto defaultLoader = sceneManager->assets()->loader();
	auto defaultOptions = defaultLoader->options();
	//----------------------------------------------------------------------------------
	// Nodos
	root = scene::Node::create("root")
		->addComponent(sceneManager)
		->addComponent(world)
		->addComponent(overlay);
	//----------------------------------------------------------------------------------
	// Cargar Assets
	defaultOptions
		->generateMipmaps(true)
		->registerParser<file::OBJParser>("obj")
		->registerParser<file::ColladaParser>("dae")
		->registerParser<file::BlenderParser>("blend")
		->registerParser<file::PNGParser>("png")
		->registerParser<file::JPEGParser>("jpg");

	defaultLoader
		->queue("effect/Line.effect")
		->queue("effect/Phong.effect")
		->queue("effect/Basic.effect")
		->queue(ROBOT);

	// Conectar eventos
	auto a = overlay->onload()->connect(overlay_onload);
	auto b = defaultLoader->complete()->connect(defaulLoader_complete);
	auto c = canvas->keyboard()->keyDown()->connect(keyboard_keyDown);
	auto d = canvas->enterFrame()->connect(canvas_enterFrame);

	// Inciar el programa
	overlay->load("html/interface.html");
	defaultLoader->load();
	//defaulLoader_complete(defaultLoader);

	Py_Initialize();
	canvas->run();
	if(Py_FinalizeEx() < 0) exit(120);

	return EXIT_SUCCESS;
}