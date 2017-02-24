#include "MotorControl.h"

MotorControl::MotorControl(uint32_t motorId){
	this->motorId = motorId;
}

uint32_t MotorControl::getMotorId(){
	return this->motorId;
}
