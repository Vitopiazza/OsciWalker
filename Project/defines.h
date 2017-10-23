#ifndef DEFINES_H
#define DEFINES_H 1

#include "assert.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define INPUT_QUEUE_LENGTH 1
extern xQueueHandle input_queue;

#define DEBUG 1

//Uart
#define UART_INTERRUPT_PRIORITY 12
#define RTC_INTERRUPT_PRIORITY 6
#define BUTTON_INTERRUPT_PRIORITY 0
#define BUFFER_SIZE 32 //Max receive message size -1 (due to appended null char)
#define PRINT_MAX_NUMBER 10

#define MESSAGE_IN_HEADER 2

#define DEFAULT_TIME_PERIOD 0

//Power
#define BATTERY_CRITICAL_LEVEL 2048
#define BATTERY_MEASURE_INTERVAL 1280 //10s

//RF power
#define ADC_INTERRUPT_PRIORITY 7
#define DEFAULT_RF_MEASURE_PERIOD 	127	//1s
#define RF_METER_ENABLE_TIME		2	//16ms

#define ATTENUATION_LOW_LEVEL			1536
#define ATTENUATION_LOW_MINIMUM_TIME	1024
#define ATTENUATION_HIGH_LEVEL			2340

//SD card
#define SD_PRESENT_INTERRUPT_PRIORITY 15
#define SD_PRESENT_START_TIME	512 // 4s
#define SD_PRESENT_WAIT_TIME	16	// 128ms

//Display
#define UPDATE_TIMER		2000
#define UPDATE_TIMER_PSC	31

#define LED_PWM_ARR			2048
#define LED_PWM_PSC			7
#define LED_PWM_VAL			1536
#define LED_PWM_DARK		LED_PWM_ARR

#define LED_BLINK_PERIOD	100

//GPIO
//Names

//KEY GPIOs
#define EXTI_BUTTON			2
#define EXTI_BUTTON_CLEAR	SYSCFG_EXTICR3_EXTI9
#define EXTI_BUTTON_MASK	SYSCFG_EXTICR3_EXTI9_PA
#define BUTTON_IRQn			EXTI9_5_IRQn

#define BUTTON_IRQHandler	EXTI9_5_IRQHandler

#define BUTTON_IMR			EXTI_IMR_MR9	
#define BUTTON_EMR			EXTI_EMR_MR9
#define BUTTON_RTSR			EXTI_RTSR_TR9
#define BUTTON_FTSR			EXTI_FTSR_TR9
#define BUTTON_PR			EXTI_PR_PR9

#define EXTI_SD_PRESENT			0
#define EXTI_SD_PRESENT_CLEAR	SYSCFG_EXTICR1_EXTI2
#define EXTI_SD_PRESENT_MASK	SYSCFG_EXTICR1_EXTI2_PB
#define SD_PRESENT_IRQn			EXTI2_IRQn

#define SD_PRESENT_IRQHandler	EXTI2_IRQHandler

#define SD_PRESENT_IMR			EXTI_IMR_MR2	
#define SD_PRESENT_EMR			EXTI_EMR_MR2
#define SD_PRESENT_RTSR			EXTI_RTSR_TR2
#define SD_PRESENT_FTSR			EXTI_FTSR_TR2
#define SD_PRESENT_PR			EXTI_PR_PR2

//RF POWER GPIOs

#define PORT_RFADC			GPIOB
#define PIN_RFADC			13

#define PORT_RFA0			GPIOB
#define PIN_RFA0			4

#define PORT_RFA1			GPIOB
#define PIN_RFA1			3

#define PORT_RFA2			GPIOA
#define PIN_RFA2			15

#define PORT_RFA3			GPIOA
#define PIN_RFA3			12

#define PORT_RFA4			GPIOA
#define PIN_RFA4			11

#define PORT_RFEN			GPIOB
#define PIN_RFEN			14

// Misc GPIOs

#define PORT_BATTERY		GPIOB
#define PIN_BATTERY			1

#define PORT_PG				GPIOH
#define PIN_PG				0

// SD card present interrupt
#define PORT_SD_PRESENT		GPIOB
#define PIN_SD_PRESENT		2

// SPI GPIOS
#define PORT_MOSI			GPIOA
#define PIN_MOSI			7

#define PORT_MISO			GPIOA
#define PIN_MISO			6

#define PORT_CLK			GPIOA
#define PIN_CLK				5

#define PORT_EN_SD			GPIOA
#define PIN_EN_SD			4

// Output to regulator enable
#define PORT_BUTTON			GPIOA
#define PIN_BUTTON			10

// Pushbutton interrupt input
#define PORT_BUTTON_INT		GPIOA
#define PIN_BUTTON_INT		9

//Display GPIOs
#define PORT_DISP_A 		GPIOB
#define PIN_DISP_A			6	

#define PORT_DISP_B 		GPIOB
#define PIN_DISP_B			7	

#define PORT_DISP_C 		GPIOB
#define PIN_DISP_C			8	

#define PORT_DISP_D 		GPIOB
#define PIN_DISP_D			5	

#define PORT_DISP_E 		GPIOA
#define PIN_DISP_E			8	

#define PORT_DISP_F 		GPIOB
#define PIN_DISP_F			9

#define PORT_DISP_G 		GPIOC
#define PIN_DISP_G			13

#define PORT_DISP_DP 		GPIOB
#define PIN_DISP_DP			15

#define PORT_DISP_1 		GPIOA
#define PIN_DISP_1 			0

#define PORT_DISP_2		 	GPIOA  
#define PIN_DISP_2 			1

#define PORT_DISP_3	 		GPIOA
#define PIN_DISP_3 			2

#define PORT_DISP_4 		GPIOA
#define PIN_DISP_4 			3

#define PORT_LED_1			GPIOH
#define PIN_LED_1			1

#define	PORT_LED_2			GPIOB
#define PIN_LED_2			12

#endif