#ifndef _GAME_THREAD_H
#define _GAME_THREAD_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"
#include "queue.h"
#include "task.h"

#include "dac.h"
#include "timer.h"
#include "game.h"


#include "defines.h"

#define GAME_THREAD_STACK 256
#define GAME_THREAD_PRIORITY tskIDLE_PRIORITY + 1

void GAME_thread(void* arg);



#endif