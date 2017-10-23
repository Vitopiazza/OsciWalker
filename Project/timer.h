#ifndef TIMER_H
#define TIMER_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"
#include "stm32f30x_misc.h"

#include "defines.h"
#include "cgpio.h"

#define TIMER_INTERRUPT_PRIORITY 5

void init_timer(void);

void start_timer(void);

void stop_timer(void);


#endif