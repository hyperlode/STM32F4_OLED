#ifndef MACHINECONTROL_H
#define MACHINECONTROL_H
#include <stdint.h>
#include <stdio.h>

#include "IOBoard.h"
#include "MotorControl.h"


#define MODE_NORMAL 0
#define MODE_TEST 1
#define MODE_CALIBRATE 2

#define NUMBER_OF_MOTORS 2

#define LED_MOTOR_HOIST_LIMIT_MIN	5
#define LED_MOTOR_HOIST_LIMIT_MAX	7
#define LED_MOTOR_HOIST_INRANGE		6

#define LED_MOTOR_CROWD_LIMIT_MIN	9
#define LED_MOTOR_CROWD_LIMIT_MAX	11
#define LED_MOTOR_CROWD_INRANGE		10

#define LED_MOTOR_SWING_LIMIT_MIN	13
#define LED_MOTOR_SWING_LIMIT_MAX	15
#define LED_MOTOR_SWING_INRANGE		14

#define LED_MOTORCONTROLLER_MODE 0
#define BUTTON_MOTORCONTROLLER_SELECT_MODE 0
#define BUTTON_MOTORCONTROLLER_SELECT_LIMIT_FOR_SETTING 1
#define BUTTON_MOTORCONTROLLER_SET_SELECTED_LIMIT_TO_CURRENT_POSITION 2
#define BUTTON_MOTORCONTROLLER_RESET_ALL_LIMITS 3






class MachineControl{

public:
	MotorControl* MotorControlHandles[6];
	MachineControl();
	void refresh(uint32_t millis);
	void setUpInputPin_motor2_channelB();
	void setUpHardWareInterrupt_motor2_channelA();
	void Motor2InterruptHandler();

	void setUpInputPin_motor1_channelB();
	void setUpHardWareInterrupt_motor1_channelA();
	void Motor1InterruptHandler();
	int (*getCharFunctionPointer)(uint8_t *buf);

private:
	//motor variables

	uint8_t activeMotorForTestingOrCalibration =0; //motorcontrolhandles
	bool ch2Memory=0;
	bool motor2_chBMemory =0;

	// motor controller
	//IOBoard panel4(PANEL_4);
	IOBoard panel4;
	IOBoard* IOBoardHandler [4];
	uint8_t motorControllerMode=0;
	int8_t activeLimit=0;

	MotorControl motor1;
	MotorControl motor2;

	uint32_t millis;
	uint16_t edgeMemory =0;
	uint16_t  secondEdgeMemory= 0;



};

#endif
