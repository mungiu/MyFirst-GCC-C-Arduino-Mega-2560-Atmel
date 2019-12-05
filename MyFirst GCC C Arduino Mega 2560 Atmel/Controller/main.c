#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "SensorTask.h"
#include "LorawanTask.h"
#include "semphr.h"

/* Priorities at which the tasks are created. */
#define TASK_MY_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define	TASK_MY_SECOND_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define	TASK_MY_THIRD_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define	TASK_MY_FORTH_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define	TASK_MY_FIFTH_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )

/* Task stack sizes*/
#define TASK_MY_TASK_STACK				( configMINIMAL_STACK_SIZE )
#define	TASK_MY_SECOND_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_THIRD_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_FORTH_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_FIFTH_TASK_STACK		( configMINIMAL_STACK_SIZE )

/* Task Handles */
TaskHandle_t _taskFirstHandle = NULL;
TaskHandle_t _taskSecondHandle = NULL;
TaskHandle_t _taskThirdHandle = NULL;
TaskHandle_t _taskForthHandle = NULL;
TaskHandle_t _taskFifthHandle = NULL;

/*Semaphores*/
SemaphoreHandle_t co2xSemaphore;
SemaphoreHandle_t lightxSemaphore;
SemaphoreHandle_t temperatureHumidityxSemaphore;
SemaphoreHandle_t xSemaphore;

//need seperate result semaphore for sensors not to take semaphones back because controller can only take and wait for one semaphore at a time
//In exemple. If one sensortask is finished much bore another, thne it would have to wait long enoght for the other task to finish or the other task semaphore to timeout for the controllertask to request and collect the semaphore of the finished waiting before beeing taken bak again by the finished waiting task. better for maintainability to use seperate tasks.
//insures thst the semaphore was taken by distingiushing between direction of communication. Doesn't mistake a semaphore not beeing taken as that reply is alreay present
//No not  to wait for the reciver to take the semaphore or for the reciver to be higher priority to immidiate take it.
SemaphoreHandle_t co2Result;
SemaphoreHandle_t lightResult;
SemaphoreHandle_t temperatureHumidityResult;
SemaphoreHandle_t xSemaphoreResult;

// --------------------------------------------------------------------------------------
void taskMyTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		//controller thread.

		puts("before create semaphores");

			/* Attempt to create a semaphore. */
		//Sensor
		co2xSemaphore = xSemaphoreCreateBinary();
		lightxSemaphore = xSemaphoreCreateBinary();
		temperatureHumidityxSemaphore = xSemaphoreCreateBinary();
		//Lorawan
		xSemaphore = xSemaphoreCreateBinary();

		//Sensor
		co2Result = xSemaphoreCreateBinary();
		lightResult = xSemaphoreCreateBinary();
		temperatureHumidityResult = xSemaphoreCreateBinary();
		//Lorawan
		xSemaphoreResult = xSemaphoreCreateBinary();

		if (xSemaphore == NULL || temperatureHumidityxSemaphore == NULL || lightxSemaphore == NULL || co2xSemaphore == NULL
			|| co2Result == NULL || lightResult == NULL || temperatureHumidityResult == NULL || xSemaphoreResult == NULL)
		{
			puts(" There was insufficient FreeRTOS heap available for the semaphore to be created successfully.");
		}
		else
		{
			for (;;)
			{

				//For some reason all delays are *2 when i run the program in Visual Studio

			//continues loop

			//For timing next measurement initialization.
			TickType_t xTimeOnEntering = xTaskGetTickCount();

			//give semaphores
			xSemaphoreGive(co2xSemaphore);
			xSemaphoreGive(lightxSemaphore);
			xSemaphoreGive(temperatureHumidityxSemaphore);

			puts("Controller gave sensor signal semaphores");

			//waits a small amount to allow other threads to acquired the semaphore
			
			TickType_t maxDelaySensor = pdMS_TO_TICKS(3000);
			//wait to take semaphores back when available
			xSemaphoreTake(co2Result, maxDelaySensor);
			xSemaphoreTake(lightResult, maxDelaySensor);
			xSemaphoreTake(temperatureHumidityResult, maxDelaySensor);

			puts("Controller took sensor result semaphores");

			//give semaphores
			xSemaphoreGive(xSemaphore);
			puts("Controller gave Lorawan signal semaphore");

			TickType_t maxDelayLorawan = pdMS_TO_TICKS(1500);
			xSemaphoreTake(xSemaphoreResult, maxDelayLorawan);

			puts("Controller took Lorawan result semaphore");
			puts("Controller took all the result semaphores");

			puts("Controller sleeps");

			//Delay until next measurement
			TickType_t timeInBetweenMeasuments = pdMS_TO_TICKS(5000);
			vTaskDelayUntil(&xTimeOnEntering, timeInBetweenMeasuments);

			}
		}

	}
}

// --------------------------------------------------------------------------------------
void taskMySeccondTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		SemaphoreHandle_t localSemaphore = co2xSemaphore;
		xSemaphoreTake(localSemaphore, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(1600));
		getCo2SensorMeasurement();
		xSemaphoreGive(co2Result);
	}
}

// --------------------------------------------------------------------------------------
void taskMyThirdTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		SemaphoreHandle_t localSemaphore = lightxSemaphore;
		xSemaphoreTake(localSemaphore, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(1200));
		getLightSensorMeasurement();
		xSemaphoreGive(lightResult);
	}
}

// --------------------------------------------------------------------------------------
void taskMyForthTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		SemaphoreHandle_t localSemaphore = temperatureHumidityxSemaphore;
		xSemaphoreTake(localSemaphore, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(800));
		getTemperatureHumiditySensorMeasurement();
		xSemaphoreGive(temperatureHumidityResult);
	}
}

void taskMyFifthTask(void* pvParameters)
{
	// Remove compiler warnings.
	(void)pvParameters;

	for (;;)
	{
		SemaphoreHandle_t localSemaphore = xSemaphore;
		xSemaphoreTake(localSemaphore, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(1000));
		sendDataPackageToLorawan();
		xSemaphoreGive(xSemaphoreResult);
	}
}

// --------------------------------------------------------------------------------------
void main(void)
{
	/* Create the task, not storing the handle. */
	xTaskCreate(
		taskMyTask,       /* Function that implements the task. */
		"MyTask",          /* Text name for the task. */
		TASK_MY_TASK_STACK,      /* Stack size in words, not bytes. */
		(void*)1,    /* Parameter passed into the task. */
		TASK_MY_TASK_PRIORITY,/* Priority at which the task is created. */
		&_taskFirstHandle);      /* Used to pass out the created task's handle. */

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