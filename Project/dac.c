#include "dac.h"


void init_dac(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;

	//DAC11 A4
	GPIO_ANALOG(GPIOA, 4);
	GPIO_PUSHPULL(GPIOA, 4);
	GPIO_SPEED_40M(GPIOA, 4);
	//GPIO_FUNCTION(GPIOA, 4, 0x07);
	
	//DAC12 A5
	GPIO_ANALOG(GPIOA, 5);
	GPIO_PUSHPULL(GPIOA, 5);
	GPIO_SPEED_40M(GPIOA, 5);
	//GPIO_FUNCTION(GPIOA, 5, 0x07);

	//DAC1->CR |= DAC_CR_MAMP2_1 | DAC_CR_WAVE2_1; //Noise of channel 2 (y)
	//DAC1->CR |= DAC_CR_MAMP1_1 | DAC_CR_WAVE1_1; //Noise of channel 1 (y)
	
	//Trigger allready on chan 6, trigger disabled
	
	
	DAC->CR |= DAC_CR_EN2 | DAC_CR_EN1; //Enable dacs //DAC_CR_BOFF2 | DAC_CR_BOFF1 | 
	
	DAC->DHR12RD = 0x0fff0777; //Output = 0 on both chanels
}
	
void dac_output(uint32_t value)
{
	DAC->DHR12RD = value;
}