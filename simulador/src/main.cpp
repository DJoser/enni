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

using namespace minko;
using namespace minko::component;

int main(int argc, char** argv)
{
	auto overlay = HtmlOverlay::create(argc, argv);
	auto world = bullet::PhysicsWorld::create();
	world->paused(true);

    auto canvas = Canvas::create("E.N.N.I.", 960, 540,Canvas::Flags::CHROMELESS);
    auto sceneManager = SceneManager::create(canvas);
    auto assets = sceneManager->assets();
    auto defaultLoader = sceneManager->assets()->loader();

	auto defaultOptions = defaultLoader->options();

	// setup assets
	defaultOptions
		->generateMipmaps(true)
		->skinningFramerate(60)
		->skinningMethod(SkinningMethod::HARDWARE)
		->startAnimation(true)
		->registerParser<file::OBJParser>("obj")
		->registerParser<file::ColladaParser>("dae")
		->registerParser<file::BlenderParser>("blend")
		->registerParser<file::PNGParser>("png")
		->registerParser<file::JPEGParser>("jpg");
	
    auto root = scene::Node::create("root")
        ->addComponent(sceneManager)
		->addComponent(world)
		->addComponent(overlay);

	auto fxLoader = file::Loader::create(defaultLoader)
		->queue("effect/Line.effect")
		->queue("effect/Phong.effect")
		->queue("effect/Basic.effect")
		->queue("robot/tyson.dae");

    scene::Node::Ptr plano = nullptr;
	scene::Node::Ptr cubo = nullptr;
	scene::Node::Ptr cubo2 = nullptr;
	scene::Node::Ptr camera = nullptr;

	auto fxComplete = fxLoader->complete()->connect([&](file::Loader::Ptr loader)
	{
		overlay->load("html/interface.html");
		defaultLoader->options()
			->effect(assets->effect("effect/Phong.effect"));

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

		cubo = scene::Node::create("cubo")
			->addComponent(Transform::create(math::translate(math::vec3(0.f, 1.f, 0.f)) * math::mat4()))
			->addComponent(Surface::create(
				geometry::CubeGeometry::create(assets->context()),
				material::Material::create()->set({
					{ "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) }
				}),
				assets->effect("effect/Phong.effect")
			));

		cubo->addComponent(bullet::Collider::create(
			bullet::ColliderData::create(
				1.f,
				bullet::BoxShape::create(0.5f, 0.5f, 0.5f)
			)
		));
		cubo->addComponent(bullet::ColliderDebug::create(assets));
		//root->addChild(cubo);

		cubo2 = scene::Node::create("cubo2")
			->addComponent(Transform::create(math::translate(math::vec3(0.f, 3.f, 0.f)) * math::mat4()))
			->addComponent(Surface::create(
				geometry::CubeGeometry::create(assets->context()),
				material::Material::create()->set({
					{ "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) }
				}),
				assets->effect("effect/Phong.effect")
			))->addComponent(bullet::Collider::create(
				bullet::ColliderData::create(
					1.f,
					bullet::BoxShape::create(0.5f, 0.5f, 0.5f)
				)
			))
			->addComponent(bullet::ColliderDebug::create(assets));
		//cubo->addChild(cubo2);

		//world->addConstraint(cubo->component<bullet::Collider>(), cubo2->component<bullet::Collider>());

		auto model = sceneManager->assets()->symbol("robot/tyson.dae");
		root->addChild(model);



        camera = scene::Node::create("camera")
            ->addComponent(PerspectiveCamera::create(canvas->aspectRatio()))
            ->addComponent(Renderer::create(0xdcdcdcff))
            ->addComponent(Transform::create(math::translate(math::vec3(0.f,2.5f,10.f)) * math::mat4()));
        root->addChild(camera);

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

		if (k->keyIsDown(input::Keyboard::U)) {
			ofArduino arduino;
			arduino.connect("COM7");
			arduino.sendDigitalPinMode(13, ARD_OUTPUT);
			arduino.sendDigital(13, ARD_HIGH);
		}
		if (k->keyIsDown(input::Keyboard::I)) {
			ofArduino arduino;
			arduino.connect("COM7");
			arduino.sendDigitalPinMode(13, ARD_OUTPUT);
			arduino.sendDigital(13, ARD_LOW);
		}
	});

    auto enterFrame = canvas->enterFrame()->connect([&](Canvas::Ptr canvas, float time, float deltaTime)
	{
        sceneManager->nextFrame(time, deltaTime);
    });

    fxLoader->load();
    canvas->run();

    return 0;
}
