/*
 * MyFirst GCC C Arduino Mega 2560 Atmel.c
 *
 * Created: 11/14/2019 10:59:58 AM
 * Author : andre
 */ 

#include "ATMEGA_FreeRTOS.h"
#include "ihal.h"
#include "lora_driver.h"
//#include <task.h>

// High priorities
#define LED_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )
// Medium priorities
#define LORAWAN_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )
// Low priorities

/* Task stack sizes*/
#define TASK_MY_TASK_STACK				( configMINIMAL_STACK_SIZE )

int main(void)
{
	hal_create(LED_TASK_PRIORITY); // Must be called first!! LED_TASK_PRIORITY must be a high priority in your system
	lora_driver_create(ser_USART1); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 (AKA COM PORT?)
	
    /* Replace with your application code */
    while (1) 
    {
    }
}



