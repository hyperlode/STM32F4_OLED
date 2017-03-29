#ifndef MACHINECONTROL_H
#define MACHINECONTROL_H
#include <stdint.h>
#include <stdio.h>

#include "IOBoard.h"
#include "MotorControl.h"
#include "AppliedDAC.h"
#include "EncoderToTimer.h"

#define MODE_NORMAL 0
//#define MODE_TEST 2
#define MODE_CALIBRATE 1
#define NUMBER_OF_MODES 2


#define NUMBER_OF_MOTORS 3



#define LED_MOTOR_HOIST_LIMIT_MIN	5
#define LED_MOTOR_HOIST_LIMIT_MAX	7
#define LED_MOTOR_HOIST_INRANGE		6
//#define LED_MOTOR_HOIST_ENABLE		4


#define LED_MOTOR_CROWD_LIMIT_MIN	9
#define LED_MOTOR_CROWD_LIMIT_MAX	11
#define LED_MOTOR_CROWD_INRANGE		10
//#define LED_MOTOR_CROWD_ENABLE		8
#define LED_MOTOR_CROWD_POS 8
#define LED_MOTOR_CROWD_NEG 4

#define LED_MOTOR_SWING_LIMIT_MIN	13
#define LED_MOTOR_SWING_LIMIT_MAX	15
#define LED_MOTOR_SWING_INRANGE		14
//#define LED_MOTOR_SWING_ENABLE		12

#define LED_MOTORCONTROLLER_MODE 0
#define BUTTON_MOTORCONTROLLER_SELECT_MODE 0
#define BUTTON_MOTORCONTROLLER_SELECT_LIMIT_FOR_SETTING 1
#define BUTTON_MOTORCONTROLLER_SET_SELECTED_LIMIT_TO_CURRENT_POSITION 2
#define BUTTON_MOTORCONTROLLER_RESET_ALL_LIMITS 3
#define BUTTON_ZEROING_ALL_AXIS 2 //dual usage of button, depending on mode.
#define ZEROING_BUTTON_TIME_DELAY_MILLIS 2000

#define REFRESH_DELAY_MILLIS_ADC 50
#define REFRESH_DELAY_MILLIS_DAC 50 //50
#define REFRESH_DELAY_MILLIS_ENCODERS 10

//empirical value, derived from joystick on controller chair, raw adc value when at zero.

#define ADC_MOTOR_HOIST_ZERO_SPEED_VALUE 3914 //redefined 20170317 2650 //defined 201703615
#define ADC_MOTOR_HOIST_MAX_VALUE 4095
#define DAC_MOTOR_HOIST_ZERO_SPEED_VALUE 2145 //defined 201703615
#define DAC_MOTOR_HOIST_MAX_VALUE 4095



#define ADC_MOTOR_CROWD_ZERO_SPEED_VALUE 2220  //defined 201703615

#define ADC_MOTOR_CROWD_MAX_VALUE 4095
#define DAC_MOTOR_CROWD_ZERO_SPEED_VALUE 2200 //defined 201703615
#define DAC_MOTOR_CROWD_MAX_VALUE 4095

#define ADC_MOTOR_SWING_ZERO_SPEED_VALUE 2315  //set 20170316

#define ADC_MOTOR_SWING_MAX_VALUE 4095
#define DAC_MOTOR_SWING_ZERO_SPEED_VALUE 168
#define DAC_MOTOR_SWING_MAX_VALUE 255



class MachineControl{

public:

	MachineControl();
	void refresh(uint32_t millis);

	bool getMotorsZeroedSinceStartup();
	void selectNextLimitToBeCalibrated();

	void setUpInputPin_motor1_channelB();
	void setUpHardWareInterrupt_motor1_channelA();
	void Motor1InterruptHandler();

	void setUpInputPin_motor2_channelB();
	void setUpHardWareInterrupt_motor2_channelA();
	void Motor2InterruptHandler();

	void setUpInputPin_motor3_channelB();
	void setUpHardWareInterrupt_motor3_channelA();
	void Motor3InterruptHandler();

	void logVref(uint16_t);


	void speedInputADCInterrupt(uint16_t potentioNumber, uint16_t value);
	int (*getCharFunctionPointer)(uint8_t *buf);

	void initEncoders();

private:

	IOBoard panel1;

	//info panel with lights and buttons
	IOBoard panel4;
	IOBoard* IOBoardHandler [4]; //contains pointers to the IOBoards
	//ADC
	uint32_t adcRanges[3];

	uint32_t dacZeroSpeedRawValues [3];

	//DAC
	AppliedDAC dacSpeedControl_Hoist;
	AppliedDAC dacSpeedControl_Crowd;
	AppliedDAC dacSpeedControl_Swing;

	uint32_t dacZeroSpeedValues [3];
	uint32_t dacRanges[3];
	uint32_t dacValues[3];
	AppliedDAC* DacHandlerPointers[3];

	//motors
	MotorControl motor1;
	MotorControl motor2;
	MotorControl motor3;
	MotorControl* MotorControlHandles[6];


	//encoders (with timers)
	EncoderToTimer encoder1;

	//encoders
	bool motor2IsCCW=0;


	uint8_t activeMotorForTestingOrCalibration =0; //motorcontrolhandles
	bool motor1ChannelBMemory = 0;
	bool motor2ChannelBMemory = 0;
	bool motor3ChannelBMemory = 0;

	//program control
	uint32_t millis;
	uint16_t edgeMemory =0;
	uint16_t  secondEdgeMemory= 0;
	uint32_t millisMemory_dacProcess;
	uint32_t millisMemory_adcProcess;
	uint32_t millisMemory_encoderProcess;

	uint32_t millisMemory_checkForSerialInput;
	uint8_t theByte;
	uint32_t zeroingButtonPressStartTime;
	uint16_t vref;

	uint8_t motorControllerMode=0;
	int8_t activeLimit=0;

};

#endif
