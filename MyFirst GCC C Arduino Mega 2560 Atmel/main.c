/*
* MyFirst GCC C Arduino Mega 2560 Atmel.c
*
* Created: 11/14/2019 10:59:58 AM
* Author : andre
*/
//
//#include "ATMEGA_FreeRTOS.h"
//#include "ihal.h"
//#include "lora_driver.h"
//#include "Controller/loraWAN.h"
////#include <task.h>
//
//// High priorities
//#define LED_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )
//// Medium priorities
//#define LORAWAN_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )
//// Low priorities
//
///* Task stack sizes*/
//#define TASK_MY_TASK_STACK				( configMINIMAL_STACK_SIZE )
//
//int main(void)
//{
//hal_create(LED_TASK_PRIORITY); // Must be called first! LED_TASK_PRIORITY must be a high priority in your system
//lora_driver_create(ser_USART1); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 (AKA COM PORT?)
//lora_handler_create(LORAWAN_TASK_PRIORITY);
//
/////* Replace with your application code */
////while (1)
////{
////// Application code goes in here
////}
//
//// Let the operating system take over :)
//vTaskStartScheduler();
//}


// TEST IS BOARD WORKS CODE
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
int main(void)
{
	DDRA |= _BV(DDA0) | _BV(DDA7); // Set PA0 and PA7 to output

	while(1)
	{
		PORTA ^= _BV(PA0); // Toggle PA0 (LED1)
		_delay_ms(100);
		PORTA ^= _BV(PA7); // Toggle PA7 (LED2)
		_delay_ms(200);
	}
}


