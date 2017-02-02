#pragma once
#include <iostream>
#include "openFrameworks\ofArduino.h"
#include "robot\IRobotController.h"

typedef struct ModuloCfg {
	int f_carrea1 = -1;
	int f_carrea2 = -1;
	int motor_negativo = -1;
	int motor_positivo = -1;
	int b0 = -1;
	int b1 = -1;
	int b2 = -1;
	int b3 = -1;
	int b4 = -1;
	int b5 = -1;
	int b6 = -1;
	int b7 = -1;
	int b8 = -1;
	int b9 = -1;
	int b10 = -1;
	int b11 = -1;
	int b12 = -1;
	int b13 = -1;
	int b14 = -1;
	int b15 = -1;
};
class Robot : public IRobotController
{
private:
	bool _robotConected = true;
	std::shared_ptr<ofArduino> _arduino = nullptr;

	class Module {
	private:
		int _minPosition = 0;
		int _maxPosition = 65535;
		int _currentPosition = 0;
		ModuloCfg _configuracion;
	public:
		Module(ModuloCfg cfg) {
			_configuracion = cfg;
		}
		void configurar(ofArduino* arduino) {
			arduino->sendDigitalPinMode(_configuracion.f_carrea1, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.f_carrea2, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.motor_negativo, ARD_OUTPUT);
			arduino->sendDigitalPinMode(_configuracion.motor_positivo, ARD_OUTPUT);
			arduino->sendDigitalPinMode(_configuracion.b0, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b1, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b2, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b3, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b4, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b5, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b6, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b7, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b8, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b9, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b10, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b11, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b12, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b13, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b14, ARD_INPUT);
			arduino->sendDigitalPinMode(_configuracion.b15, ARD_INPUT);
		}
	};
	typedef std::shared_ptr<Module> ModulePtr;

	ModulePtr Mx = nullptr;
	ModulePtr My = nullptr;
	ModulePtr Mz = nullptr;


public:
	typedef std::shared_ptr<Robot> Ptr;
	Robot(ModuloCfg Modx, ModuloCfg Mody, ModuloCfg Modz) {
		Mx = ModulePtr(new Module(Modx));
		My = ModulePtr(new Module(Mody));
		Mz = ModulePtr(new Module(Modz));
	}
	~Robot()
	{

	}

	bool conectar(std::string Puerto, int baudRate = 57600) {
		_arduino->connect("COM9",baudRate);

		if (_arduino->isArduinoReady()) {
			Mx = ModulePtr();
			Mx->configurar(_arduino.get());

			My = ModulePtr();
			My->configurar(_arduino.get());

			Mz = ModulePtr();
			Mz->configurar(_arduino.get());
		}
	}

	double getPosition() {
		if (!_robotConected)
			return 0.0;	
	}
	int initialPositionX = 0;
	int initialPositionY = 0;
	int initialPositionZ = 0;

	void ClearPositionX() { initialPositionX = 0; };
	void MoveInitialX() {};
	void MoveFinalX() {};
	double PositionX() { return 0; };
	void PositionX(double valor) { };

	void ClearPositionY() { initialPositionY = 0; };
	void MoveInitialY() {};
	void MoveFinalY() {};
	double PositionY() { return 0; };
	void PositionY(double valor) { };

	void ClearPositionZ() { initialPositionZ = 0; };
	void MoveInitialZ() {};
	void MoveFinalZ() {};
	double PositionZ() { return 0; };
	void PositionZ(double valor) { };
};

