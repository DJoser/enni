#include "enni.h"
//------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//---------------------------------------------------------------------------------
	// Inicializar la interfaz del programa
	PyImport_AppendInittab("enni", &PyInit_enni);

	//---------------------------------------------------------------------------------
	// Interfaz grafica HTML
	overlay = HtmlOverlay::create(argc, argv);

	//---------------------------------------------------------------------------------
	// Simulador Fisica (Bullet)
	world = bullet::PhysicsWorld::create();
	world->paused(true);

	//---------------------------------------------------------------------------------
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
	defaultLoader->load();

	Py_Initialize();

	size_t size;
	auto programName = std::string("enni_init");
	Py_SetProgramName(Py_DecodeLocale(programName.c_str(), &size));

	FILE* file;
	file = fopen("./asset/config/init.py", "r");
	PyRun_SimpleFile(file, "./config/init.py");
	canvas->run();
	if(Py_FinalizeEx() < 0) exit(120);

	return EXIT_SUCCESS;
}