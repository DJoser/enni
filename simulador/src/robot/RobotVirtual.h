#pragma once
#include "IRobotController.h"
#include "minko/Minko.hpp"

using namespace minko;
using namespace minko::component;

class RobotVirtual : public IRobotController
{
private:
	const std::string ROBOT = std::string("robot/tyson.dae");

	// Root
	scene::Node::Ptr Rob = nullptr;

	// Partes
	scene::Node::Ptr Base = nullptr;
	scene::Node::Ptr Cabeza = nullptr;
	scene::Node::Ptr Extensor = nullptr;
	scene::Node::Ptr Piso = nullptr;
	scene::Node::Ptr Rotor = nullptr;

	int posicion = 0;

public:
	typedef std::shared_ptr<RobotVirtual> Ptr;

	RobotVirtual(scene::Node::Ptr root)
	{
		// Agregar el robot
		auto assets = root->component<SceneManager>()->assets();
		Rob = assets->symbol(ROBOT);

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

		// TODO: Agregar propiedades fisicas
		//		- RigidBody
		//		- Join
		root->addComponent(bullet::Collider::create(
			bullet::ColliderData::create(
				1.f, // static object (no mass)
				bullet::BoxShape::create(.2f, .4f, .2f)
			)
		))->addComponent(bullet::ColliderDebug::create(assets));
	}
	~RobotVirtual() {}

	void ClearPositionX() {
		auto transform = Extensor->component<Transform>();
		transform->matrix(translate(math::vec3(0.f, .10f, 0.f)) * transform->matrix());
	};
	void MoveInitialX() {
		// Extraer
		auto transform = Extensor->component<Transform>();

		// Calcular
		auto dx = .01;
		auto X = transform->matrix();
		auto Xt = translate(math::vec3(dx, 0.f, 0.f));
		auto Xf = Xt * X;

		// Aplicar
		transform->matrix(Xf);
	};
	void MoveFinalX() {
		// Extraer
		auto transform = Extensor->component<Transform>();

		// Calcular
		auto dx = -.01;
		auto X = transform->matrix();
		auto Xt = translate(math::vec3(dx, 0.f, 0.f));
		auto Xf = Xt * X;

		// Aplicar
		transform->matrix(Xf);
	};
	double PositionX() { 
		return 0; 
	};
	void PositionX(double valor) { 
	};

	void ClearPositionY() {};
	void MoveInitialY() {
		// Extraer
		auto transformX = Extensor->component<Transform>();
		auto transformY = Cabeza->component<Transform>();

		// Calcular
		auto dy = .01;

		auto Xi = transformX->matrix();
		auto Yi = transformY->matrix();

		auto T = translate(math::vec3(0.f,  0.f, dy));

		auto Xf = T * Xi;
		auto Yf = T * Yi;

		// Aplicar
		transformX->matrix(Xf);
		transformY->matrix(Yf);
	};
	void MoveFinalY() {
		// Extraer
		auto transformX = Extensor->component<Transform>();
		auto transformY = Cabeza->component<Transform>();

		// Calcular
		auto dy = -.01;

		auto Xi = transformX->matrix();
		auto Yi = transformY->matrix();

		auto T = translate(math::vec3(0.f, 0.f, dy));

		auto Xf = T * Xi;
		auto Yf = T * Yi;

		// Aplicar
		transformX->matrix(Xf);
		transformY->matrix(Yf);
	};
	double PositionY() { return 0; };
	void PositionY(double valor) { };

	void ClearPositionZ() {};
	void MoveInitialZ() {};
	void MoveFinalZ() {};
	double PositionZ() { return 0; };
	void PositionZ(double valor) { };
};