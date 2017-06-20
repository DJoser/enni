#include "enni.h"

void canvas_enterFrame(AbstractCanvas::Ptr canvas, float time, float deltaTime, bool visible)
{
	sceneManager->nextFrame(time, deltaTime);
};
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
	//----------------------------------------------------------------------------------
	// Nodos
	root = scene::Node::create("root")
		->addComponent(sceneManager)
		->addComponent(world)
		->addComponent(overlay);
	//----------------------------------------------------------------------------------
	// Conectar eventos
	auto a = overlay->onload()->connect(overlay_onload);
	auto c = canvas->keyboard()->keyDown()->connect(keyboard_keyDown);
	auto d = canvas->enterFrame()->connect(canvas_enterFrame);

	Py_Initialize();
	FILE* file;
	file = fopen("./asset/config/init.py", "r");
	PyRun_SimpleFile(file, "./config/init.py");
	fclose(file);
	canvas->run();

	if(Py_FinalizeEx() < 0) exit(120);

	return EXIT_SUCCESS;
}