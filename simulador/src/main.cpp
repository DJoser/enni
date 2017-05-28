#include "enni.h"

//------------------------------------------------------------------------------------
// Data
Canvas::Ptr canvas;

PhysicsWorld::Ptr world;
SceneManager::Ptr sceneManager;
scene::Node::Ptr root = nullptr;
scene::Node::Ptr camera = nullptr;

// Robot
// Cargar Robot
const std::string ROBOT = std::string("asset/robot/Tyson/tyson.dae");

// Root
scene::Node::Ptr Rob = nullptr;
scene::Node::Ptr Base = nullptr;
scene::Node::Ptr Cabeza = nullptr;
scene::Node::Ptr Extensor = nullptr;
scene::Node::Ptr Piso = nullptr;
scene::Node::Ptr Rotor = nullptr;

//------------------------------------------------------------------------------------
// Operadores C Api
void app_default_script();

void camera_rotar();
void camera_zoom();
void camera_move();

void world_load_model();
void world_play();
void world_pause();
void world_set_gravity();
void world_show_debug_data();

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
			->set({{ "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) }}),
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

void input_map_key();

void assets_find();

//------------------------------------------------------------------------------------
void defaulLoader_complete(file::Loader::Ptr loader)
{
	// Cargar default shader
	sceneManager->assets()->loader()->options()->effect(
		sceneManager->assets()->effect("effect/Phong.effect")
	);

	// Agregar Camara
	camera = scene::Node::create("camera")
		->addComponent(Renderer::create(0x7f7f7fff))
		->addComponent(Transform::create(
			math::inverse(math::lookAt(math::vec3(5.f, 1.5f, 5.f), math::vec3(), math::vec3(0.f, 1.f, 0.f))
			)))
		->addComponent(Camera::create(math::perspective(.785f, canvas->aspectRatio(), 0.1f, 1000.f)));
	root->addChild(camera);

	scene_add_ligth();
	scene_load_plane("prueba");
	scene_load_robot();
	html_load_page("prueba");

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
	PyRun_SimpleString(
		"import enni\n"
		"print(enni.zen())\n"
		"n = enni.Noddy()\n"
		"print(n)\n"
		"help(n)\n"
	);
	canvas->run();
	if(Py_FinalizeEx() < 0) exit(120);

	return EXIT_SUCCESS;
}