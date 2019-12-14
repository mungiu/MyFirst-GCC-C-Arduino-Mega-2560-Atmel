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
#include "Model/Header Files/final_data_bundle.h"
#include "Model/Header Files/temp_hum_data.h"
#include "Model/Header Files/light_data.h"
#include "Model/Header Files/co2_data.h"

//---Task---//
#include "Controller/loraWAN.h"
#include "Controller/Co2Sensor.h"
#include "Controller/LightSensor.h"
#include "Controller/Co2Sensor.h"

/* Priorities at which the tasks are created. */
#define LED_TASK_PRIORITY				( tskIDLE_PRIORITY + 2 ) /*Must be highest priority*/
#define LORAWAN_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define CO2_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define	LIGHT_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define	HUMIDITY_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )

/* Task stack sizes*/
#define	TASK_MY_Co2_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_LIGHT_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_HUMIDITY_TASK_STACK		( configMINIMAL_STACK_SIZE )

/* Task Handles */
TaskHandle_t _taskCo2Handle = NULL;
TaskHandle_t _taskLightHandle = NULL;
TaskHandle_t _taskHumidityHandle = NULL;
EventGroupHandle_t contrlEvtGrp = NULL;

//declaration

TickType_t xTimeOnEntering;

// --------------------------------------------------------------------------------------
void taskMyCo2SensorTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		xEventGroupWaitBits(contrlEvtGrp, 0b00000001, pdTRUE, pdTRUE, portMAX_DELAY);
		vTaskDelay(400/portTICK_PERIOD_MS);
		getCo2SensorMeasurement();
		xEventGroupSetBits(contrlEvtGrp, 0b00010000);
	}
}

// --------------------------------------------------------------------------------------
void taskLightSensorTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		xEventGroupWaitBits(contrlEvtGrp, 0b00000010, pdTRUE, pdTRUE, portMAX_DELAY);
		vTaskDelay(700/portTICK_PERIOD_MS);
		getLightSensorMeasurement();
		xEventGroupSetBits(contrlEvtGrp, 0b00100000);
	}
}

// --------------------------------------------------------------------------------------
void taskMyHumiditySensorTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		xEventGroupWaitBits(contrlEvtGrp, 0b00000100, pdTRUE, pdTRUE, portMAX_DELAY);
		vTaskDelay(1000/portTICK_PERIOD_MS);
		getTemperatureHumiditySensorMeasurement();
		xEventGroupSetBits(contrlEvtGrp, 0b01000000);
	}
}

// --------------------------------------------------------------------------------------
void main(void)
{
	/*
	//////////////////////////////////////////////// START
	// testing and printing out test results, NOTE tests that FAIL are only printed out
	char* result = test_suite_c02();                                        // assigning the result of "ALL TESTSS PASSED (0 or 1) into "result"
	printf("Number of tests run: %d\n", tests_run);
	printf("!!! NOTE: Only the first test that failed is presented below !!!\n");
	printf("!!! NOTE: If you se no notice of failed tests below, it means they all have passed !!!\n\n");
	if (result) printf("FAIL: %s\n", result);                            // if result is a string, we will print the string which is the failure message

	//removeAllItems();                                                    // clearing the list after tests
	//////////////////////////////////////////////// END
	*/
	
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
	
	xTaskCreate(
	taskMyCo2SensorTask,			/* Function that implements the task. */
	"MyCo2SensorTask",				/* Text name for the task. */
	TASK_MY_Co2_TASK_STACK,      /* Stack size in words, not bytes. */
	(void*)2,						/* Parameter passed into the task. */
	CO2_TASK_PRIORITY,	/* Priority at which the task is created. */
	&_taskCo2Handle);			/* Used to pass out the created task's handle. */

	xTaskCreate(
	taskLightSensorTask,				/* Function that implements the task. */
	"MyLightSensorTask",					/* Text name for the task. */
	TASK_MY_LIGHT_TASK_STACK,		/* Stack size in words, not bytes. */
	(void*)3,						/* Parameter passed into the task. */
	LIGHT_TASK_PRIORITY,	/* Priority at which the task is created. */
	&_taskLightHandle);				/* Used to pass out the created task's handle. */

	xTaskCreate(
	taskMyHumiditySensorTask,				/* Function that implements the task. */
	"MyHumiditySensorTask",					/* Text name for the task. */
	TASK_MY_HUMIDITY_TASK_STACK,		/* Stack size in words, not bytes. */
	(void*)4,						/* Parameter passed into the task. */
	HUMIDITY_TASK_PRIORITY,	/* Priority at which the task is created. */
	&_taskHumidityHandle);				/* Used to pass out the created task's handle. */

	// Let the operating system take over :)
	vTaskStartScheduler();
}