#pragma once
#include <iostream>
#include "openFrameworks\ofArduino.h"

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
class Robot
{
private:
	bool _robotConected = true;
	std::shared_ptr<ofArduino> _arduino = nullptr;

	class Module {
	private:
		int _minPosition = 0;
		int _maxPosition = 65535;
		int _currentPosition = 0;
		ModuloCfg c_onfiguracion;
	public:
		Module(ModuloCfg cfg) {

		}
		void configurar() {

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
			Mx->configurar();

			My = ModulePtr();
			My->configurar();

			Mz = ModulePtr();
			Mz->configurar();
		}
	}

	double getPosition() {
		if (!_robotConected)
			return 0.0;	
	}
};

