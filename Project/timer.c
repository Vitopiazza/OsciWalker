#include "timer.h"

void init_timer(void)
{

	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	
	
	
	TIM6->CR2 |= TIM_CR2_MMS_2; //Set trigger output to overflow
	
	TIM6->CNT &= ~0xff; //Clear counter
	TIM6->PSC = 0; //No prescaler
	TIM6->ARR = 360; //Update rate to 100 kHz, hopefully
	
	TIM6->CR1 |= TIM_CR1_CEN; //Enable counter
}

void start_timer(void)
{
	TIM6->SR &= ~TIM_SR_UIF; //Reset interrupt flag
	TIM6->CNT &= ~0xff; //Clear counter
	
	NVIC->IP[TIM6_DAC_IRQn] = TIMER_INTERRUPT_PRIORITY << 4; 
	// >> 0x05 take top 3 bits = 32 bits per register, &0x1f - 32 bit mask
    NVIC->ISER[TIM6_DAC_IRQn >> 0x05] = 0x01 << (TIM6_DAC_IRQn & 0x1F); //Enable interrupt - can assing due to it being positive edge triggered
	
	
	TIM6->DIER |= TIM_DIER_UIE; //Enable interrupts
}

void stop_timer(void)
{
	TIM6->DIER &= ~TIM_DIER_UIE; //Disable interrupts
	NVIC->ICER[TIM6_DAC_IRQn >> 0x05] = 0x01 << (TIM6_DAC_IRQn & 0x1F); //Disable interrupt - can assing due to it being positive edge triggered
}