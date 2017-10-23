#ifndef DAC_H
#define DAC_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"

#include "defines.h"
#include "cgpio.h"

void init_dac(void);
	
void dac_output(uint32_t value);

#endif
