#include "usart.h"

char usart_usb_on = 0;

uint8_t position;
uint8_t receiving_state;

xSemaphoreHandle usb_out_mutex;

void init_usart(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	 
	//U3 TX 10
	GPIO_ALTERNATE(GPIOB, 10);
	GPIO_PUSHPULL(GPIOB, 10);
	GPIO_SPEED_40M(GPIOB, 10);
	GPIO_FUNCTION(GPIOB, 10, 0x07);
	
	//U3 RX 11
	GPIO_ALTERNATE(GPIOB, 11);
	GPIO_PUSHPULL(GPIOB, 11);
	GPIO_SPEED_40M(GPIOB, 11);
	GPIO_FUNCTION(GPIOB, 11, 0x07);
	
	//USART3 - USB
	USART3->CR1 |= (USART_CR1_RE | USART_CR1_TE); //Enable recieve and transmit
	//CR2 OK
	//CR3 OK//USART3->CR3 |= (USART_CR3_CTSE | USART_CR3_RTSE); //Enable Flow control
	USART3->BRR =0x271/2; //115.2k
	
	
	//usb_buffer init
	position = 0;
	receiving_state = USB_RECEIVER_STATE_IDLE;
	
	USART3->CR1 |= USART_CR1_UE;
	usart_usb_on = 1;
	
	//USART3 Interrupt
	//NVIC->IP[USART3_IRQn] = UART_INTERRUPT_PRIORITY << 4; 
	// >> 0x05 take top 3 bits = 32 bits per register, &0x1f - 32 bit mask
    ///NVIC->ISER[USART3_IRQn >> 0x05] = 0x01 << (USART3_IRQn & 0x1F); //Enable interrupt - can assing due to it being positive edge triggered
	
	//USART3->CR1 |= USART_CR1_RXNEIE; //Enable RX interrupt - here, so it doesn't cause a send before we enable uart_usb_on var
}

void start_usart_usb(void)
{
	//RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	USART3->CR1 |= USART_CR1_UE;
	usart_usb_on = 1;
	
	NVIC->ISER[USART3_IRQn >> 0x05] = 0x01 << (USART3_IRQn & 0x1F); //Enable interrupt - can assing due to it being positive edge triggered
}
void stop_usart_usb(void)
{
	NVIC->ICER[USART3_IRQn >> 0x05] = 0x01 << (USART3_IRQn & 0x1F); //Disable interrupt - can assing due to it being positive edge triggered
	USART3->CR1 &= ~USART_CR1_UE;
	//RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN;
	
	usart_usb_on = 0;
}

//Currently send is blocking, due to the fact that more probably isn't required
void usb_putchar_direct(char c)
{
	assert_param(usart_usb_on);
	
	//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	while(!(USART3->ISR & USART_ISR_TXE));
	
	USART3->TDR = (c);
}

void usb_putstring_direct(char* s)
{
	while(*s != '\0'){
		usb_putchar_direct(*(s++));
	}
}

void usb_putstring_direct_const(char const* s)
{
	while(*s != '\0'){
		usb_putchar_direct(*(s++));
	}
}
/*
void usb_putstring_const(char const* const sc)
{
	static char const* s;
	s = sc;
	while(*s != '\0'){
		usb_putchar_direct(*(s++));
	}
}
*/
void usb_putuint_direct(uint32_t number)
{
	static char s[PRINT_MAX_NUMBER];
	static uint32_t temp;
	static uint8_t i;
	i = 0;
	if(number == 0){
		usb_putchar_direct('0');
	}
	while(number){
		temp = number%10;
		number /= 10;
		s[i] = temp + 0x30;
		i++;
	}
	for(; i; i--){
		usb_putchar_direct(s[i-1]);
	}
}

void usb_putint_direct(int32_t number)
{
	if(number < 0){
		usb_putchar_direct('-');
		usb_putuint_direct((uint32_t)(-number));
	}else{
		usb_putuint_direct((uint32_t)(number));
	}
}

void usb_putchar(char c)
{
	xSemaphoreTake(usb_out_mutex, portMAX_DELAY);
	usb_putchar_direct(c);
	xSemaphoreGive(usb_out_mutex);
}

void usb_putstring(char* s)
{
	xSemaphoreTake(usb_out_mutex, portMAX_DELAY);
	usb_putstring_direct(s);
	xSemaphoreGive(usb_out_mutex);
}

void usb_putuint(uint32_t number)
{
	xSemaphoreTake(usb_out_mutex, portMAX_DELAY);
	usb_putuint_direct(number);
	xSemaphoreGive(usb_out_mutex);
}

void usb_putint(int32_t number)
{
	xSemaphoreTake(usb_out_mutex, portMAX_DELAY);
	usb_putint_direct(number);
	xSemaphoreGive(usb_out_mutex);
}

void USART3_IRQHandler(void)
{
	/*
	static char c;
	static usb_queue_message queue_message;
	static portBASE_TYPE xHigherPriorityTaskWoken;
    static portBASE_TYPE queued;
	xHigherPriorityTaskWoken = pdFALSE;
	queued = pdTRUE;
	
	if(USART3->SR & USART_SR_RXNE){
		//usb_putchar_direct('R');
		//usb_putchar_direct('\n');
		c = USART3->DR;
		if(receiving_state == USB_RECEIVER_STATE_RECEIVING){
			if(c == '<'){
				receiving_state = USB_RECEIVER_STATE_RECEIVING;
				position = 0;
			}else if(c == '>'){
				receiving_state = USB_RECEIVER_STATE_ENDING;
			}else if(c == '\r'){
				receiving_state = USB_RECEIVER_STATE_IDLE;
				queue_message.type = USB_QUEUE_TYPE_ERROR_IN;
				queue_message.data.error_in.error = USB_ERROR_IN_INVALID;
				queued = xQueueSendToBackFromISR(USB_in_queue, &queue_message, &xHigherPriorityTaskWoken);
				portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
			}else{
				if(position < BUFFER_SIZE - 1){
					queue_message.data.uart_in.message[position] = c;
					position++;
				}else{
					queue_message.type = USB_QUEUE_TYPE_ERROR_IN;
					queue_message.data.error_in.error = USB_ERROR_IN_OVERFLOW;
					queued = xQueueSendToBackFromISR(USB_in_queue, &queue_message, &xHigherPriorityTaskWoken);
					portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
					receiving_state = USB_RECEIVER_STATE_IDLE;
				}
			}
		}else if(receiving_state == USB_RECEIVER_STATE_IDLE){
			if(c == '<'){
				receiving_state = USB_RECEIVER_STATE_RECEIVING;
				position = 0;
			}else if(c == '\r'){
				queue_message.type = USB_QUEUE_TYPE_ERROR_IN;
				queue_message.data.error_in.error = USB_ERROR_IN_INVALID;
				queued = xQueueSendToBackFromISR(USB_in_queue, &queue_message, &xHigherPriorityTaskWoken);
				portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
			}
		}else if(receiving_state == USB_RECEIVER_STATE_ENDING){
			if(c == '\r'){
				queue_message.type = USB_QUEUE_TYPE_UART_IN;
				queue_message.data.uart_in.size = position;
				queued = xQueueSendToBackFromISR(USB_in_queue, &queue_message, &xHigherPriorityTaskWoken);
				portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
			}
			receiving_state = USB_RECEIVER_STATE_IDLE;
		}
		
		if(queued == errQUEUE_FULL){
			usb_putstring("<COULDN'T PROCESS COMMAND, PLEASE RESEND.>\r");
		}
	}
	*/	
}
