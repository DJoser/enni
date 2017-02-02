#pragma once
#include "IRobotController.h" 

class RobotVirtual : public IRobotController
{
private:
	

public:
	typedef std::shared_ptr<RobotVirtual> Ptr;

	RobotVirtual()
	{

	}
	~RobotVirtual() {

	}

	void ClearPositionX() {};
	void MoveInitialX(){};
	void MoveFinalX(){};
	double PositionX() { return 0; };
	void PositionX(double valor) { };

	void ClearPositionY() {};
	void MoveInitialY() {};
	void MoveFinalY() {};
	double PositionY() { return 0; };
	void PositionY(double valor) { };

	void ClearPositionZ() {};
	void MoveInitialZ() {};
	void MoveFinalZ() {};
	double PositionZ() { return 0; };
	void PositionZ(double valor) { };
};

