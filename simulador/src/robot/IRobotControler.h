#pragma once
class IRobotController
{
public:
	virtual void AbstractMemberFunction() = 0;
	virtual double GetPositionX() = 0;

};

