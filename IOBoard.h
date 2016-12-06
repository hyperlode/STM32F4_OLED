/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IOBOARD_H
#define IOBOARD_H


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

	//uint16_t readSlider();



};



#endif //IOBOARD_H
