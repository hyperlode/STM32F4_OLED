#include "MotorControl.h"

MotorControl::MotorControl(){
	//first thing to do: init
}

void MotorControl::init(uint32_t motorId){
	this->motorId = motorId;
	this->position = 0;
	this->mode = MODE_NORMAL;
	this->zeroPositionMargin = POSITION_ZERO_DEFAULT_MARGIN;
	//choose selected limit
	this->calibrationSelectedLimit = 0;//limit to be calibrated

	resetPositionAndLimits();
	this-> zeroingAxisHappenedAtLeastOnce = false;

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

void MotorControl::setZeroPositionMargin(uint32_t margin){
	//zero will be assumed from 0-margin to 0+margin.
	this->zeroPositionMargin = margin;
}

bool MotorControl::getPositionAtZero(){

	//zero is not only 0, it has a bit of play. defined in this->zeroPositionMargin
	return this->position <= this->zeroPositionMargin && this->position >= -this->zeroPositionMargin;
}

void MotorControl::setCurrentPositionToZero(){
	//resets the assumed position of the axis to zero
	this->position= 0;
	this->zeroingAxisHappenedAtLeastOnce=true;
}

bool MotorControl::getZeroingAxisHappenedAtLeastOnce(){
	return this->zeroingAxisHappenedAtLeastOnce;
}

void MotorControl::updatePositionOneStep(bool rotationIsCCW){
	if (rotationIsCCW){
		this->position++;
	}else{
		this->position--;
	}
}

void MotorControl::setCurrentPositionAsLimit(){

	if (getZeroingAxisHappenedAtLeastOnce()){
		if (this->calibrationSelectedLimit == CALIBRATION_SELECTED_LIMIT_MAX){
			this->limitMaximum = this->position;
		}else if (this->calibrationSelectedLimit == CALIBRATION_SELECTED_LIMIT_MIN) {
			this->limitMinimum = this->position;
		}
	}else{
		printf ("USER ASSERT ERROR: as long as the axis was never zeroed, setting a limit is not allowed\r\n");
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
	bool tmpSelectSaver = this->calibrationSelectedLimit; //save selected limit
	//reset limits
	this->calibrationSelectedLimit =CALIBRATION_SELECTED_LIMIT_MIN;//limit to be calibrated
	resetLimit(); //lower
	this->calibrationSelectedLimit =CALIBRATION_SELECTED_LIMIT_MAX;//limit to be calibrated
	resetLimit(); //upper

	this->calibrationSelectedLimit = tmpSelectSaver;

	resetPosition();

}
void MotorControl::resetLimit(){
	if (this->calibrationSelectedLimit == CALIBRATION_SELECTED_LIMIT_MAX){
		this->limitMaximum = RESET_VALUE_LIMIT_MAXIMUM;
	}else if (this->calibrationSelectedLimit == CALIBRATION_SELECTED_LIMIT_MIN) {
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

void MotorControl::selectLimitToBeCalibrated(int8_t selectLimit){
	//0 is no limit selected!
	//when limit calibration button is set, the limit selected here will be set.
	//this-> selectedLimitForCalibrationIsMax = !this-> selectedLimitForCalibrationIsMax;
	this->calibrationSelectedLimit = selectLimit;
}
int8_t MotorControl::getSelectedLimitForCalibration(){
	return this->calibrationSelectedLimit;
}

bool MotorControl::getStatusLed(uint8_t led, uint32_t millis){
	//this is purely led light output! not status! i.e. an led might blink, so it will be 0 or 1 time dependent, not status depended.
	//see defines for led numbers.
	//provide millis for blinking function

	bool blink1Hz = millis%1000 > 500; //do XOR (boolean != blink1Hz) with the other value (MUST BE BOOL see:normalize to boolean), this way, there will always be blinking
	bool blink2Hz = millis%500>250;
	switch (this->mode){
		case MODE_NORMAL:
			switch (led){
				case LED_LIMIT_MIN:

					if (getLimit(false) == RESET_VALUE_LIMIT_MINIMUM ){
						//blink fast when limit not set
						return blink2Hz;
					}else{
						return belowLimitMinimum(); //on when limit reached
					}

					break;
				case LED_LIMIT_MAX:
					if (getLimit(true) == RESET_VALUE_LIMIT_MAXIMUM ){
						return blink2Hz;
					}else{
						return aboveLimitMaximum();
					}
					break;
				case LED_WITHIN_RANGE:
					if (!getZeroingAxisHappenedAtLeastOnce()){
						return blink2Hz;
					}else if (getPositionAtZero()){
						return blink1Hz;
					}else{
						return withinRange();
					}
					break;
				case LED_ENABLE:
					return blink2Hz;
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



/*
	case MODE_TEST:
		switch (led){
			case LED_LIMIT_MIN:

				if (getLimit(false) == RESET_VALUE_LIMIT_MINIMUM ){
					return false;
				}else if (getLimit(false) > 0){
					return blink2Hz;
				}else{
					return true;
				}
				break;
			case LED_LIMIT_MAX:
				if (getLimit(true) == RESET_VALUE_LIMIT_MAXIMUM ){
					return false;
				}else if (getLimit(true) < 0){
					return blink2Hz;
				}else{
					return true;
				}
				break;


			case LED_WITHIN_RANGE:
				if (getLimit(false) > getLimit(true)){
					//error condition
					return blink2Hz;

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
		*/
	case MODE_CALIBRATE:
		switch (led){
			case LED_LIMIT_MIN:
				if (getSelectedLimitForCalibration() ==1){
					return belowLimitMinimum() != blink1Hz; //xor operation
				}else{
					return belowLimitMinimum();
				}

				//return belowLimitMinimum() &&!(!blink1Hz && !selectedLimitForCalibrationIsMax) || (!belowLimitMinimum() && !selectedLimitForCalibrationIsMax && blink1Hz);
				break;
			case LED_LIMIT_MAX:
				if (getSelectedLimitForCalibration() ==2){
					return aboveLimitMaximum() != blink1Hz;
				}else{
					return aboveLimitMaximum();
				}

				//return aboveLimitMaximum();
			///	return aboveLimitMaximum() &&!(!blink1Hz && selectedLimitForCalibrationIsMax) || (!aboveLimitMaximum() && selectedLimitForCalibrationIsMax && blink1Hz);

				break;
			case LED_WITHIN_RANGE:
				return withinRange();
				break;
			case LED_ENABLE:
				return blink1Hz;
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
