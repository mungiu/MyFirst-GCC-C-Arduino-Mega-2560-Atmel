#include <stdio.h>
#include <stdio_driver.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/sfr_defs.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include "../FreeRTOSTraceDriver/FreeRTOSTraceDriver.h"

//light sensor
#include <tsl2591.h>

//---Model---//
#include "..//Model/shared_variables.h"
plight_data data_light;

TaskHandle_t _taskLightHandle = NULL;

/*Doxygen: Light sensor task method. Takes a measurements from the sensor. Waits for event bit flag to set, then performs a measurement. After measurement method has returned an event bit is set to mark that the method returned. Runs in a continues loop.
\Variable contrlEvtGrp: shared variable holds the event group to check the event bits*/
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

/*Task initialization*/
void createLightTask()
{
	xTaskCreate(
	taskLightSensorTask,				/* Function that implements the task. */
	"MyLightSensorTask",					/* Text name for the task. */
	TASK_MY_LIGHT_TASK_STACK,		/* Stack size in words, not bytes. */
	(void*)3,						/* Parameter passed into the task. */
	LIGHT_TASK_PRIORITY,	/* Priority at which the task is created. */
	&_taskLightHandle);				/* Used to pass out the created task's handle. */
}

//Functions and variables must be declared before use for this C compiler to see them
void tsl2591Callback(tsl2591ReturnCode_t rc);

void getLightSensorMeasurement() {
	puts("Hi from light");
	
	if ( TSL2591_OK == tsl2591Enable() )
	{
		// The power up command is now send to the sensor - it can be powered down with a call to tsl2591Disable()
	}
	
	//---------
	
	_delay_ms(250);
	
	/*Tries to take a measurement from the from the light sensor tsl2591. After the measuremnt is taken there is a small delay for the callback function to be called*/
	if ( TSL2591_OK != tsl2591FetchData() )
	{
		// Something went wrong
		// Investigate the return code further
	}
	else
	{
		//The light data will be ready after the driver calls the call back function with TSL2591_DATA_READY.
	}
	
};

void tsl2591Callback(tsl2591ReturnCode_t rc/*, tsl2591_data lightMeasurement*/)
{
	/*Variables to store the return values from the driver interface.
	 The values are saved into the shared data instance data_light after they have been retrieved*/
	uint16_t _fullRaw;
	uint16_t _visibleRaw;
	uint16_t _infraredRaw;
	float _lux;
	switch (rc)
	{
		case TSL2591_DATA_READY:
		if ( TSL2591_OK == (rc = tsl2591GetFullSpectrumRaw(&_fullRaw)) )
		{
			printf("Full Raw:%08d\n", _fullRaw);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Full spectrum overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetVisibleRaw(&_visibleRaw)) )
		{
			printf("Visible Raw:%08d\n", _visibleRaw);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Visible overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetInfraredRaw(&_infraredRaw)) )
		{
			printf("Infrared Raw:%08d\n", _infraredRaw);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Infrared overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetLux(&_lux)) )
		{
			printf("Lux: %08d\n", _lux);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Lux overflow - change gain and integration time\n");
		}

		if(TSL2591_OK == rc)
		{
			// If last command performed successful then
			// save to struct.
			
			set_light_data(data_light,_fullRaw,_visibleRaw,_infraredRaw,_lux);
			set_is_corrupt_data(data_light,false);
		}
	}
}