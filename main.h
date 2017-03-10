#ifndef MAIN_H
#define MAIN_H

#define USE_VCP



#ifdef __cplusplus
	#include "MachineControl.h"




	#include <stdio.h>
	extern "C"
	{
#endif


	//char lodeStrTest []={'a','\0'};

	MachineControl* machineControlPointer;


	//volatile uint32_t ticker, downTicker;
	static uint32_t ticker, downTicker,ticker20ms ;
	static int ConvertedValue = 0; //Converted value readed from ADC

	static uint16_t temp = 0;
	static uint16_t vref = 0;
	static uint16_t adcSampleChannelCounter = 0;
	static uint16_t adcNumberOfSampleCycles= 0;


	static uint16_t  ticker20msEdgeMemory= 0;
	uint32_t millis;




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
