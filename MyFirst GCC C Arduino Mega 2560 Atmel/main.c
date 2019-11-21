/*
 * MyFirst GCC C Arduino Mega 2560 Atmel.c
 *
 * Created: 11/14/2019 10:59:58 AM
 * Author : andre
 */ 

#include <avr/io.h>
#include "LoRaWAN.c"

#define TASK_LoRaWAN_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )

int main(void)
{
	lora_handler_create(TASK_LoRaWAN_TASK_PRIORITY);
	_lora_setup(void);
	
    while (1) 
    {
		// Application code goes in here
    }
	
	// Let the operating system take over :)
	vTaskStartScheduler();
}

