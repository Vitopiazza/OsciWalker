#include "game_thread.h"


void GAME_thread(void* arg)
{
	static portTickType previous_wake_time = 0;
	for(;;){
		
		//Load last input into game code
		if(xQueueReceive(input_queue, &last_event, 0) == pdFALSE){
			last_event = EV_NONE;
		}
		
		game_loop(); //Do all game related stuff
		
		vTaskDelayUntil(&previous_wake_time , 5);
	}
}

void TIM6_DAC_IRQHandler(void)
{
	uint32_t value;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(TIM6->SR & TIM_SR_UIF){
		if(xQueueReceiveFromISR( draw_queue, &value, &xHigherPriorityTaskWoken)){
			dac_output(value);
		}
		TIM6->SR &= ~TIM_SR_UIF;
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}


