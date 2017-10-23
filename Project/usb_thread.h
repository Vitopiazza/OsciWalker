#ifndef USB_THREAD_H
#define USB_THREAD_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"
#include "queue.h"
#include "task.h"

#include "usart.h"

#include "defines.h"

#define USB_THREAD_STACK 256
#define USB_THREAD_PRIORITY tskIDLE_PRIORITY + 2

void USB_thread(void* arg);

#endif