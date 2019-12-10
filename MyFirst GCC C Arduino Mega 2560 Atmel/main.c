#include <stdio.h>

/*#include "FreeRTOS.h"*/
/*---instead---*/
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <event_groups.h>

#include <FreeRTOSTraceDriver.h>
/*#include "FreeRTOS/FreeRTOSTraceDriver/FreeRTOSTraceDriver.h"*/
/*---*/

/*---write to terminal---*/
#include <stdio.h>
#include <stdio_driver.h>

#include <avr/io.h>
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

//---

#include "task.h"

#include "Controller/loraWAN.h"
#include "LorawanTask.h"
#include "Controller/Co2Sensor.h"
#include "Controller/LightSensor.h"
#include "Controller/Co2Sensor.h"

/* Priorities at which the tasks are created. */
#define LED_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 ) /*Must be highest priority*/
#define LORAWAN_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define	TASK_MY_SECOND_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define	TASK_MY_THIRD_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define	TASK_MY_FORTH_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define	TASK_MY_FIFTH_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )

/* Task stack sizes*/
#define	TASK_MY_SECOND_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_THIRD_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_FORTH_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_FIFTH_TASK_STACK		( configMINIMAL_STACK_SIZE )

/* Task Handles */
TaskHandle_t _taskSecondHandle = NULL;
TaskHandle_t _taskThirdHandle = NULL;
TaskHandle_t _taskForthHandle = NULL;
TaskHandle_t _taskFifthHandle = NULL;

/*Semaphores*/
//need separate result semaphore for sensors not to take semaphores back because controller can only take and wait for one semaphore at a time
//In example. If one sensor task is finished much bore another, then it would have to wait long enough for the other task to finish or the other task semaphore to timeout for the controller task to request and collect the semaphore of the finished waiting before being taken back again by the finished waiting task. better for maintainability to use separate tasks.
//insures that the semaphore was taken by distinguishing between direction of communication. Doesn't mistake a semaphore not being taken as that reply is already present
//No not  to wait for the receiver to take the semaphore or for the receiver to be higher priority to immediate take it.

//creates an eventgroup. Basically a set of semaphores or flags. Holds a set of bits. Each bit is a flag.
//Allays you to wait for multiple flags to be set simultaneously.
//replaces the use of semaphores. We still use separate semaphores for each communication direction. To avoid having to wait for others task to take a semaphore before taking it back.
//Otherwise you can only be sure that the semaphore was taken by th other task by waiting forever or by making an additional flag that is was taken. This would give you two flags anyway.

EventGroupHandle_t contrlEvtGrp;

//declaration

TickType_t xTimeOnEntering;

// --------------------------------------------------------------------------------------
void taskMySeccondTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		/*SemaphoreHandle_t localSemaphore = co2xSemaphore;
		xSemaphoreTake(localSemaphore, portMAX_DELAY);*/
		xEventGroupWaitBits(contrlEvtGrp, 0b00000001, pdTRUE, pdTRUE, portMAX_DELAY);
		vTaskDelay(400/portTICK_PERIOD_MS);
		getCo2SensorMeasurement();
		/*xSemaphoreGive(co2Result);*/
		xEventGroupSetBits(contrlEvtGrp, 0b00010000);
	}
}

// --------------------------------------------------------------------------------------
void taskMyThirdTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		/*SemaphoreHandle_t localSemaphore = lightxSemaphore;
		xSemaphoreTake(localSemaphore, portMAX_DELAY);*/
		xEventGroupWaitBits(contrlEvtGrp, 0b00000010, pdTRUE, pdTRUE, portMAX_DELAY);
		vTaskDelay(700/portTICK_PERIOD_MS);
		getLightSensorMeasurement();
		/*xSemaphoreGive(lightResult);*/
		xEventGroupSetBits(contrlEvtGrp, 0b00100000);
	}
}

// --------------------------------------------------------------------------------------
void taskMyForthTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		/*SemaphoreHandle_t localSemaphore = temperatureHumidityxSemaphore;
		xSemaphoreTake(localSemaphore, portMAX_DELAY);*/
		xEventGroupWaitBits(contrlEvtGrp, 0b00000100, pdTRUE, pdTRUE, portMAX_DELAY);
		vTaskDelay(1000/portTICK_PERIOD_MS);
		getTemperatureHumiditySensorMeasurement();
		/*xSemaphoreGive(temperatureHumidityResult);*/
		xEventGroupSetBits(contrlEvtGrp, 0b01000000);
	}
}

void taskMyFifthTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		/*SemaphoreHandle_t localSemaphore = xSemaphore;
		xSemaphoreTake(localSemaphore, portMAX_DELAY);*/
		xEventGroupWaitBits(contrlEvtGrp, 0b00001000, pdTRUE, pdTRUE, portMAX_DELAY);
		vTaskDelay(250/portTICK_PERIOD_MS);
		sendDataPackageToLorawan();
		/*xSemaphoreGive(xSemaphoreResult);*/
		xEventGroupSetBits(contrlEvtGrp, 0b10000000);
	}
}

// --------------------------------------------------------------------------------------
void main(void)
{
	
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
		
		
	
	/* Create the task, not storing the handle. */
	
	//Lorawan task
	lora_handler_create(LORAWAN_TASK_PRIORITY, contrlEvtGrp);

		/* Create the task, storing the handle. */
	xTaskCreate(
		taskMySeccondTask,       /* Function that implements the task. */
		"MySecondTask",          /* Text name for the task. */
		TASK_MY_SECOND_TASK_STACK,      /* Stack size in words, not bytes. */
		(void*)2,    /* Parameter passed into the task. */
		TASK_MY_SECOND_TASK_PRIORITY,/* Priority at which the task is created. */
		&_taskSecondHandle);      /* Used to pass out the created task's handle. */

	xTaskCreate(
		taskMyThirdTask,       /* Function that implements the task. */
		"MyThirdTask",          /* Text name for the task. */
		TASK_MY_THIRD_TASK_STACK,      /* Stack size in words, not bytes. */
		(void*)3,    /* Parameter passed into the task. */
		TASK_MY_THIRD_TASK_PRIORITY,/* Priority at which the task is created. */
		&_taskThirdHandle);      /* Used to pass out the created task's handle. */

	xTaskCreate(
		taskMyForthTask,       /* Function that implements the task. */
		"MyForthTask",          /* Text name for the task. */
		TASK_MY_FORTH_TASK_STACK,      /* Stack size in words, not bytes. */
		(void*)4,    /* Parameter passed into the task. */
		TASK_MY_FORTH_TASK_PRIORITY,/* Priority at which the task is created. */
		&_taskForthHandle);      /* Used to pass out the created task's handle. */

	xTaskCreate(
		taskMyFifthTask,       /* Function that implements the task. */
		"MyFifthTask",          /* Text name for the task. */
		TASK_MY_FIFTH_TASK_STACK,      /* Stack size in words, not bytes. */
		(void*)5,    /* Parameter passed into the task. */
		TASK_MY_FIFTH_TASK_PRIORITY,/* Priority at which the task is created. */
		&_taskFifthHandle);      /* Used to pass out the created task's handle. */

	// Let the operating system take over :)
	vTaskStartScheduler();
}