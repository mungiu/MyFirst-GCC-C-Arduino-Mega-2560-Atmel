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
#include "..//Model/Header Files/temp_hum_data.h"
#include "..//Model/shared_variables.h"
ptemp_hum_data data_temp_hum;

//Humidity Temperature sensor
#include <hih8120.h>

float humidity;
float temperature;

void getTemperatureHumiditySensorMeasurement2() {
	puts("Hi from temperatureHumidity");}

void getTemperatureHumiditySensorMeasurement() {
	puts("Hi from temperatureHumidity");
		
		//In this example these two variables will be used to store the results in
		humidity = 0.0;
		temperature = 0.0;
		
		if ( HIH8120_OK != hih8120Wakeup() )
		{
			puts("Something went wrong hih8120Wakeup()");
			// Something went wrong
			// Investigate the return code further
		}
		else
		{
			//puts("hih8120Wakeup() positive");
		}
		
		//After the hih8120Wakeup() call the sensor will need minimum 50 ms to be ready!
		_delay_ms(50);
		
		if ( HIH8120_OK !=  hih8120Meassure() )
		{
			puts("Something went wrong hih8120Meassure()");
			// Something went wrong
			// Investigate the return code further
		}
		else
		{
			//puts("hih8120Meassure() positive");
		}
		
		//After the hih8120Meassure() call the two wire inteface (TWI) will need minimum 1 ms to fetch the results from the sensor! 
		_delay_ms(5);
		
		//Get the results: The result can now be retrieved by calling one of these methods hih8120GetHumidityPercent_x10(), hih8120GetTemperature_x10(), hih8120GetHumidity() or hih8120GetTemperature().
		humidity = hih8120GetHumidity();
		temperature = hih8120GetTemperature();
		
		//print results
		//something is wrong with the formatter when using floating print format ex. %5.4f. Even when using syntax from the Internet or Ib s example. Using hex format to verify sensor response.
		printf("Humidity: %08d\nTemperature: %08d\n", humidity, temperature);
		
		//Save to model struct
		data_temp_hum = create_temp_hum_data(temperature, humidity , false);

};


