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


#define DEMOLOOP  //when defined, configured to run in demo mode. --> at buttonpress, led blinks, blink rate set by slider.
#define BUTTON_PRESS_DELAY 20    //20ms jitter delay
#define ADC_SAMPLE_PERIOD_MILLIS 50 //every 50ms adc sampling.
#define DEMOLOOP_UPDATE_DELAY 20	//millis
typedef enum
{
  PANEL_1 = 0,
  PANEL_2 = 1,
  PANEL_3 = 2
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
		//void initSlider(SliderNumber_TypeDef sliderNumberOnBoard, GPIOPinSlider_TypeDef adcPin);
		void stats(char* outputString);
		void refresh(uint32_t millis);
		void demoModeLoop();

		void initADC();
		void adcDoSingleConversion();
		bool getAtLeastOneButtonStateChanged();
		void ADCInterruptHandler(uint16_t slider, uint16_t value);
		uint16_t getSliderValue(uint16_t slider);

		void initLeds();
		void scanLeds();
		void setLed(uint16_t ledNumber, bool value);
		void toggleLed(uint16_t ledNumber);

		void initButtons();
		void readButtons();
		void readButtonsHigh();
		void readButtonsLow();
		bool getButtonState(uint16_t button);
		bool getButtonEdgeDePressed(uint16_t button);
		bool getButtonEdgePressed(uint16_t button);

	private:
		PanelId_TypeDef panelId;
		GPIO_InitTypeDef GPIO_Buttons_initStructure;
		uint16_t demoLoopCounter[4];
		uint32_t demoLooptimer;
		uint32_t millis;

		bool isADCSetUp = false;
		uint16_t sliderValues [4];
		uint32_t adcSampleTimer;
		uint16_t  adcPins [4];
		GPIO_TypeDef* adcPort;
		uint32_t adcPeripheral;

		uint16_t numberOfButtons;
		bool buttonValues[16];
		bool buttonEdgesPressed[16];
		bool buttonEdgesDePressed[16];
		bool atLeastOneButtonStateChanged;
		uint32_t buttonTimer;
		bool buttonsReadHighElseLow;
		uint16_t  buttonPins [8];
		GPIO_TypeDef* buttonPort;
		uint32_t buttonPeripheral;
		bool pinsStatePullUpLow[8];
		bool pinsStatePullUpHigh[8];

		uint16_t numberOfLeds;
		uint16_t scanCathode;
		uint16_t  ledAnodePins [4];
		uint16_t ledCathodePins[4];
		GPIO_TypeDef* ledPort;
		uint32_t ledPeripheral;
		bool leds[4];


};



#endif //IOBOARD_H
