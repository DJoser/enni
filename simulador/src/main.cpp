#include "minko/Minko.hpp"
#include "minko/MinkoSDL.hpp"
#include "minko/MinkoHtmlOverlay.hpp"

using namespace minko;
using namespace minko::component;

int main(int argc, char** argv)
{
	auto overlay = HtmlOverlay::create(argc, argv);

    auto canvas = Canvas::create("E.N.N.I.", 960, 540);
    auto sceneManager = SceneManager::create(canvas);
    auto assets = sceneManager->assets();
    auto defaultLoader = sceneManager->assets()->loader();
    auto root = scene::Node::create("root")
        ->addComponent(sceneManager)
		->addComponent(overlay);

    auto fxLoader = file::Loader::create(defaultLoader)
        ->queue("effect/Phong.effect")
        ->queue("effect/Basic.effect");

    scene::Node::Ptr cube = nullptr;

	overlay->load("html/interface.html");

    auto fxComplete = fxLoader->complete()->connect([&](file::Loader::Ptr loader)
    {
        defaultLoader->options()
            ->effect(assets->effect("effect/Phong.effect"));

        cube = scene::Node::create("cube")
            ->addComponent(Transform::create())
            ->addComponent(Surface::create(
                geometry::CubeGeometry::create(assets->context()),
                material::Material::create()->set({
                    { "diffuseColor", math::vec4(.5f, .5f, .5f, 1.f) }
                }),
                assets->effect("effect/Phong.effect")
            ));
        root->addChild(cube);

        auto camera = scene::Node::create("camera")
            ->addComponent(PerspectiveCamera::create(canvas->aspectRatio()))
            ->addComponent(Renderer::create(0x7f7f7fff))
            ->addComponent(Transform::create(math::inverse(math::lookAt(
                math::vec3(2.f, 1.f, 2.f),
                math::vec3(0.f, 0.f, 0.f),
                math::vec3(0.f, 1.f, 0.f)
            ))));
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
		auto transform = cube->component<Transform>();

		if (k->keyIsDown(input::Keyboard::LEFT))
			transform->matrix(translate(math::vec3(-.1f, 0.f, 0.f)) * transform->matrix());
		if (k->keyIsDown(input::Keyboard::RIGHT))
			transform->matrix(translate(math::vec3(.1f, 0.f, 0.f)) * transform->matrix());
	});

    auto enterFrame = canvas->enterFrame()->connect([&](Canvas::Ptr canvas, float time, float deltaTime)
    {
        /*if (cube)
            cube->component<Transform>()->matrix(
                cube->component<Transform>()->matrix()
                * math::rotate(.01f, math::vec3(0.f, 1.f, 0.f))
            );*/

        sceneManager->nextFrame(time, deltaTime);
    });

    fxLoader->load();
    canvas->run();

    return 0;
}
