

//notes: to convert to cpp:
//-change name
//-configuration -> compile "use CPP"
//-configurations-> link ->"don't use the standard system startup files"


#define HSE_VALUE ((uint32_t)8000000) /* STM32 discovery uses a 8Mhz external crystal */



#include "main.h"



#ifdef USE_VCP
/*
 * The USB data must be 4 byte aligned if DMA is enabled. This macro handles
 * the alignment, if necessary (it's actually magic, but don't tell anyone).
 */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

#endif


/*
 * Define prototypes for interrupt handlers here. The conditional "extern"
 * ensures the weak declarations from startup_stm32f4xx.c are overridden.
 */
#ifdef __cplusplus
 extern "C" {
#endif
 void init();
 void ColorfulRingOfDeath(void);




void SysTick_Handler(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
#ifdef USE_VCP
void OTG_FS_IRQHandler(void);
void OTG_FS_WKUP_IRQHandler(void);
#endif

#ifdef __cplusplus
}
#endif

int main(void)
{
	/* Set up the system clocks */
	SystemInit();

	/* Initialize USB, IO, SysTick, and all those other things you do in the morning */
	init();


	//DAC_Setup();
	initDiscoveryBoard();

	//init machine control
	MachineControl machineControl;
	machineControlPointer = &machineControl;

	machineControl.getCharFunctionPointer = &VCP_get_char;
	//refresh machine control loop
	while(1){
		machineControl.refresh(millis);
/*
		//machineControlPointer -> dacTest.initDAC1();
		machineControlPointer -> dacTest.triggerDAC1(millis%4000);
		//machineControlPointer -> dacTest.triggerDAC1(4000);

		if (millis% 10 > 5 and timingEdge == 0 ){
			timingEdge == 1;

		}
		if (millis % 10 )
		timingEdge = millis%10 > 5;
		*/
		//if ((millis%1000)==0){

		//	printf("oieoe what didd is dooo.");
		//}
/*
		uint8_t theByte;
		if (VCP_get_char(&theByte))
		{
			if ( theByte != '\r' &&  theByte != '\n'){
				printf("Chaar Sent: %c  \r\n", theByte); //VCP_put_char(theByte);
			}
		}
		/**/

	}
}


void initDiscoveryBoard(){
	//init the leds on the discoveryboard
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDOn(LED5);

	STM_EVAL_LEDInit(LED3);

	STM_EVAL_LEDInit(LED4);


	STM_EVAL_LEDInit(LED6);
	STM_EVAL_LEDOff(LED6);

}
#ifdef __cplusplus
 extern "C" {
#endif


void init()
{
	/* STM32F4 discovery LEDs */
	//GPIO_InitTypeDef LED_Config;

	/* Always remember to turn on the peripheral clock...  If not, you may be up till 3am debugging... */
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//LED_Config.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	//LED_Config.GPIO_Mode = GPIO_Mode_OUT;
	//LED_Config.GPIO_OType = GPIO_OType_PP;
	//LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
	//LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_Init(GPIOD, &LED_Config);

	//dac test
	//DAC_GPIO_Config();
	//DAC_Config();

	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	//GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//Setup SysTick or CROD!
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		ColorfulRingOfDeath();
}

#ifdef USE_VCP
	// Setup USB
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);
#endif
	return;

}


/*
 * Call this to indicate a failure.
 */

void ColorfulRingOfDeath(void){
	STM_EVAL_LEDOn(LED6);
	while (1)
	{
		//get stuck here forever.

	}

}

/*
 * Interrupt Handlers
 */

void SysTick_Handler(void)
{
	ticker++;
	if (downTicker > 0)
	{
		downTicker--;
	}

	ticker20ms++;
	millis++;

}

void NMI_Handler(void)       {}
void HardFault_Handler(void) { ColorfulRingOfDeath(); }
void MemManage_Handler(void) { ColorfulRingOfDeath(); }
void BusFault_Handler(void)  { ColorfulRingOfDeath(); }
void UsageFault_Handler(void){ ColorfulRingOfDeath(); }
void SVC_Handler(void)       {}
void DebugMon_Handler(void)  {}
void PendSV_Handler(void)    {}

#ifdef USE_VCP
void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

void OTG_FS_WKUP_IRQHandler(void)
{
  if(USB_OTG_dev.cfg.low_power)
  {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}

#endif

/*
 * void adc_multiChannelConfigure(){

}
*/


/**/
void ADC_IRQHandler() {
        // acknowledge interrupt
        uint16_t value;
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);


        value = ADC_GetConversionValue(ADC1);
		switch (adcSampleChannelCounter){
		   case 0:
				temp = value;
				adcSampleChannelCounter++;
				break;
		   case 1:
				vref = value;
				adcSampleChannelCounter++;
				machineControlPointer->logVref(value);
				break;

			//all the other channels.
		   default:
				if (adcSampleChannelCounter<6){
					//
					//IOBoardHandler[0]->ADCInterruptHandler(adcSampleChannelCounter - 2, value); //IOBoard handle triggers.
					machineControlPointer->speedInputADCInterrupt(adcSampleChannelCounter - 2, value);

				}else{
					//set adcSampleChannelCounter to 10IOBoardHandler[1]->ADCInterruptHandler(adcSampleChannelCounter - 6, value); //IOBoard handle triggers.
				}

				adcSampleChannelCounter++;
				if (adcSampleChannelCounter ==6){
					adcSampleChannelCounter =0;
					adcNumberOfSampleCycles++;
				}
				break;

		}



}
/**/

// ---external C
/// Set interrupt handlers

void EXTI4_IRQHandler(void) {
	machineControlPointer->Motor2InterruptHandler();

}

/// Handle PB3 interrupt
void EXTI3_IRQHandler(void) {
	machineControlPointer->Motor1InterruptHandler();

}

/// Handle PB3 interrupt
void EXTI1_IRQHandler(void) {
	machineControlPointer->Motor3InterruptHandler();

}

#ifdef __cplusplus
 }
#endif
