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
		void adcDoSingleConversion();
		void initButtons();
		void readButtons();
		void readButtonsHigh();
		void readButtonsLow();
		bool readButton(uint16_t button);
		bool getAtLeastOneButtonStateChanged();
		void ADCInterruptHandler(uint16_t slider, uint16_t value);
		uint16_t getSliderValue(uint16_t slider);
	private:

		bool isADCSetUp = false;
		//uint16_t readSlider();
		uint16_t sliderValues [4];

		bool buttonValues[4];
		bool buttonEdgesPressed[4];
		bool buttonEdgesDePressed[4];
		bool atLeastOneButtonStateChanged;

		PanelId_TypeDef panelId;
		GPIO_InitTypeDef GPIO_initStructre;

		bool pinsStatePullUpLow[2];
		bool pinsStatePullUpHigh[2];


};



#endif //IOBOARD_H
