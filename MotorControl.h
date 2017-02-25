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
#define MODE_TEST 1
#define MODE_CALIBRATE 2

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

		 MotorControl (uint32_t motorId);
		 uint32_t getMotorId();
		 int32_t getPosition();
		 void updatePositionOneStep(bool rotationIsCW);
		 void setCurrentPositionAsLimit();
		 void resetLimit();
		 void resetPositionAndLimits();
		 void resetPosition();
		 uint32_t getLimit(bool maxLimitElseMin);
		 bool belowLimitMinimum();
		 bool aboveLimitMaximum();
		 bool withinRange();


		 void setMode(uint8_t mode);
		 uint8_t getMode();
		 void toggleLimitToBeCalibrated();
		 bool getStatusLed(uint8_t led, uint32_t millis);


	private:
		 uint8_t mode;
		 uint32_t motorId;
		 int32_t position;
		 int32_t limitMinimum;
		 int32_t limitMaximum;
		 bool selectedLimitForCalibrationIsMax;

};

#endif
