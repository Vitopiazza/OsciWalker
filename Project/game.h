#ifndef GAME_H
#define GAME_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"
#include "queue.h"
#include "task.h"

#include "game_settings.h"
#include "graphics.h"
#include "rand.h"

#define DRAW_QUEUE_LENGTH 512

extern uint8_t last_event;
extern xQueueHandle draw_queue;

void game_loop(void);


#endif