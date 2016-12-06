/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IOBOARD_H
#define IOBOARD_H

#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include <stdio.h>
#include "stm32f4_discovery.h"

typedef enum
{
  Panel1 = 0,
  Panel2 = 1,
  Panel3 = 2,
  Panel4 = 3
} PanelNumber;


typedef enum
{
  PC0 = 0,
  PC1 = 1,
  PC2 = 2,
  PC3 = 3
} GPIOPinSlider;


typedef enum
{
	SLIDER_1, SLIDER_2, SLIDER_3, SLIDER_4

} SliderNumber;




class IOBoard{
	public:
	IOBoard();
	void initSlider(SliderNumber sliderNumberOnBoard, GPIOPinSlider adcPin);
	void stats(char* outputString);
	void initADC();
	void ADCInterruptHandler(uint16_t slider, uint16_t value);
	uint16_t getSliderValue(uint16_t slider);
	private:
	bool isConstructed=0;
	bool isADCSetUp = false;
	//uint16_t readSlider();
	uint16_t sliderValues [4];


};



#endif //IOBOARD_H
