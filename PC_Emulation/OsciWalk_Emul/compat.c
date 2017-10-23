#include "FreeRTOS.h"

xQueueHandle draw_queue;
uint32_t * pixels;

#define ADC_MAX 4096

void xQueueSendToBack(xQueueHandle draw_queue, uint32_t* value, int max_delay){
	int x = ((*value) >> 0)&0xffff;
	int y = ((*value) >> 16)&0xffff;

	pixels[(511 - y*512/ADC_MAX)*512 + x*512/ADC_MAX] += 0x003f3f3f;
}
