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

		int getEncoder(ofArduino* arduino) {
			int b0 = arduino->getDigital(_configuracion.b0) == 1 ? pow(2, 0) : 0;
			int b1 = arduino->getDigital(_configuracion.b1) == 1 ? pow(2, 1) : 0;
			int b2 = arduino->getDigital(_configuracion.b2) == 1 ? pow(2, 2) : 0;
			int b3 = arduino->getDigital(_configuracion.b3) == 1 ? pow(2, 3) : 0;
			int b4 = arduino->getDigital(_configuracion.b4) == 1 ? pow(2, 4) : 0;
			int b5 = arduino->getDigital(_configuracion.b5) == 1 ? pow(2, 5) : 0;
			int b6 = arduino->getDigital(_configuracion.b6) == 1 ? pow(2, 6) : 0;
			int b7 = arduino->getDigital(_configuracion.b7) == 1 ? pow(2, 7) : 0;
			int b8 = arduino->getDigital(_configuracion.b8) == 1 ? pow(2, 8) : 0;
			int b9 = arduino->getDigital(_configuracion.b9) == 1 ? pow(2, 9) : 0;
			int b10 = arduino->getDigital(_configuracion.b10) == 1 ? pow(2, 10) : 0;
			int b11 = arduino->getDigital(_configuracion.b11) == 1 ? pow(2, 11) : 0;
			int b12 = arduino->getDigital(_configuracion.b12) == 1 ? pow(2, 12) : 0;
			int b13 = arduino->getDigital(_configuracion.b13) == 1 ? pow(2, 13) : 0;
			int b14 = arduino->getDigital(_configuracion.b14) == 1 ? pow(2, 14) : 0;
			int b15 = arduino->getDigital(_configuracion.b15) == 1 ? pow(2, 15) : 0;
			return (b0 + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + b10 + b11 + b12 + b13 + b14 + b15);
		}

		void EncenderMotor1(ofArduino* arduino) {
			arduino->sendDigital(_configuracion.motor_positivo, 1);
			arduino->sendDigital(_configuracion.motor_negativo, 0);
		}

		void ApagarMotor1(ofArduino* arduino) {
			arduino->sendDigital(_configuracion.motor_positivo, 0);
			arduino->sendDigital(_configuracion.motor_negativo, 0);
		}

		void EncenderMotor2(ofArduino* arduino) {
			arduino->sendDigital(_configuracion.motor_positivo, 0);
			arduino->sendDigital(_configuracion.motor_negativo, 1);
		}

		void ApagarMotor2(ofArduino* arduino) {
			arduino->sendDigital(_configuracion.motor_positivo, 0);
			arduino->sendDigital(_configuracion.motor_negativo, 0);
		}

		bool LeerFinal(ofArduino* arduino) {
			bool valor = arduino->getDigital(_configuracion.f_carrea2) == 0 ? true : false;
			return valor;
		}

		bool LeerInicio(ofArduino* arduino) {
			bool valor = arduino->getDigital(_configuracion.f_carrea1) == 0 ? true : false;
			return valor;
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
	int PositionX = 0;
	int PositionY = 0;
	int PositionZ = 0;

	void ClearPositionX() { PositionX = 0; };
	void MoveInitialX() {
		if (!Mx->LeerInicio(_arduino.get())) {
			Mx->EncenderMotor2(_arduino.get());
			while (!Mx->LeerInicio(_arduino.get()));
			Mx->ApagarMotor2(_arduino.get());
		}
	};
	void MoveFinalX() {
		if (!Mx->LeerFinal(_arduino.get())) {
			Mx->EncenderMotor1(_arduino.get());
			while (!Mx->LeerFinal(_arduino.get()));
			Mx->ApagarMotor1(_arduino.get());
		}
	};
	double PositionX() { return 0; };
	void PositionX(double valor) { 
		int encoder = Mx->getEncoder(_arduino.get());
		if (encoder != valor)
		{
			if (encoder > valor)
				Mx->EncenderMotor2(_arduino.get());
			if (encoder < valor)
				Mx->EncenderMotor1(_arduino.get());

			while (encoder != valor)
			{
				encoder = Mx->getEncoder(_arduino.get());
				if (encoder > valor) {
					Mx->ApagarMotor1(_arduino.get());
					Mx->EncenderMotor2(_arduino.get());
				}
				if (encoder < valor){
					Mx->ApagarMotor2(_arduino.get());
					Mx->EncenderMotor1(_arduino.get());
				}
			}

			Mx->ApagarMotor1(_arduino.get());
			Mx->ApagarMotor2(_arduino.get());
		}
	};

	void ClearPositionY() { PositionY = 0; };
	void MoveInitialY() {
		if (!My->LeerInicio(_arduino.get())) {
			My->EncenderMotor2(_arduino.get());
			while (!My->LeerInicio(_arduino.get()));
			My->ApagarMotor2(_arduino.get());
		}
	};
	void MoveFinalY() {
		if (!My->LeerFinal(_arduino.get())) {
			My->EncenderMotor1(_arduino.get());
			while (!My->LeerFinal(_arduino.get()));
			My->ApagarMotor1(_arduino.get());
		}
	};
	double PositionY() { return 0; };
	void PositionY(double valor) {
		int encoder = My->getEncoder(_arduino.get());
		if (encoder != valor)
		{
			if (encoder > valor)
				My->EncenderMotor2(_arduino.get());
			if (encoder < valor)
				My->EncenderMotor1(_arduino.get());

			while (encoder != valor)
			{
				encoder = My->getEncoder(_arduino.get());
				if (encoder > valor) {
					My->ApagarMotor1(_arduino.get());
					My->EncenderMotor2(_arduino.get());
				}
				if (encoder < valor) {
					My->ApagarMotor2(_arduino.get());
					My->EncenderMotor1(_arduino.get());
				}
			}

			My->ApagarMotor1(_arduino.get());
			My->ApagarMotor2(_arduino.get());
		}
	};

	void ClearPositionZ() { PositionZ = 0; };
	void MoveInitialZ() {
		if (!Mz->LeerInicio(_arduino.get())) {
			Mz->EncenderMotor2(_arduino.get());
			while (!Mz->LeerInicio(_arduino.get()));
			Mz->ApagarMotor2(_arduino.get());
		}
	};
	void MoveFinalZ() {
		if (!Mz->LeerFinal(_arduino.get())) {
			Mz->EncenderMotor1(_arduino.get());
			while (!Mz->LeerFinal(_arduino.get()));
			Mz->ApagarMotor1(_arduino.get());
		}
	};
	double PositionZ() { return 0; };
	void PositionZ(double valor) {
		int encoder = Mz->getEncoder(_arduino.get());
		if (encoder != valor)
		{
			if (encoder > valor)
				Mz->EncenderMotor2(_arduino.get());
			if (encoder < valor)
				Mz->EncenderMotor1(_arduino.get());

			while (encoder != valor)
			{
				encoder = Mz->getEncoder(_arduino.get());
				if (encoder > valor) {
					Mz->ApagarMotor1(_arduino.get());
					Mz->EncenderMotor2(_arduino.get());
				}
				if (encoder < valor) {
					Mz->ApagarMotor2(_arduino.get());
					Mz->EncenderMotor1(_arduino.get());
				}
			}

			Mz->ApagarMotor1(_arduino.get());
			Mz->ApagarMotor2(_arduino.get());
		}
	};
};

