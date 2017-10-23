#ifndef RAND_H
#define RAND_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"

uint16_t rand(void);

void srand(uint16_t seed);

#endif