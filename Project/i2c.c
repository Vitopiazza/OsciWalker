#include "i2c.h"

uint8_t items_remaining;
uint8_t* current_array;

xSemaphoreHandle I2C_semaphore = NULL;

void init_i2c(void)
{
	I2C_semaphore = xSemaphoreCreateBinary();
	assert_param(I2C_semaphore);

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->CFGR3 &= ~RCC_CFGR3_I2C1SW;
	
	//I2C1 SCL B6
	GPIO_ALTERNATE(GPIOB, 6);
	//GPIO_PUSHPULL(GPIOB, 6);
	GPIO_SPEED_40M(GPIOB, 6);
	GPIO_FUNCTION(GPIOB, 6, 0x04);
	
	//I2C1 SDA B7
	GPIO_ALTERNATE(GPIOB, 7);
	//GPIO_PUSHPULL(GPIOB, 7);
	GPIO_SPEED_40M(GPIOB, 7);
	GPIO_FUNCTION(GPIOB, 7, 0x04);
	
	I2C1->TIMINGR |= (0x1 << 28) | (0x4 << 20) | (0x2 << 16) | (0xf << 8) | (0x13 << 0); //PRESC, SCLDEL, SDADEL, SCLH, SCLL
	I2C1->CR1 |= I2C_CR1_RXIE | I2C_CR1_TXIE | I2C_CR1_NACKIE;
	
	NVIC->IP[I2C1_EV_IRQn] = I2C_INTERRUPT_PRIORITY << 4; 
	// >> 0x05 take top 3 bits = 32 bits per register, &0x1f - 32 bit mask
    NVIC->ISER[I2C1_EV_IRQn >> 0x05] = 0x01 << (I2C1_EV_IRQn & 0x1F); //Enable interrupt - can assing due to it being positive edge triggered
	
	I2C1->CR1 |= I2C_CR1_PE;
}


void start_read_operation(uint8_t* dest, uint8_t size, uint8_t reload)
{
	current_array = dest;
	items_remaining = size;

	I2C1->CR2 |= I2C_CR2_RD_WRN;
	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= ((uint32_t)size << 16) | 0x32;
	if(reload){
		I2C1->CR2 &= ~I2C_CR2_AUTOEND;
	}else{
		I2C1->CR2 |= I2C_CR2_AUTOEND;
	}
	

	I2C1->CR2 |= I2C_CR2_START;
}

void nblocking_read(uint8_t* dest, uint8_t size, uint8_t reload)
{
	while(xSemaphoreTake(I2C_semaphore, 0) == pdTRUE); //Make sure it is empty
	start_read_operation(dest, size, reload);
	xSemaphoreTake(I2C_semaphore, portMAX_DELAY);
}

void start_write_operation(uint8_t* src, uint8_t size, uint8_t reload)
{
	current_array = src;
	items_remaining = size;

	I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= ((uint32_t)size << 16) | 0x32;
	if(reload){
		I2C1->CR2 &= ~I2C_CR2_AUTOEND;
	}else{
		I2C1->CR2 |= I2C_CR2_AUTOEND;
	}
	
	I2C1->CR2 |= I2C_CR2_START;
}

void nblocking_write(uint8_t* src, uint8_t size, uint8_t reload)
{
	while(xSemaphoreTake(I2C_semaphore, 0) == pdTRUE); //Make sure it is empty
	start_write_operation(src, size, reload);
	xSemaphoreTake(I2C_semaphore, portMAX_DELAY);
}

void unblock_thread()
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(I2C_semaphore, &xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void I2C1_EV_IRQHandler(void)
{
	if(I2C1->ISR & I2C_ISR_NACKF){
		if(items_remaining){
			items_remaining = 0;
			unblock_thread();
		}
		usb_putchar_direct('N');
		I2C1->ICR |= I2C_ICR_NACKCF;
	}else if(I2C1->ISR & I2C_ISR_RXNE){
		*current_array = I2C1->RXDR;
		if(items_remaining){
			current_array++;
			items_remaining--;
		}
		if(!items_remaining){
			unblock_thread();
		}
	}else if(I2C1->ISR & I2C_ISR_TXIS){
		I2C1->TXDR = *current_array;
		if(items_remaining){
			current_array++;
			items_remaining--;
		}
		if(!items_remaining){
			unblock_thread();
		}
	}else{
		usb_putchar_direct('I');
	}
}