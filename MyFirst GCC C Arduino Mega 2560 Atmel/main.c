/*---instead---*/
#include <avr/io.h>
#include <avr/sfr_defs.h>

/*---write to terminal---*/
#include <stdio_driver.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/*---*/

//co2 sensor
#include <mh_z19.h>
//light sensor
#include <tsl2591.h>
//Humidity Temperature sensor
#include <hih8120.h>
//Lorawan driver
#include <ihal.h>

//--- FreeRTOS ---//
#include <ATMEGA_FreeRTOS.h>
#include <FreeRTOSTraceDriver.h>
#include "task.h"

//---Model---//
#include "Model/shared_variables.h"

//---Task---//
#include "Controller/loraWAN.h"
#include "Controller/Co2Sensor.h"
#include "Controller/LightSensor.h"
#include "Controller/Co2Sensor.h"

/*Declaration*/
EventGroupHandle_t contrlEvtGrp = NULL;

// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
void main(void)
{

	
	//initialize model
	data_co2 = create_co2_data(0, true);
	data_light = create_light_data(0, 0, 0, 0, true);
	data_temp_hum = create_temp_hum_data(0, 0, true);

	//initialize driver
	stdioCreate(0);
	//No interrupts allowed before this line. Use after finalizing initialization.
	sei();
	//---------
	//initialize light sensor driver
	tsl2591Create/*driver*/(tsl2591Callback/*Function for the driver to call/utillize*/);
	//initialize co2 sensor driver
	mh_z19_create(ser_USART3, my_co2_call_back);
	//initialize TempHum driver
	if ( HIH8120_OK == hih8120Create() )
	{
		puts("Driver created OK");
		// Driver created OK
		// Always check what hih8120Create() returns
	}
	
	//initialize Lorawan driver and on board led
	hal_create(LED_TASK_PRIORITY); // Must be called first! LED_TASK_PRIORITY must be a high priority in your system
	lora_driver_create(ser_USART1); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 (AKA COM PORT?)
	
	/*If i initialize the semaphores inside of the controller task (taskMyTask()) then they will not work. Even though that they are declared before at the top of the .c file or module */
	puts("before create semaphores");
	
	//Initialize controller eventgroup
	contrlEvtGrp = xEventGroupCreate();
	/* Was the event group created successfully? */
	if( contrlEvtGrp == NULL )
	{
		printf("The event group was not created because there was insufficient FreeRTOS heap available.");
	}
	else
	{
		printf("The event group was created.");
	}
	
	/* Create the task, storing the handle. */
	
	//Lorawan task
	lora_handler_create(LORAWAN_TASK_PRIORITY, contrlEvtGrp);

	createCo2Task();	
	
	createLightTask();

	createTempHumTask();

	// Let the operating system take over :)
	vTaskStartScheduler();
}