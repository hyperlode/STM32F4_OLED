#include "MotorControl.h"

MotorControl::MotorControl(uint32_t motorId){
	this->motorId = motorId;
	this->position = 0;
	resetLimit(true); //lower
	resetLimit(false); //upper
}

uint32_t MotorControl::getMotorId(){
	return this->motorId;
}

int32_t MotorControl::getPosition(){
	return this->position;
}

void MotorControl::updatePositionOneStep(bool rotationIsCCW){
	if (rotationIsCCW){
		this->position++;
	}else{
		this->position--;
	}
}
void MotorControl::setCurrentPositionAsLimit(bool maxLimitElseMin){
	if (maxLimitElseMin){
		this->limitMaximum = this->position;
	}else{
		this->limitMinimum = this->position;
	}
}

void MotorControl::resetLimit(bool maxLimitElseMin){
	if (maxLimitElseMin){
		this->limitMaximum = 2147483647;
	}else{
		this->limitMinimum = -2147483648;
	}
}

bool MotorControl::belowLimitMinimum(){
	return this->position < this->limitMinimum;
}

bool MotorControl::aboveLimitMaximum(){
	return this->position > this->limitMaximum;
}

bool MotorControl::withinRange(){
	return !belowLimitMinimum() && !aboveLimitMaximum();
}
