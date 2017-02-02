#pragma once
#include <iostream>
#include "openFrameworks\ofArduino.h"

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

	public:
		void configurar() {

		}
	};
	typedef std::shared_ptr<Module> ModulePtr;

	ModulePtr Mx = nullptr;
	ModulePtr My = nullptr;
	ModulePtr Mz = nullptr;


public:
	Robot() {
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

