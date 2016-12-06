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
  PANEL_1 = 0
} PanelId_TypeDef;


typedef enum
{
  PC0 = 0,
  PC1 = 1,
  PC2 = 2,
  PC3 = 3
} GPIOPinSlider_TypeDef;


typedef enum
{
	SLIDER_1, SLIDER_2, SLIDER_3, SLIDER_4

} SliderNumber_TypeDef;




class IOBoard{
	public:
		IOBoard(PanelId_TypeDef panelId);
		void initSlider(SliderNumber_TypeDef sliderNumberOnBoard, GPIOPinSlider_TypeDef adcPin);
		void stats(char* outputString);
		void initADC();
		void ADCInterruptHandler(uint16_t slider, uint16_t value);
		uint16_t getSliderValue(uint16_t slider);
	private:

		bool isADCSetUp = false;
		//uint16_t readSlider();
		uint16_t sliderValues [4];
		PanelId_TypeDef panelId;

};



#endif //IOBOARD_H
