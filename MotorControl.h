/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
 #include <stdint.h>

class MotorControl{
	public:
		 MotorControl (uint32_t motorId);
		 uint32_t getMotorId();
		 int32_t getPosition();
		 void updatePositionOneStep(bool rotationIsCW);
		 void setCurrentPositionAsLimit(bool maxLimitElseMin);
		 void resetLimit(bool maxLimitElseMin);
		 bool belowLimitMinimum();
		 bool aboveLimitMaximum();
		 bool withinRange();

	private:
		 uint32_t motorId;
		 int32_t position;
		 int32_t limitMinimum;
		 int32_t limitMaximum;
};

#endif
