#include "IOBoard.h"


IOBoard::IOBoard(PanelId_TypeDef panelId){
	this ->panelId = panelId;

	if (panelId == PANEL_1){

		ledAnodePins[0] = GPIO_Pin_2,
		ledAnodePins[1] = GPIO_Pin_3;
		ledAnodePins[2] = GPIO_Pin_4;
		ledAnodePins[3] = GPIO_Pin_5;

	}
}

void IOBoard::initSlider(SliderNumber_TypeDef sliderNumberOnBoard, GPIOPinSlider_TypeDef adcPin){

	if (!this->isADCSetUp){
		initADC();
	}
}

void IOBoard::readButtons(){
	//will read all four buttons.
	//this is just the last step after scanning readButtonsLow and readButtonsHigh

	//i.e.
	/*button readout
			if (ticker20ms>=20){
				 ticker20msEdgeMemory= !ticker20msEdgeMemory;
				if (ticker20msEdgeMemory){
					panel1.readButtonsHigh();
				}else{
					panel1.readButtonsLow();
					panel1.readButtons();
				}
				ticker20ms =0;
			}
	*/

	bool previousButtonValues[4];

	//preserve previous button values
	for (uint8_t i=0; i<4;i++){
		previousButtonValues[i] = buttonValues[i];
	}

	//determine button presses
	buttonValues [0] = !pinsStatePullUpLow [0] && !pinsStatePullUpHigh [0];//low and low, means button connected to ground is switched
	buttonValues [1] =  pinsStatePullUpLow [0] &&  pinsStatePullUpHigh [0];// high and high means button connected to VCC is switched
	buttonValues [2] = !pinsStatePullUpLow [1] && !pinsStatePullUpHigh [1];//low and low, means button connected to ground is switched
	buttonValues [3] =  pinsStatePullUpLow [1] &&  pinsStatePullUpHigh [1];// high and high means button connected to VCC is switched

	//check for edges
	for (uint8_t i=0; i<4;i++){
		buttonEdgesPressed[i] = 	!previousButtonValues[i] &&  buttonValues [i];
		buttonEdgesDePressed[i] = 	 previousButtonValues[i] && !buttonValues [i];
	}

	//check if a state changed.
	this->atLeastOneButtonStateChanged = false;
	for (uint8_t i=0; i<4;i++){
		if (buttonEdgesPressed[i] || buttonEdgesDePressed[i]){
			this->atLeastOneButtonStateChanged = true;
		}
	}
}

bool IOBoard::getAtLeastOneButtonStateChanged(){
	bool memory;
	memory = this->atLeastOneButtonStateChanged;
	this->atLeastOneButtonStateChanged = false;
	return memory;
}

void IOBoard::readButtonsLow(){
	//ASSUMES the pull up resistor is not set (pin floating) no pull down either, this is set in the hardware.
	this->pinsStatePullUpLow [0] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
	this->pinsStatePullUpLow [1] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_UP; //set for the next cycle.
	GPIO_Init(GPIOB,&GPIO_initStructre);//Affecting the port with the initialization structure configuration
}

void IOBoard::readButtonsHigh(){
	//ASSUMES the pull up resistor is enabled.
	this->pinsStatePullUpHigh [0] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
	this->pinsStatePullUpHigh [1] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);

	//put already down for the next cycle.
	//GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL;//set for the next cycle --> no pull down resistor, already implemented in hardware...
	GPIO_Init(GPIOB,&GPIO_initStructre);//Affecting the port with the initialization structure configuration
}


bool IOBoard::getButtonState(uint16_t button){
	return buttonValues [button];
}

bool IOBoard::getButtonEdgeDePressed(uint16_t button){
	bool state;
	state = buttonEdgesDePressed [button];
	 buttonEdgesDePressed [button] = false;
	return state;
}

bool IOBoard::getButtonEdgePressed(uint16_t button){
	//make sure each edge can only be called once.
	bool state;
	state = buttonEdgesPressed [button];
	buttonEdgesPressed [button] = false;
	return state;
}

void IOBoard::initButtons(){
	//very specific
	if (panelId == PANEL_1){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		//GPIO_InitTypeDef GPIO_initStructre; defined in .h file, has to be available because we work with two buttons on one pin...
		//Analog pin configuration
		GPIO_initStructre.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14  ;
		GPIO_initStructre.GPIO_Mode = GPIO_Mode_IN ;
		GPIO_initStructre.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_initStructre.GPIO_OType = GPIO_OType_PP;
		GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOB,&GPIO_initStructre);//Affecting the port with the initialization structure configuration
	}
}

void IOBoard::initLeds(){
	//very specific
	if (panelId == PANEL_1){
		//leds
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		//GPIO_InitTypeDef GPIO_initStructre; defined in .h file, has to be available because we work with two buttons on one pin...
		//Analog pin configuration
		GPIO_InitTypeDef GPIO_initStructre;
		GPIO_initStructre.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |  GPIO_Pin_4 | GPIO_Pin_5 ;
		GPIO_initStructre.GPIO_Mode = GPIO_Mode_OUT ;
		GPIO_initStructre.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_initStructre.GPIO_OType = GPIO_OType_PP;
		GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

		//common cathode pin always low.
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		//GPIO_SetBits(GPIOA, GPIO_Pin_2 |  GPIO_Pin_3 |  GPIO_Pin_4 |  GPIO_Pin_5 );
	}
}

void IOBoard::scanLeds(){
	for (uint8_t i=0; i<4;i++){
		if (leds[i]){
			GPIO_SetBits(GPIOA,this->ledAnodePins[i]);
		}else{
			GPIO_ResetBits(GPIOA,this->ledAnodePins[i]);
		}
	}
}

void IOBoard::setLed(uint16_t ledNumber, bool value){
	this->leds[ledNumber] = value;
}

void IOBoard::toggleLed(uint16_t ledNumber){
	this->leds[ledNumber] = !this->leds[ledNumber];
}

void IOBoard::initADC(){

	//very specific

	if (panelId == PANEL_1){

		//set pin PC0 as analog in
		RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
		GPIO_InitTypeDef GPIO_initStructre; //Structure for analog input pin
		//Analog pin configuration
		GPIO_initStructre.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3  ;//The channel 10 is connected to PC0
		GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
		GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
		GPIO_Init(GPIOC,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

		/* Unchanged: Define ADC init structures */
		ADC_InitTypeDef       ADC_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* Unchanged: populate default values before use */
		ADC_StructInit(&ADC_InitStructure);
		ADC_CommonStructInit(&ADC_CommonInitStructure);

		/* Unchanged: enable ADC peripheral */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

		/* Unchanged: init ADC */
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInit(&ADC_CommonInitStructure);

		/* Changed: Enabled scan mode conversion*/
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;
		ADC_InitStructure.ADC_ExternalTrigConv= 0;
		ADC_InitStructure.ADC_ExternalTrigConvEdge= 0;
		ADC_InitStructure.ADC_NbrOfConversion= 6;

		ADC_Init(ADC1, &ADC_InitStructure);

		/* Enable Vref & Temperature channel */
		ADC_TempSensorVrefintCmd(ENABLE);

		/* Configure channels */
		/* Temp sensor */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
		/* VREF_int (2nd) */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 2, ADC_SampleTime_480Cycles);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_480Cycles); ///PC0  //channel10
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 4, ADC_SampleTime_480Cycles); ///PC0  //channel10
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 5, ADC_SampleTime_480Cycles); ///PC0  //channel10
		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_480Cycles); ///PC0  //channel10

		ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);

		/* Enable ADC interrupts */
		ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

		/* Configure NVIC */
		NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
		NVIC_Init(&NVIC_InitStructure);

		/* Enable ADC1 **************************************************************/
		ADC_Cmd(ADC1, ENABLE);
	}


}
void IOBoard::adcDoSingleConversion(){
	if (panelId == PANEL_1){
		ADC_SoftwareStartConv(ADC1);
	}
}

void IOBoard::ADCInterruptHandler(uint16_t slider, uint16_t value){
	//call this from the interrupt vector. Will update the data in this class with the appropriate value.
	this->sliderValues[slider] = value;

}

uint16_t IOBoard::getSliderValue(uint16_t slider){
	return this->sliderValues[slider];
}

void IOBoard::stats(char* outputString){

	//if (isConstructed){
		outputString[0]= 'O';
	//}else{
	//	outputString[0]= 'P';
	//}
}

void IOBoard::demoModeLoop(){
	//call every 20 ms --> 50Hz
	this->demoLoopCounter++; //update counter

	for (uint8_t i=0;i<4;i++){

		if (getButtonState(i)){
		//blinkmode

			if (demoLoopCounter > getSliderValue(i)/490 ){
				setLed(i,true);
			}else{
				setLed(i,false);
			}
		}else{
			//led off
			setLed(i,false);
		}
	}
	if (this-> demoLoopCounter > 10){
		this->demoLoopCounter = 0;

	}


}

