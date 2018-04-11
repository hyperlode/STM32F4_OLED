#ifndef MAIN_H
#define MAIN_H

//#define USE_VCP //in coocox in precompiler directives defined




#ifdef __cplusplus
	#include "MachineControl.h"
	//#include "EncoderToTimer.h"
	#include "tm_stm32f4_ssd1306.h"

	#include <stdio.h>
	extern "C"
	{
#endif

	IOBoard panel1;
	//char lodeStrTest []={'a','\0'};

	//MachineControl* machineControlPointer;

	//EncoderToTimer testEncoder;
	//EncoderToTimer testEncoder2;
	//EncoderToTimer testEncoder3;

	//volatile uint32_t ticker, downTicker;
	static uint32_t ticker, downTicker,ticker20ms ;
	static int ConvertedValue = 0; //Converted value readed from ADC

	static uint16_t temp = 0;
	static uint16_t vref = 0;
	static uint16_t adcSampleChannelCounter = 0;
	static uint16_t adcNumberOfSampleCycles= 0;


	uint32_t  millisMemory_testing = 0;
	uint32_t  millisMemory_outputToSerial = 0;

	uint32_t millis;




	#include "stm32f4xx_adc.h"
	#include "stm32f4xx_conf.h"
	#include "stm32f4xx.h"
	#include "stm32f4xx_gpio.h"
	#include "stm32f4xx_rcc.h"
	#include "stm32f4xx_exti.h"
	#include "stm32f4xx_dac.h"
	#include "stm32f4xx_spi.h"

#ifdef USE_VCP
	#include "usbd_cdc_core.h"
	#include "usbd_usr.h"
	#include "usbd_desc.h"
	#include "usbd_cdc_vcp.h"
	#include "usb_dcd_int.h"
#endif
	#include "stm32f4_discovery.h"

	void initDiscoveryBoard();
	void init_SPI1();
	uint8_t SPI1_send(uint8_t data);
	void oledInit();
	void ssd1306Init();
	void WRITE_COMMAND(uint8_t command);
	void WRITE_DATA(uint8_t data);
#ifdef __cplusplus
	}
#endif




#endif //MAIN_H
