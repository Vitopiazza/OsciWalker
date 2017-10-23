#ifndef _INPUT_THREAD_H
#define _INPUT_THREAD_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"
#include "queue.h"
#include "task.h"

#include "i2c.h"
#include "usart.h"

#include "defines.h"

#define INPUT_THREAD_STACK 256
#define INPUT_THREAD_PRIORITY tskIDLE_PRIORITY + 3

//Duplicate from game.h
#define EV_NONE		0
#define EV_UP		1
#define EV_DOWN		2

#define FILTER_FACTOR 8

void INPUT_thread(void* arg);

#endif