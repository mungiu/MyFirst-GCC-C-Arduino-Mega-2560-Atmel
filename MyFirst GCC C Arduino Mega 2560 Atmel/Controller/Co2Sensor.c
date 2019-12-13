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
#include "..//Model/Header Files/co2_data.h"
#include "..//Model/shared_variables.h"
pco2_data data_co2;

//co2 sensor
#include <mh_z19.h>

uint16_t ppmReturn;
mh_z19_return_code_t rc;

void my_co2_call_back(uint16_t ppm);

void getCo2SensorMeasurement2() {
	puts("Hi from co2");}

void getCo2SensorMeasurement() {
	puts("Hi from co2");
	
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
			// must give to wait the the callback function was called and data saved
			_delay_ms(50);
			printf("Co2 ppm value returned: %08d\n", ppmReturn);
		}
	
};

void my_co2_call_back(uint16_t ppm)
{
	//save co2 ppm value
	ppmReturn=ppm;
	
	//save to shared
	data_co2 = create_co2_data(ppm, true);
}


