#include <stdio.h>
#include <stdio_driver.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <tsl2591.h>

#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include "../FreeRTOSTraceDriver/FreeRTOSTraceDriver.h"

//---Model---//
#include "..//Model/shared_variables.h"
pco2_data data_co2;

//co2 sensor
#include <mh_z19.h>

/*Task handle*/
TaskHandle_t _taskCo2Handle = NULL;

uint16_t ppmReturn;
mh_z19_return_code_t rc;

/*Doxygen: CO2 sensor task method. Takes a measurements from the sensor. Waits for event bit flag to set, then performs a measurement. After measurement method has returned an event bit is set to mark that the method returned. Runs in a continues loop.
\Variable contrlEvtGrp: shared variable holds the event group to check the event bits*/
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

void createCo2Task()
{
		xTaskCreate(
		taskMyCo2SensorTask,			/* Function that implements the task. */
		"MyCo2SensorTask",				/* Text name for the task. */
		TASK_MY_Co2_TASK_STACK,      /* Stack size in words, not bytes. */
		(void*)2,						/* Parameter passed into the task. */
		CO2_TASK_PRIORITY,	/* Priority at which the task is created. */
		&_taskCo2Handle);			/* Used to pass out the created task's handle. */

}

void my_co2_call_back(uint16_t ppm);

void getCo2SensorMeasurement() {
	puts("Hi from co2");
	
	/*Initializes a variable to hold the value of the co2 measurement*/	
	ppmReturn=0;

		_delay_ms(50);
		rc = mh_z19_take_meassuring();
		if (rc != MHZ19_OK)
		{
			// Something went wrong
			puts("Something went wrong take_meassuring()");
		}
		else
		{
			// must wait for the callback function to be called called and save data
			_delay_ms(50);
			printf("Co2 ppm value returned: %08d\n", ppmReturn);
		}
	
};

void my_co2_call_back(uint16_t ppm)
{
	//save co2 ppm value
	ppmReturn=ppm;
	
	//save to shared abstract data type
	set_co2_data(data_co2,ppm);
	set_is_corrupt_data_c(data_co2,false);
	//data_co2 = create_co2_data(ppm, true);
}


