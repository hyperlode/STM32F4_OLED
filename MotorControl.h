/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
 #include <stdint.h>

class MotorControl{
	public:
		 MotorControl (uint32_t motorId);
		 uint32_t getMotorId();


	private:
		 uint32_t motorId;

};

#endif
