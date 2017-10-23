#include "usb_thread.h"
/*
static uint8_t read_uint(char s[], uint8_t* pos, uint8_t max_pos, uint32_t* value)
{
	*value = 0;
	while(s[*pos] >= 0x30 && s[*pos] <= 0x39 && *pos < max_pos){
		*value *= 10;
		*value += (s[*pos] - 0x30);
		*pos += 1;
	}
	if(s[*pos] == ' ' || *pos >= max_pos){
		*pos += 1;
		return 1;
	}
	return 0;
}

static uint8_t interpret_message(char* s, uint8_t size, uint32_t* val_1, uint32_t* val_2)
{
	static char header[MESSAGE_IN_HEADER];
	static uint8_t pos;
	if(size >= 2){
		header[0] = s[0];
		header[1] = s[1];
		if(header[0] == 'S' && header[1] == 'T'){
			if(size == 2){
				return CONF_COMMAND_GET_TIME;
			}else if (size >= 4){
				if(s[2] != ' '){
					return CONF_COMMAND_PARSE_ERROR;
				}else{
					pos = 3;
					if(read_uint(s, &pos, size, val_1)){
						return CONF_COMMAND_SET_TIME;
					}else{
						return CONF_COMMAND_PARSE_ERROR;
					}
				}
			}else{
				return CONF_COMMAND_PARSE_ERROR;
			}
		}else if(header[0] == 'D' && header[1] == 'T'){
			if(size == 2){
				return CONF_COMMAND_DATA_TIME_READ;
			}else if (size >= 4){
				if(s[2] != ' '){
					return CONF_COMMAND_PARSE_ERROR;
				}else{
					pos = 3;
					if(read_uint(s, &pos, size, val_1)){
						if(pos >= size){
							*val_2 = 0;
							return CONF_COMMAND_DATA_TIME_WRITE;
						}else if(read_uint(s, &pos, size, val_2)){
							return CONF_COMMAND_DATA_TIME_WRITE;
						}else{
							return CONF_COMMAND_PARSE_ERROR;
						}
					}else{
						return CONF_COMMAND_PARSE_ERROR;
					}
				}
			}else{
				return CONF_COMMAND_PARSE_ERROR;
			}
		}else if(header[0] == 'D' && header[1] == 'A'){
			if(size < 4){
				return CONF_COMMAND_PARSE_ERROR;
			}else{
				if(s[2] != ' '){
					return CONF_COMMAND_PARSE_ERROR;
				}else{
					pos = 3;
					if(read_uint(s, &pos, size, val_1)){
						return CONF_COMMAND_DATA_ACQUISITION;
					}else{
						return CONF_COMMAND_PARSE_ERROR;
					}
				}
			}
		}else if(header[0] == 'L' && header[1] == 'E'){
			if(size < 4){
				return CONF_COMMAND_PARSE_ERROR;
			}else{
				if(s[2] != ' '){
					return CONF_COMMAND_PARSE_ERROR;
				}else{
					pos = 3;
					if(read_uint(s, &pos, size, val_1)){
						return CONF_COMMAND_DISPLAY_ENABLE;
					}else{
						return CONF_COMMAND_PARSE_ERROR;
					}
				}
			}
		}else if(header[0] == 'S' && header[1] == 'D'){
			if(size < 4){
				return CONF_COMMAND_PARSE_ERROR;
			}else{
				if(s[2] != ' '){
					return CONF_COMMAND_PARSE_ERROR;
				}else{
					pos = 3;
					if(read_uint(s, &pos, size, val_1)){
						return CONF_COMMAND_SAVE_SD;
					}else{
						return CONF_COMMAND_PARSE_ERROR;
					}
				}
			}
		}else if(header[0] == 'R' && header[1] == 'S'){
			if(size == 2){
				*val_2 = 1;
				return CONF_COMMAND_SD_READ;
			}else if(size >= 4){
				if(s[2] != ' '){
					return CONF_COMMAND_PARSE_ERROR;
				}else{
					pos = 3;
					if(read_uint(s, &pos, size, val_1)){
						*val_2 = 0;
						return CONF_COMMAND_SD_READ;
					}else{
						return CONF_COMMAND_PARSE_ERROR;
					}
				}
			}else{
				return CONF_COMMAND_PARSE_ERROR;
			}
		}else{
			return CONF_COMMAND_PARSE_ERROR;
		}
	}else{
		return CONF_COMMAND_PARSE_ERROR;
	}
} 
*/
void USB_thread(void* arg)
{
	//Uart turned on in main thread.
	//static usb_queue_message in_queue_message;
	//static all_queue_messages queue_messages;
	
	//static uint8_t message_type;
	//static uint32_t val_1;
	//static uint32_t val_2;
	
	//static uint8_t time_counter = 0;
	//static uint8_t time_report_period = DEFAULT_TIME_PERIOD;
	//static uint32_t previous_time = 0xffffffff;
	for(;;){
		usb_putstring("Hi.\r\n");
		vTaskDelay(200);
	}
	
	/*
	static  portBASE_TYPE queued;
	for(;;) {
		xQueueReceive(USB_in_queue, &queue_messages.usb, portMAX_DELAY);
		//battery_adc_post_trigger();
		switch(queue_messages.usb.type){
			case USB_QUEUE_TYPE_UART_IN:
				//rf_adc_trigger(queue_messages.usb.data.uart_in.size, 1);
				message_type = interpret_message(queue_messages.usb.data.uart_in.message, queue_messages.usb.data.uart_in.size, &val_1, &val_2);
				queued = pdTRUE;
				switch(message_type){
					case CONF_COMMAND_DATA_TIME_WRITE: //Set the time reporting variable
						time_report_period = (uint8_t)val_2;
					case CONF_COMMAND_DATA_TIME_READ:					
					case CONF_COMMAND_SET_TIME: 		
					case CONF_COMMAND_GET_TIME:			
					case CONF_COMMAND_DATA_ACQUISITION:
					case CONF_COMMAND_SAVE_SD:
					case CONF_COMMAND_SD_READ:
					case CONF_COMMAND_DISPLAY_ENABLE:
						queue_messages.conf.type = CONF_QUEUE_TYPE_COMMAND;
						queue_messages.conf.data.command.type = message_type;
						queue_messages.conf.data.command.val_1 = val_1;
						queue_messages.conf.data.command.val_2 = val_2;
						xQueueSendToBack(CONF_in_queue, &queue_messages.conf, 0);
					break;
					case CONF_COMMAND_PARSE_ERROR:
					default:
						usb_putstring("<ERROR INVALID MESSAGE FORMAT>\r");
				}
				if (queued == errQUEUE_FULL){
					usb_putstring("<COULDN'T PROCESS COMMAND, PLEASE RESEND.>\r");
				}
			break;
			case USB_QUEUE_TYPE_ERROR_IN:
				switch(queue_messages.usb.data.error_in.error){
					case USB_ERROR_IN_INVALID:
						usb_putstring("<ERROR INVALID MESSAGE>\r");
					break;
					case USB_ERROR_IN_OVERFLOW:
						usb_putstring("<ERROR MESSAGE OVERFLOW>\r");
					break;
					default:
						usb_putstring("<INTERNAL ERROR>\r");
				}
			break;
			case USB_QUEUE_TYPE_RF_OUT:
				xSemaphoreTake(usb_out_mutex, portMAX_DELAY);
				usb_putstring_direct("<R ");
				usb_putint_direct(queue_messages.usb.data.rf_out.value);
				usb_putstring_direct(">\r");
				if(previous_time != queue_messages.usb.data.rf_out.time){
					time_counter++;
					if(time_counter > time_report_period){
						time_counter = 0;
						usb_putstring_direct("<T ");
						usb_putuint_direct(queue_messages.usb.data.rf_out.time);
						usb_putstring_direct(">\r");
					}
				}
				xSemaphoreGive(usb_out_mutex);
			break;
			case USB_QUEUE_TYPE_STATUS_OUT:
				xSemaphoreTake(usb_out_mutex, portMAX_DELAY);
				switch(queue_messages.usb.data.status_out.status){
					case USB_STATUS_OUT_NO_SD:
						usb_putstring_direct("<NO SD>\r");
					break;
					case USB_STATUS_OUT_DONE:
						usb_putstring_direct("<DONE>\r");
					break;
					case USB_STATUS_OUT_SD_ON:
						usb_putstring_direct("<SD ON>\r");
					break;
					case USB_STATUS_OUT_DA_ON:
						usb_putstring_direct("<DA ON>\r");
					break;
					case USB_STATUS_OUT_NO_FILE:
						usb_putstring_direct("<NO FILE>\r");
					break;
					case USB_STATUS_OUT_BATTERY_LOW:
						usb_putstring_direct("<BATTERY LOW>\r");
					break;
					case USB_STATUS_OUT_FILE_NUMBER:
						usb_putstring_direct("<FILES ");
						usb_putuint_direct(queue_messages.usb.data.status_out.val_1);
						usb_putstring_direct(">\r");
					break;
					case USB_STATUS_OUT_OK_0:
						usb_putstring_direct("<OK>\r");
					break;
					case USB_STATUS_OUT_OK_1:
						usb_putstring_direct("<OK ");
						usb_putuint_direct(queue_messages.usb.data.status_out.val_1);
						usb_putstring_direct(">\r");
					break;
					case USB_STATUS_OUT_OK_2:
						usb_putstring_direct("<OK ");
						usb_putuint_direct(queue_messages.usb.data.status_out.val_1);
						usb_putchar_direct(' ');
						usb_putuint_direct(queue_messages.usb.data.status_out.val_2);
						usb_putstring_direct(">\r");
					break;
					default:
						usb_putstring_direct("<INTERNAL ERROR>\r");
					break;
				}
				xSemaphoreGive(usb_out_mutex);
			break;
			case USB_QUEUE_TYPE_ERROR_OUT:
				xSemaphoreTake(usb_out_mutex, portMAX_DELAY);
					usb_putstring_direct("<ERROR ");
					usb_putstring_direct_const(queue_messages.usb.data.error_out.message);
					usb_putstring_direct(">\r");
				xSemaphoreGive(usb_out_mutex);
			break;
			default:
				usb_putstring("<INTERNAL ERROR>\r");
		}
	}*/
}