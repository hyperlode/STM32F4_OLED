#ifndef MAIN_H
#define MAIN_H

#define USE_VCP

#define MODE_NORMAL 0
#define MODE_TEST 1
#define MODE_CALIBRATE 2

#define NUMBER_OF_MOTORS 2

#define LED_MOTOR_HOIST_LIMIT_MIN	4
#define LED_MOTOR_HOIST_LIMIT_MAX	6
#define LED_MOTOR_HOIST_INRANGE		5
#define LED_MOTOR_CROWD_LIMIT_MIN	8
#define LED_MOTOR_CROWD_LIMIT_MAX	10
#define LED_MOTOR_CROWD_INRANGE		9
#define LED_MOTOR_SWING_LIMIT_MIN	0
#define LED_MOTOR_SWING_LIMIT_MAX	2
#define LED_MOTOR_SWING_INRANGE		1

#define LED_MOTORCONTROLLER_MODE 3
#define BUTTON_MOTORCONTROLLER_SELECT_MODE 3
#define BUTTON_MOTORCONTROLLER_SELECT_LIMIT_FOR_SETTING 0
#define BUTTON_MOTORCONTROLLER_SET_SELECTED_LIMIT_TO_CURRENT_POSITION 1
#define BUTTON_MOTORCONTROLLER_RESET_ALL_LIMITS 2


#ifdef __cplusplus
	#include "IOBoard.h"
	#include "MotorControl.h"
	#include <stdio.h>
	extern "C"
	{
#endif


	char lodeStrTest []={'a','\0'};




	//volatile uint32_t ticker, downTicker;
	static uint32_t ticker, downTicker,ticker20ms ;
	static int ConvertedValue = 0; //Converted value readed from ADC

	static uint16_t temp = 0;
	static uint16_t vref = 0;
	static uint16_t adcSampleChannelCounter = 0;
	static uint16_t adcNumberOfSampleCycles= 0;
	static uint16_t  secondEdgeMemory= 0;
	static uint16_t edgeMemory =0;
	static uint16_t  ticker20msEdgeMemory= 0;
	uint32_t millis;

	//motor variables
	MotorControl* MotorControlHandles[6];
	static uint8_t activeMotorForTestingOrCalibration =0; //motorcontrolhandles
	static bool ch2Memory=0;
	static bool motor2_chBMemory =0;

	// motor controller
	IOBoard* IOBoardHandler [4];
	uint8_t motorControllerMode=0;
	int8_t activeLimit=0;


	#include "stm32f4xx_adc.h"
	#include "stm32f4xx_conf.h"
	#include "stm32f4xx.h"
	#include "stm32f4xx_gpio.h"
	#include "stm32f4xx_rcc.h"
	#include "stm32f4xx_exti.h"
#ifdef USE_VCP
	#include "usbd_cdc_core.h"
	#include "usbd_usr.h"
	#include "usbd_desc.h"
	#include "usbd_cdc_vcp.h"
	#include "usb_dcd_int.h"
#endif
	#include "stm32f4_discovery.h"

	void initDiscoveryBoard();
#ifdef __cplusplus
	}
#endif




#endif //MAIN_H
