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

/*Doxygen: Reads a measurement from the co2 sensor mh_z19. The driver must be initialized before the method call.
\Variable ppmReturn: Hold the return value of the co2 measurement*/
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

/*Doxygen: Callback function to to called from the driver interface. The measurement value is saved to shared abstract data type to be accessed from other parts of the code.
\variable: data_co2. A pointer to the shared data instance*/
void my_co2_call_back(uint16_t ppm)
{
	//save co2 ppm value
	ppmReturn=ppm;
	
	//save to shared abstract data type
	set_co2_data(data_co2,ppm);
	set_is_corrupt_data_c(data_co2,false);
	//data_co2 = create_co2_data(ppm, true);
}


