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
class RobotReal : public IRobotController
{
private:
	class Module {
	private:
		int _minPosition = 0;
		int _maxPosition = 65535;
		int _currentPosition = 0;
		ModuloCfg _configuracion;
	public:
		typedef std::shared_ptr<Module> Ptr;
		Module(ModuloCfg cfg) {
			_configuracion = cfg;
		}
		void configurar(ofArduino arduino) {
			arduino.sendDigitalPinMode(_configuracion.f_carrea1, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.f_carrea2, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.motor_negativo, ARD_OUTPUT);
			arduino.sendDigitalPinMode(_configuracion.motor_positivo, ARD_OUTPUT);
			arduino.sendDigitalPinMode(_configuracion.b0, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b1, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b2, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b3, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b4, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b5, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b6, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b7, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b8, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b9, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b10, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b11, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b12, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b13, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b14, ARD_INPUT);
			arduino.sendDigitalPinMode(_configuracion.b15, ARD_INPUT);
		}

		int getEncoder(ofArduino arduino) {
			int b0 = arduino.getDigital(_configuracion.b0) == 1 ? pow(2, 0) : 0;
			int b1 = arduino.getDigital(_configuracion.b1) == 1 ? pow(2, 1) : 0;
			int b2 = arduino.getDigital(_configuracion.b2) == 1 ? pow(2, 2) : 0;
			int b3 = arduino.getDigital(_configuracion.b3) == 1 ? pow(2, 3) : 0;
			int b4 = arduino.getDigital(_configuracion.b4) == 1 ? pow(2, 4) : 0;
			int b5 = arduino.getDigital(_configuracion.b5) == 1 ? pow(2, 5) : 0;
			int b6 = arduino.getDigital(_configuracion.b6) == 1 ? pow(2, 6) : 0;
			int b7 = arduino.getDigital(_configuracion.b7) == 1 ? pow(2, 7) : 0;
			int b8 = arduino.getDigital(_configuracion.b8) == 1 ? pow(2, 8) : 0;
			int b9 = arduino.getDigital(_configuracion.b9) == 1 ? pow(2, 9) : 0;
			int b10 = arduino.getDigital(_configuracion.b10) == 1 ? pow(2, 10) : 0;
			int b11 = arduino.getDigital(_configuracion.b11) == 1 ? pow(2, 11) : 0;
			int b12 = arduino.getDigital(_configuracion.b12) == 1 ? pow(2, 12) : 0;
			int b13 = arduino.getDigital(_configuracion.b13) == 1 ? pow(2, 13) : 0;
			int b14 = arduino.getDigital(_configuracion.b14) == 1 ? pow(2, 14) : 0;
			int b15 = arduino.getDigital(_configuracion.b15) == 1 ? pow(2, 15) : 0;
			return (b0 + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + b10 + b11 + b12 + b13 + b14 + b15);
		}

		void EncenderMotor1(ofArduino arduino) {
			arduino.sendDigital(_configuracion.motor_positivo, 1);
			arduino.sendDigital(_configuracion.motor_negativo, 0);
		}

		void ApagarMotor1(ofArduino arduino) {
			arduino.sendDigital(_configuracion.motor_positivo, 0);
			arduino.sendDigital(_configuracion.motor_negativo, 0);
		}

		void EncenderMotor2(ofArduino arduino) {
			arduino.sendDigital(_configuracion.motor_positivo, 0);
			arduino.sendDigital(_configuracion.motor_negativo, 1);
		}

		void ApagarMotor2(ofArduino arduino) {
			arduino.sendDigital(_configuracion.motor_positivo, 0);
			arduino.sendDigital(_configuracion.motor_negativo, 0);
		}

		bool LeerFinal(ofArduino arduino) {
			bool valor = arduino.getDigital(_configuracion.f_carrea2) == 0 ? true : false;
			return valor;
		}

		bool LeerInicio(ofArduino arduino) {
			bool valor = arduino.getDigital(_configuracion.f_carrea1) == 0 ? true : false;
			return valor;
		}
	};
	
	bool _robotConected = true;
	ofArduino _arduino;

	Module::Ptr Mx = nullptr;
	Module::Ptr My = nullptr;
	Module::Ptr Mz = nullptr;

	int _positionX = 0;
	int _positionY = 0;
	int _positionZ = 0;

public:
	typedef std::shared_ptr<RobotReal> Ptr;
	
	RobotReal(ModuloCfg Modx, ModuloCfg Mody, ModuloCfg Modz) {
		Mx = Module::Ptr(new Module(Modx));
		My = Module::Ptr(new Module(Mody));
		Mz = Module::Ptr(new Module(Modz));
	}
	
	~RobotReal()
	{

	}

	bool conectar(std::string Puerto, int baudRate = 57600) {
		_arduino.connect(Puerto,baudRate);

		if (_arduino.isArduinoReady()) {
			Mx->configurar(_arduino);
			//My->configurar(_arduino);
			//Mz->configurar(_arduino);
		}

		return _arduino.isArduinoReady();
	}

	void ClearPositionX() { _positionX = 0; };
	void MoveInitialX() {
		if (!Mx->LeerInicio(_arduino)) {
			Mx->EncenderMotor2(_arduino);
			while (!Mx->LeerInicio(_arduino));
			Mx->ApagarMotor2(_arduino);
		}
	};
	void MoveFinalX() {
		if (!Mx->LeerFinal(_arduino)) {
			Mx->EncenderMotor1(_arduino);
			while (!Mx->LeerFinal(_arduino));
			Mx->ApagarMotor1(_arduino);
		}
	};
	double PositionX() { return 0; };
	void PositionX(double valor) { 
		int encoder = Mx->getEncoder(_arduino);
		if (encoder != valor)
		{
			if (encoder > valor)
				Mx->EncenderMotor2(_arduino);
			if (encoder < valor)
				Mx->EncenderMotor1(_arduino);

			while (encoder != valor)
			{
				encoder = Mx->getEncoder(_arduino);
				if (encoder > valor) {
					Mx->ApagarMotor1(_arduino);
					Mx->EncenderMotor2(_arduino);
				}
				if (encoder < valor){
					Mx->ApagarMotor2(_arduino);
					Mx->EncenderMotor1(_arduino);
				}
			}

			Mx->ApagarMotor1(_arduino);
			Mx->ApagarMotor2(_arduino);
		}
	};

	void ClearPositionY() { _positionY = 0; };
	void MoveInitialY() {
		if (!My->LeerInicio(_arduino)) {
			My->EncenderMotor2(_arduino);
			while (!My->LeerInicio(_arduino));
			My->ApagarMotor2(_arduino);
		}
	};
	void MoveFinalY() {
		if (!My->LeerFinal(_arduino)) {
			My->EncenderMotor1(_arduino);
			while (!My->LeerFinal(_arduino));
			My->ApagarMotor1(_arduino);
		}
	};
	double PositionY() { return 0; };
	void PositionY(double valor) {
		int encoder = My->getEncoder(_arduino);
		if (encoder != valor)
		{
			if (encoder > valor)
				My->EncenderMotor2(_arduino);
			if (encoder < valor)
				My->EncenderMotor1(_arduino);

			while (encoder != valor)
			{
				encoder = My->getEncoder(_arduino);
				if (encoder > valor) {
					My->ApagarMotor1(_arduino);
					My->EncenderMotor2(_arduino);
				}
				if (encoder < valor) {
					My->ApagarMotor2(_arduino);
					My->EncenderMotor1(_arduino);
				}
			}

			My->ApagarMotor1(_arduino);
			My->ApagarMotor2(_arduino);
		}
	};

	void ClearPositionZ() { _positionZ = 0; };
	void MoveInitialZ() {
		if (!Mz->LeerInicio(_arduino)) {
			Mz->EncenderMotor2(_arduino);
			while (!Mz->LeerInicio(_arduino));
			Mz->ApagarMotor2(_arduino);
		}
	};
	void MoveFinalZ() {
		if (!Mz->LeerFinal(_arduino)) {
			Mz->EncenderMotor1(_arduino);
			while (!Mz->LeerFinal(_arduino));
			Mz->ApagarMotor1(_arduino);
		}
	};
	double PositionZ() { return 0; };
	void PositionZ(double valor) {
		int encoder = Mz->getEncoder(_arduino);
		if (encoder != valor)
		{
			if (encoder > valor)
				Mz->EncenderMotor2(_arduino);
			if (encoder < valor)
				Mz->EncenderMotor1(_arduino);

			while (encoder != valor)
			{
				encoder = Mz->getEncoder(_arduino);
				if (encoder > valor) {
					Mz->ApagarMotor1(_arduino);
					Mz->EncenderMotor2(_arduino);
				}
				if (encoder < valor) {
					Mz->ApagarMotor2(_arduino);
					Mz->EncenderMotor1(_arduino);
				}
			}

			Mz->ApagarMotor1(_arduino);
			Mz->ApagarMotor2(_arduino);
		}
	};
};

