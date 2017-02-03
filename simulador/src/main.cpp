#include "minko/Minko.hpp"
#include "minko/MinkoSDL.hpp"
#include "minko/MinkoHtmlOverlay.hpp"
#include "minko/MinkoBullet.hpp"
#include "minko/MinkoASSIMP.hpp"
#include "minko/MinkoJPEG.hpp"
#include "minko/MinkoPNG.hpp"
#include "SDL.h"
#include "openFrameworks/ofSerial.h"
#include "openFrameworks/ofArduino.h"
#include "robot/RobotReal.h"
#include "robot/RobotVirtual.h"


using namespace minko;
using namespace minko::component;

int main(int argc, char** argv)
{
	// Interfaz grafica HTML
	auto overlay = HtmlOverlay::create(argc, argv);

	// Simulador Fisica (Bullet)
	auto world = bullet::PhysicsWorld::create();
	world->paused(true);

	// Canvas de dibujo
	auto canvas = Canvas::create("E.N.N.I.", 960, 540, Canvas::Flags::CHROMELESS);
	auto sceneManager = SceneManager::create(canvas);
	auto assets = sceneManager->assets();
	auto defaultLoader = sceneManager->assets()->loader();
	auto defaultOptions = defaultLoader->options();

	// Cargar Assets
	defaultOptions
		->generateMipmaps(true)
		->registerParser<file::OBJParser>("obj")
		->registerParser<file::ColladaParser>("dae")
		->registerParser<file::BlenderParser>("blend")
		->registerParser<file::PNGParser>("png")
		->registerParser<file::JPEGParser>("jpg");

	auto fxLoader = file::Loader::create(defaultLoader)
		->queue("effect/Line.effect")
		->queue("effect/Phong.effect")
		->queue("effect/Basic.effect")
		->queue("robot/tyson.dae");

	// Variables de control
	bool modoCamara = true;
	RobotVirtual::Ptr robotVirtual = nullptr;
	RobotReal::Ptr robotReal = nullptr;


	// Nodos
	auto root = scene::Node::create("root")
		->addComponent(sceneManager)
		->addComponent(world)
		->addComponent(overlay);
	scene::Node::Ptr plano = nullptr;
	scene::Node::Ptr camera = nullptr;

	auto onloadSlot = overlay->onload()->connect([=](minko::dom::AbstractDOM::Ptr dom, std::string page)
	{
		auto filename = dom->fileName();

		if (!dom->isMain())
			return;

		auto tituloPagina = dom->getElementById("logo-container");
		tituloPagina->textContent("Hola mundo");

		auto contenido = dom->getElementById("logo-container")->textContent();
	});

	auto fxComplete = fxLoader->complete()->connect([&](file::Loader::Ptr loader)
	{
		// Cargar default shader
		defaultLoader->options()->effect(assets->effect("effect/Phong.effect"));

		// Cargar el robot Virtual
		robotVirtual = RobotVirtual::Ptr(new RobotVirtual(root));

		// Conectar robot Real
		ModuloCfg ModuloX, ModuloY, ModuloZ;
		RobotReal::Ptr robotReal = RobotReal::Ptr(new RobotReal(ModuloX, ModuloY, ModuloZ));

		// Crear el plano de simulacion
		float GROUND_WIDTH = 5.f;
		float GROUND_DEPTH = 5.f;
		float GROUND_THICK = 0.1f;
		auto planoMatrix = math::scale(math::vec3(GROUND_WIDTH, GROUND_THICK, GROUND_DEPTH)) * math::mat4();
		plano = scene::Node::create("plano")
			->addComponent(Transform::create(planoMatrix))
			->addComponent(Surface::create(
				geometry::CubeGeometry::create(assets->context()),
				material::Material::create()->set({
					{ "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) }
		}),
				assets->effect("effect/Phong.effect")
			))
			->addComponent(bullet::Collider::create(
				bullet::ColliderData::create(
					0.f, // static object (no mass)
					bullet::BoxShape::create(GROUND_WIDTH * 0.5f, GROUND_THICK * 0.5f, GROUND_DEPTH * 0.5f)
				)
			))
			->addComponent(bullet::ColliderDebug::create(assets));
		root->addChild(plano);

		// Agregar Luces
		camera = scene::Node::create("camera")
			->addComponent(PerspectiveCamera::create(canvas->aspectRatio()))
			->addComponent(Renderer::create(0xdcdcdcff))
			->addComponent(Transform::create(math::translate(math::vec3(0.f, 0.5f, 2.5f)) * math::mat4()));
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
	});

	auto keyDown = canvas->keyboard()->keyDown()->connect([&](input::Keyboard::Ptr k) {
		if (modoCamara)
		{
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
			if (k->keyIsDown(input::Keyboard::F)) {
				SDL_MaximizeWindow(canvas->window());
			}
			if (k->keyIsDown(input::Keyboard::P)) {
				world->paused(false);
			}


			// Control del robot
			if (k->keyIsDown(input::Keyboard::O)) {
				robotVirtual->MoveInitialX();
			}
			if (k->keyIsDown(input::Keyboard::L)) {
				robotVirtual->MoveFinalX();
			}
			if (k->keyIsDown(input::Keyboard::I)) {
				robotVirtual->MoveInitialY();
			}
			if (k->keyIsDown(input::Keyboard::K)) {
				robotVirtual->MoveFinalY();
			}
			if (k->keyIsDown(input::Keyboard::U)) {
				robotVirtual->MoveInitialZ();
			}
			if (k->keyIsDown(input::Keyboard::J)) {
				robotVirtual->MoveFinalZ();
			}

			// Prueba Arduino
			if (k->keyIsDown(input::Keyboard::Y)) {
				ofArduino arduino;
				arduino.connect("COM9");
				arduino.sendDigitalPinMode(4, ARD_OUTPUT);

				arduino.sendDigital(4, ARD_HIGH);
			}
			if (k->keyIsDown(input::Keyboard::H)) {
				ofArduino arduino;
				arduino.connect("COM9");
				arduino.sendDigitalPinMode(4, ARD_OUTPUT);
				arduino.sendDigital(4, ARD_LOW);
			}
		}
	});

	auto enterFrame = canvas->enterFrame()->connect([&](Canvas::Ptr canvas, float time, float deltaTime)
	{
		sceneManager->nextFrame(time, deltaTime);
	});

	overlay->load("html/interface.html");
	fxLoader->load();
	canvas->run();

	return 0;
}