

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
// void encoderInitTest();
// void encoderReadTest();
 //void encoderOutputTest();


#include "EncoderTimer.h"



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
	initDiscoveryBoard();





	/**
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN ;

	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC ,&GPIO_InitDef);


	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)){
		printf("button pressed\r\n " );
	}else{
		printf("button not pressed\r\n " );

	}
	*/

	/**/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT ;

	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitDef);

	GPIO_ResetBits(GPIOA,GPIO_Pin_6 ); //reset pin. should be high for oled to work.
	while (millis < 500){}
	GPIO_SetBits(GPIOA,GPIO_Pin_6 );

	while (millis < 1000){}
	GPIO_ResetBits(GPIOA,GPIO_Pin_3 );

	/**/




	bool isInit = false;

	millisMemory_outputToSerial = millis;

	uint8_t received_val = 0;

	init_SPI1();
	ssd1306Init(); //has worked (all lights on)
	//oledInit();
	bool test;

	while(1){
		test = false;
		//panel1.refresh(millis);
		//panel1.demoModeLoop();
		if (millis>3000 && !test){
			STM_EVAL_LEDOn(LED4);
			test = true;
		}

	}
}

// this function initializes the SPI1 peripheral
void init_SPI1(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* configure pins used by SPI1
	 * PA5 = SCK
	 * PA6 = MISO
	 * PA7 = MOSI
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);


	// enable peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* configure SPI1 in Mode 0
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */
	/*
	 *
	 */
/*
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first


/**/
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set ; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
/**/



	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE); // enable SPI1
}

/* This funtion is used to transmit and receive data
 * with SPI1
 * 			data --> data to be transmitted
 * 			returns received value
 */
uint8_t SPI1_send(uint8_t data){

	SPI1->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
//	while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI1->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
//	return SPI1->DR; // return received data from SPI data register
}

void WRITE_COMMAND(uint8_t command){
	GPIO_ResetBits(GPIOA,GPIO_Pin_3 );
	SPI1_send(command);
	GPIO_SetBits(GPIOA,GPIO_Pin_3 );
}

void WRITE_DATA(uint8_t data){
	GPIO_SetBits(GPIOA,GPIO_Pin_3 );
	SPI1_send(data);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3 );

}

/**
 * https://gist.github.com/pulsar256/564fda3b9e8fc6b06b89
 * according to http://www.adafruit.com/datasheets/UG-2864HSWEG01.pdf Chapter 4.4 Page 15
 */
void ssd1306Init(void)
{

    WRITE_COMMAND(0xAE); // Set display OFF

    WRITE_COMMAND(0xD4); // Set Display Clock Divide Ratio / OSC Frequency
    WRITE_COMMAND(0x80); // Display Clock Divide Ratio / OSC Frequency

    WRITE_COMMAND(0xA8); // Set Multiplex Ratio
    WRITE_COMMAND(0x3F); // Multiplex Ratio for 128x64 (64-1)

    WRITE_COMMAND(0xD3); // Set Display Offset
    WRITE_COMMAND(0x00); // Display Offset

    WRITE_COMMAND(0x40); // Set Display Start Line

    WRITE_COMMAND(0x8D); // Set Charge Pump
    WRITE_COMMAND(0x14); // Charge Pump (0x10 External, 0x14 Internal DC/DC)

    WRITE_COMMAND(0xA1); // Set Segment Re-Map
    WRITE_COMMAND(0xC8); // Set Com Output Scan Direction

    WRITE_COMMAND(0xDA); // Set COM Hardware Configuration
    WRITE_COMMAND(0x12); // COM Hardware Configuration

    WRITE_COMMAND(0x81); // Set Contrast
    WRITE_COMMAND(0xCF); // Contrast

    WRITE_COMMAND(0xD9); // Set Pre-Charge Period
    WRITE_COMMAND(0xF1); // Set Pre-Charge Period (0x22 External, 0xF1 Internal)

    WRITE_COMMAND(0xDB); // Set VCOMH Deselect Level
    WRITE_COMMAND(0x40); // VCOMH Deselect Level

    WRITE_COMMAND(0xA5); // a4 Set all pixels OFF  a5 is all pixels on
    WRITE_COMMAND(0xA6); // Set display not inverted
    WRITE_COMMAND(0xAF); // Set display On

    //i2c_OLED_clear_display();
}


void oledInit(){
	//https://cdn-shop.adafruit.com/datasheets/UG-2864HSWEG01+user+guide.pdf
	WRITE_COMMAND(0xae);//--turn off oled panel
	WRITE_COMMAND(0x00);//---set low column address
	WRITE_COMMAND(0x10);//---set high column address
	WRITE_COMMAND(0x40);//--set start line address
	WRITE_COMMAND(0x81);//--set contrast control register
	WRITE_COMMAND(0xcf);
	WRITE_COMMAND(0xa1);//--set segment re-map 95 to 0
	WRITE_COMMAND(0xa6);//--set normal display
	WRITE_COMMAND(0xa8);//--set multiplex ratio(1 to 64)
	WRITE_COMMAND(0x3f);//--1/64 duty
	WRITE_COMMAND(0xd3);//-set display offset
	WRITE_COMMAND(0x00);//-not offset
	WRITE_COMMAND(0xd5);//--set display clock divide ratio/oscillator frequency
	WRITE_COMMAND(0x80);//--set divide ratio
	WRITE_COMMAND(0xd9);//--set pre-charge period
	WRITE_COMMAND(0xf1);
	WRITE_COMMAND(0xda);//--set com pins hardware configuration
	WRITE_COMMAND(0x12);
	WRITE_COMMAND(0xdb);//--set vcomh
	WRITE_COMMAND(0x40);
	WRITE_COMMAND(0x8d);//--set Charge Pump enable/disable
	WRITE_COMMAND(0x14);//--set(0x10) disable
	WRITE_COMMAND(0xaf);//--turn on oled panel
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
/*
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

*/
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
	printf("Board generated an internal error. Please restart.");
	//while (1)
	//{
	//	//get stuck here forever.

	//}

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


/*
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
/// Handle interrupt
/*
void EXTI3_IRQHandler(void) {
	machineControlPointer->Motor1InterruptHandler();

}


void EXTI4_IRQHandler(void) {
	machineControlPointer->Motor2InterruptHandler();

}



/// Handle interrupt
void EXTI1_IRQHandler(void) {
	machineControlPointer->Motor3InterruptHandler();

}
*/
#ifdef __cplusplus
 }
#endif
