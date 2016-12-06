#include "IOBoard.h"


IOBoard::IOBoard(PanelId_TypeDef panelId){
	this ->panelId = panelId;
}

void IOBoard::initSlider(SliderNumber_TypeDef sliderNumberOnBoard, GPIOPinSlider_TypeDef adcPin){

	if (!this->isADCSetUp){
		initADC();
	}
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

void IOBoard::ADCInterruptHandler(uint16_t slider, uint16_t value){
	//switch (channel){
	this->sliderValues[slider] = value;
	//printf("%d\r\n",slider);
	//}
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

