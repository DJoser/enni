#pragma once
class IRobotController
{
public:
	virtual void ClearPositionX() = 0;
	virtual void MoveInitialX() = 0;
	virtual void MoveFinalX() = 0;
	virtual double PositionX() = 0;
	virtual void PositionX(double valor) = 0;

	virtual void ClearPositionY() = 0;
	virtual void MoveInitialY() = 0;
	virtual void MoveFinalY() = 0;
	virtual double PositionY() = 0;
	virtual void PositionY(double valor) = 0;

	virtual void ClearPositionZ() = 0;
	virtual void MoveInitialZ() = 0;
	virtual void MoveFinalZ() = 0;
	virtual double PositionZ() = 0;
	virtual void PositionZ(double valor) = 0;

};

