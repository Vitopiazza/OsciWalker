#include "input_thread.h"

static uint8_t config[5] = {0xa0, 0x77, 0x00, 0x00, 0x98};
static uint8_t accel[6];
static uint8_t read_accel[1] = {0xa8};

static int32_t accel_x;
static int32_t accel_y;
static int32_t accel_z;

static int16_t accel_x_d;
static int16_t accel_y_d;
static int16_t accel_z_d;

static uint8_t start_delay;

void INPUT_thread(void* arg)
{
	static uint32_t value;
	//usb_putchar('C');
	nblocking_write(config, 5, 0);
	//usb_putchar('D');
	
	nblocking_write(read_accel, 1, 1);
	//usb_putchar('W');
	nblocking_read(accel, 6, 0);
	//usb_putchar('R');
	
	start_delay = 100;
	while(start_delay){
		accel_x_d = (accel[1] << 8) | accel[0];
		accel_y_d = (accel[3] << 8) | accel[2];
		accel_z_d = (accel[5] << 8) | accel[4];
	
		vTaskDelay(2);
		start_delay--;
	}
	
	for(;;){
		//usb_putchar('B');
		nblocking_write(read_accel, 1, 1);
		//usb_putchar('W');
		nblocking_read(accel, 6, 0);
		//usb_putchar('R');
		accel_x = (int16_t)((accel[1] << 8) | accel[0]) - (int32_t)accel_x_d;
		accel_y = (int16_t)((accel[3] << 8) | accel[2]) - (int32_t)accel_y_d;
		accel_z = (int16_t)((accel[5] << 8) | accel[4]) - (int32_t)accel_z_d;
		
		if(accel_z > 8192){
			value = EV_UP;
			xQueueSendToBack(input_queue, &value, 0);
			usb_putstring("UP\r\n");
		}else if(accel_z < -8192){
			value = EV_DOWN;
			xQueueSendToBack(input_queue, &value, 0);
			usb_putstring("DOWN\r\n");
		}
		
		accel_x_d += (int16_t)accel_x/(1 << FILTER_FACTOR);
		accel_y_d += (int16_t)accel_y/(1 << FILTER_FACTOR);
		accel_z_d += (int16_t)accel_z/(1 << FILTER_FACTOR);
		/*
		if(start_delay){
			start_delay--;
		}else{
			start_delay = 100;
			usb_putint(accel_x);
			usb_putchar(',');
			usb_putint(accel_y);
			usb_putchar(',');
			usb_putint(accel_z);
			usb_putchar('\r');
			usb_putchar('\n');
			usb_putint(accel_x_d);
			usb_putchar(',');
			usb_putint(accel_y_d);
			usb_putchar(',');
			usb_putint(accel_z_d);
			usb_putchar('\r');
			usb_putchar('\n');
			usb_putint((int16_t)((accel[1] << 8) | accel[0]));
			usb_putchar(',');
			usb_putint((int16_t)((accel[3] << 8) | accel[2]));
			usb_putchar(',');
			usb_putint((int16_t)((accel[5] << 8) | accel[4]));
			usb_putchar('\r');
			usb_putchar('\n');
		}*/
		
		vTaskDelay(2);
	}
}
