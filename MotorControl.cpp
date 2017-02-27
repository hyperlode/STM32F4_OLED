#include "MotorControl.h"

MotorControl::MotorControl(uint32_t motorId){
	this->motorId = motorId;
	this->position = 0;
	this->mode = MODE_NORMAL;

	//choose selected limit
	this->selectedLimitForCalibrationIsMax =false;//limit to be calibrated

	resetPositionAndLimits();
}

void MotorControl::setMode(uint8_t mode){
	//see defines for different modes.
	this->mode = mode;
}

uint8_t MotorControl::getMode(){
	return this->mode;
}

uint32_t MotorControl::getMotorId(){
	return this->motorId;
}

void MotorControl::resetPosition(){
	this->position = 0;
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





void MotorControl::setCurrentPositionAsLimit(){
	if (selectedLimitForCalibrationIsMax){
		this->limitMaximum = this->position;
	}else{
		this->limitMinimum = this->position;
	}
}
int32_t MotorControl::getLimit(bool maxLimitElseMin){
	if (maxLimitElseMin){
		return this->limitMaximum;
	}else{
		return this->limitMinimum;
	}

}
void MotorControl::resetPositionAndLimits(){
	bool tmpSelectSaver = this->selectedLimitForCalibrationIsMax; //save selected limit
	//reset limits
	this->selectedLimitForCalibrationIsMax =false;//limit to be calibrated
	resetLimit(); //lower
	this->selectedLimitForCalibrationIsMax =true;//limit to be calibrated
	resetLimit(); //upper

	this->selectedLimitForCalibrationIsMax = tmpSelectSaver;

	resetPosition();

}
void MotorControl::resetLimit(){
	if (this-> selectedLimitForCalibrationIsMax){
		this->limitMaximum = RESET_VALUE_LIMIT_MAXIMUM;
	}else{
		this->limitMinimum = RESET_VALUE_LIMIT_MINIMUM;
	}
}



bool MotorControl::belowLimitMinimum(){
	return this->position <= this->limitMinimum;
}

bool MotorControl::aboveLimitMaximum(){
	return this->position >= this->limitMaximum;
}

bool MotorControl::withinRange(){
	return !belowLimitMinimum() && !aboveLimitMaximum();
}



void MotorControl::toggleLimitToBeCalibrated(){
	//when limit calibration button is set, the limit selected here will be set.
	this-> selectedLimitForCalibrationIsMax = !this-> selectedLimitForCalibrationIsMax;
}

bool MotorControl::getStatusLed(uint8_t led, uint32_t millis){
	//this is purely led light output! not status! i.e. an led might blink, so it will be 0 or 1 time dependent, not status depended.
	//see defines for led numbers.
	//provide millis for blinking function

	bool blink1Hz = millis%1000 > 500; //do XOR (boolean != blink1Hz) with the other value (MUST BE BOOL see:normalize to boolean), this way, there will always be blinking
	bool blinkHalfHz = millis%500>250;
	//uint8_t loddde = MODE_NORMAL;
	switch (this->mode){
		//	switch (loddde){
		case MODE_NORMAL:


			switch (led){
				case LED_LIMIT_MIN:
					return belowLimitMinimum();
					break;
				case LED_LIMIT_MAX:
					return aboveLimitMaximum();
					break;
				case LED_WITHIN_RANGE:
					return withinRange();
					break;
				case LED_ENABLE:
					break;
				case LED_ROTATING_RIGHT:
					break;
				case LED_ROTATING_LEFT:
					break;
				default:
					return false;
					break;
			}
			break;




	case MODE_TEST:
		switch (led){
			case LED_LIMIT_MIN:

				if (getLimit(false) == RESET_VALUE_LIMIT_MINIMUM ){
					return false;
				}else if (getLimit(false) > 0){
					return blinkHalfHz;
				}else{
					return true;
				}
				break;
			case LED_LIMIT_MAX:
				if (getLimit(true) == RESET_VALUE_LIMIT_MAXIMUM ){
					return false;
				}else if (getLimit(true) < 0){
					return blinkHalfHz;
				}else{
					return true;
				}
				break;

				break;
			case LED_WITHIN_RANGE:
				if (getLimit(false) > getLimit(true)){
					//error condition
					return blinkHalfHz;

				}else{
					return true;
				}
				//return withinRange()!= blink1Hz ;
				break;
			case LED_ENABLE:
				break;
			case LED_ROTATING_RIGHT:
				break;
			case LED_ROTATING_LEFT:
				break;
			default:
				return false;
				break;
		}
	case MODE_CALIBRATE:
		switch (led){
			case LED_LIMIT_MIN:
				return belowLimitMinimum() &&!(!blink1Hz && !selectedLimitForCalibrationIsMax) || (!belowLimitMinimum() && !selectedLimitForCalibrationIsMax && blink1Hz);
				break;
			case LED_LIMIT_MAX:
				//return aboveLimitMaximum();
				return aboveLimitMaximum() &&!(!blink1Hz && selectedLimitForCalibrationIsMax) || (!aboveLimitMaximum() && selectedLimitForCalibrationIsMax && blink1Hz);

				break;
			case LED_WITHIN_RANGE:
				return withinRange();
				break;
			case LED_ENABLE:
				break;
			case LED_ROTATING_RIGHT:
				break;
			case LED_ROTATING_LEFT:
				break;
			default:
				return false;
				break;
		}
	break;
	default:

		break;

	}



}
