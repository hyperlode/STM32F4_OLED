/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <stdint.h>
#include <stdio.h>
/*
#define LED_LIMIT_MIN 0
#define LED_LIMIT_MAX 1
#define LED_WITHIN_RANGE 2
#define LED_ENABLE 3
#define LED_ROTATING_RIGHT 4
#define LED_ROTATING_LEFT 5
*/
#define MODE_NORMAL 0
#define MODE_TEST 1  //shows which limits are configured.
#define MODE_CALIBRATE 2

#define RESET_VALUE_LIMIT_MINIMUM -2147483648
#define RESET_VALUE_LIMIT_MAXIMUM 2147483647
#define POSITION_ZERO_DEFAULT_MARGIN  0

#define CALIBRATION_SELECTED_LIMIT_MIN 1
#define CALIBRATION_SELECTED_LIMIT_MAX 2
#define CALIBRATION_SELECTED_LIMIT_NONE 0

typedef enum
{
	LED_LIMIT_MIN = 0,
	LED_LIMIT_MAX = 1,
	LED_WITHIN_RANGE = 2,
	LED_ENABLE = 3,
	LED_ROTATING_RIGHT = 4,
	LED_ROTATING_LEFT = 5
} StatusLed_TypeDef;




class MotorControl{
	public:

		 MotorControl ();
		 void init(uint32_t motorId);
		 uint32_t getMotorId();
		 void setMode(uint8_t mode);
		 uint8_t getMode();

		 int32_t getPosition();
		 void updatePositionOneStep(bool rotationIsCW);
		 void setCurrentPositionAsLimit();
		 void resetLimit();
		 void resetPositionAndLimits();
		 void resetPosition();

		 void setZeroPositionMargin(uint32_t margin);
		 bool getPositionAtZero();
		 void setCurrentPositionToZero();


		 int32_t getLimit(bool maxLimitElseMin);
		 bool belowLimitMinimum();
		 bool aboveLimitMaximum();
		 bool withinRange();

		 void selectLimitToBeCalibrated(int8_t selectLimit);
		 int8_t getSelectedLimitForCalibration();
		 bool getStatusLed(uint8_t led, uint32_t millis);


	private:
		 uint8_t mode;
		 uint32_t motorId;
		 int32_t position;
		 int32_t limitMinimum;
		 int32_t limitMaximum;
		 uint32_t zeroPositionMargin;
		 bool zeroingAxisHappenedAtLeastOnce;
		 //bool selectedLimitForCalibrationIsMax;
		 int8_t calibrationSelectedLimit;

};

#endif
