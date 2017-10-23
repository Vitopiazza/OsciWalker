#ifndef USART_H
#define USART_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"
#include "semphr.h"
#include "queue.h"
//Other files
#include "defines.h"

#include "cgpio.h"

extern xSemaphoreHandle usb_out_mutex;

void init_usart(void);

void start_usart_usb(void);
void stop_usart_usb(void);

void usb_putchar_direct(char c);
void usb_putstring_direct(char* s);
void usb_putstring_direct_const(char const* s);
void usb_putuint_direct(uint32_t number);
void usb_putint_direct(int32_t number);

void usb_putchar(char c);
void usb_putstring(char* s);
//void usb_putstring_const(char const* const sc);
void usb_putuint(uint32_t number);
void usb_putint(int32_t number);

#define USB_RECEIVER_STATE_IDLE 0
#define USB_RECEIVER_STATE_RECEIVING 1
#define USB_RECEIVER_STATE_ENDING 2


void USART3_IRQHandler(void);


#endif