#include "stm32f30x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//Other files
#include "defines.h"
#include "cgpio.h"

#include "usart.h"
#include "timer.h"
#include "dac.h"
#include "i2c.h"
//#include "rtc.h"

#include "usb_thread.h"
#include "game_thread.h"
#include "input_thread.h"

xQueueHandle draw_queue = NULL;
xQueueHandle input_queue = NULL;

void enable_gpio_clocks(void)
{
	//enable clocks to ports
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	RCC->AHBENR |= RCC_AHBENR_GPIODEN;
}

void init_gpio(void)
{
	//ENABLE SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/*GPIO_RESET(PORT_BUTTON, PIN_BUTTON);
	GPIO_OUTPUT(PORT_BUTTON, PIN_BUTTON);
	GPIO_PULLUP(PORT_BUTTON, PIN_BUTTON);
	GPIO_SPEED_400K(PORT_BUTTON, PIN_BUTTON);
	GPIO_FUNCTION(PORT_BUTTON, PIN_BUTTON, 0x00);
	
	GPIO_RESET(PORT_RFEN, PIN_RFEN);
	GPIO_OUTPUT(PORT_RFEN, PIN_RFEN);
	GPIO_NOPULL(PORT_RFEN, PIN_RFEN);
	GPIO_SPEED_10M(PORT_RFEN, PIN_RFEN);
	GPIO_FUNCTION(PORT_RFEN, PIN_RFEN, 0x00);*/

}

void configure_interrupts(void)
{
	//Got values from misc.c
	//Priorities 0x700 to 0x300, Group_0 = 0x700 - no priority; Group_4 = 0x300 - all priority
	SCB->AIRCR = 0x05FA0000 | 0x300;
}

void enter_sleep(void)
{
	//__WFI();
}

int main ( void )
{
	//SCB->SCR &= ~SCB_SCR_SLEEPDEEP;
	PWR->CR &= ~PWR_CR_LPSDSR;
	
	configure_interrupts();
	enable_gpio_clocks();
	init_gpio();
	
	usb_out_mutex = xSemaphoreCreateMutex();
	assert_param(usb_out_mutex);
	draw_queue = xQueueCreate( DRAW_QUEUE_LENGTH, sizeof( uint32_t ) );
	assert_param(draw_queue);
	input_queue = xQueueCreate( INPUT_QUEUE_LENGTH, sizeof( uint32_t ) );
	assert_param(input_queue);
	
	//init_rtc();
	init_usart();	
	init_timer();
	init_dac();
	init_i2c();
	
	uint32_t reset_vector = RCC->CSR;
	usb_putstring_direct("\nR-");
	if(reset_vector & RCC_CSR_LPWRRSTF){// Low-Power reset flag
		usb_putchar_direct('L');
	}
	if(reset_vector & RCC_CSR_OBLRSTF){ // Option Bytes Loader reset flag
		usb_putchar_direct('O');
	}
	if(reset_vector & RCC_CSR_PINRSTF){ // PIN reset flag
		usb_putchar_direct('N');
	}
	if(reset_vector & RCC_CSR_PORRSTF){ // POR/PDR reset flag
		usb_putchar_direct('P');
	}
	if(reset_vector & RCC_CSR_SFTRSTF){ // Software Reset flag
		usb_putchar_direct('S');
	}
	if(reset_vector & RCC_CSR_IWDGRSTF){ // Independent Watchdog reset flag
		usb_putchar_direct('I');
	}
	if(reset_vector & RCC_CSR_WWDGRSTF){ // Window watchdog reset flag
		usb_putchar_direct('W');
	}
	usb_putchar_direct('\n');
	RCC->CSR |= RCC_CSR_RMVF; // Reset flags	
	
	
	xTaskCreate( USB_thread, ( signed char const * const ) "USB", USB_THREAD_STACK, NULL, USB_THREAD_PRIORITY, NULL );
	xTaskCreate( GAME_thread, ( signed char const * const ) "GAME", GAME_THREAD_STACK, NULL, GAME_THREAD_PRIORITY, NULL );
	xTaskCreate( INPUT_thread, ( signed char const * const ) "INPUT", INPUT_THREAD_STACK, NULL, INPUT_THREAD_PRIORITY, NULL );
	
	start_timer();
	
	//Wait till everything is done untill enabling the powerdown
	//init_extint();
	
	vTaskStartScheduler();
	
	for(;;);
}

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;
	for(;;);
}

void vApplicationMallocFailedHook( void )
{
	for(;;);
}

void vApplicationIdleHook( void )
{
	enter_sleep();
}

void rtos_assert_failed(volatile char* file, volatile unsigned int line){
	volatile uint8_t __attribute__ ((unused)) i;
	i = 0;
	taskDISABLE_INTERRUPTS();
	for( ;; );
}	

//# ifdef USE_FULL_ASSERT
void assert_failed(volatile uint8_t* file, volatile uint32_t line)
{
	volatile uint8_t __attribute__ ((unused)) i;
	i = 0;
/* Infinite loop */
/* Use GDB to find out why we 're here */
	for( ;; );
}
//# endif


/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    assert_param(0);
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    assert_param(0);
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    assert_param(0);
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    assert_param(0);
}
