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
ptemp_hum_data data_temp_hum;

//Humidity Temperature sensor
#include <hih8120.h>

/*Declaration for task initialization*/
TaskHandle_t _taskHumidityHandle = NULL;

float humidity;
float temperature;

/*Doxygen: Temperature humidity sensor task method. Takes a measurements from the sensor. Waits for event bit flag to set, then performs a measurement. After measurement method has returned an event bit is set to mark that the method returned. Runs in a continues loop.
\Variable contrlEvtGrp: shared variable holds the event group to check the event bits*/
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

void createTempHumTask()
{
	xTaskCreate(
	taskMyHumiditySensorTask,				/* Function that implements the task. */
	"MyHumiditySensorTask",					/* Text name for the task. */
	TASK_MY_HUMIDITY_TASK_STACK,		/* Stack size in words, not bytes. */
	(void*)4,						/* Parameter passed into the task. */
	HUMIDITY_TASK_PRIORITY,	/* Priority at which the task is created. */
	&_taskHumidityHandle);				/* Used to pass out the created task's handle. */
}

/*Doxygen: Reads a measurement from the temperature and humidity sensor hih8120. The driver must be initialized before the method call.
\variable: data_temp_hum - pointer to the shared data instance
\Variable x2 humidity, temperature: Variables to store the return values from the driver interface. 
The values are later saved into the shared data instance data_temp_hum*/
void getTemperatureHumiditySensorMeasurement() {
	puts("Hi from temperatureHumidity");
		
		/*Variables to store the return values from the driver interface. 
		The values are later saved into the shared data instance data_temp_hum*/
		humidity = 0.0;
		temperature = 0.0;
		
		/*Powers up the sensor before measurement*/
		if ( HIH8120_OK != hih8120Wakeup() )
		{
			puts("Something went wrong hih8120Wakeup()");
			// Investigate the return code further
		}
		else
		{
			//Power up positive returned
		}
		
		//After the hih8120Wakeup() power up call the sensor will need minimum 50 ms to be ready!
		_delay_ms(50);
		
		if ( HIH8120_OK !=  hih8120Meassure() )
		{
			puts("Something went wrong hih8120Meassure()");
			// Investigate the return code further
		}
		else
		{
			//Measurement positive returned
		}
		
		//After the hih8120Meassure() call. Now the two wire inteface (TWI) will need minimum 1 ms to fetch the results from the sensor! 
		_delay_ms(5);
		
		//Get the results: The result can now be retrieved by calling one of these methods hih8120GetHumidityPercent_x10(), hih8120GetTemperature_x10(), hih8120GetHumidity() or hih8120GetTemperature().
		humidity = hih8120GetHumidity();
		temperature = hih8120GetTemperature();
		
		//print results
		printf("Humidity: %08d\nTemperature: %08d\n", humidity, temperature);
		
		//Save to model struct
		set_temp_data(data_temp_hum,temperature);
		set_hum_data(data_temp_hum,humidity);
		set_is_corrupt_data_th(data_temp_hum,false);

};


