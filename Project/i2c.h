#ifndef I2C_H
#define I2C_H 1

#include "FreeRTOS.h"
#include "stm32f30x.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "defines.h"
#include "cgpio.h"
#include "usart.h"

#define I2C_INTERRUPT_PRIORITY 8

void init_i2c(void);

void start_read_operation(uint8_t* dest, uint8_t size, uint8_t reload);
void nblocking_read(uint8_t* dest, uint8_t size, uint8_t reload);

void start_write_operation(uint8_t* src, uint8_t size, uint8_t reload);
void nblocking_write(uint8_t* src, uint8_t size, uint8_t reload);

#endif