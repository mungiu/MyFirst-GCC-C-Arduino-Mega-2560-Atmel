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

//---

//co2 sensor
#include <mh_z19.h>

uint16_t ppmReturn;
mh_z19_return_code_t rc;

void my_co2_call_back(uint16_t ppm);

int main(void)
{
	ppmReturn=22;
	//initialize driver
	stdioCreate(0);
	puts("Program Started!");
	
	//initialize driver
	mh_z19_create(ser_USART3, my_co2_call_back); 
	
	//---------
	
	//No interrupts allowed before this line. Use after finalizing initialization.
	sei();
	
	for(;;)
	{
		/*Callback function is never called as specified in documentation*/
		_delay_ms(500);
		rc = mh_z19_take_meassuring();
		if (rc != MHZ19_OK)
		{
			// Something went wrong
			puts("Something went wrong take_meassuring()");
		}
		else
		{
			printf("\nCo2 ppm value returned: %08X\n", ppmReturn);
		}
		
		/*Not in documentation put does not work either*/
		_delay_ms(500);
		rc = mh_z19_get_co2_ppm(ppmReturn);
		if (rc != MHZ19_OK)
		{
			// Something went wrong
			puts("Something went wrong mh_z19_get_co2_ppm()");
		}
		else
		{
			printf("\nCo2 ppm value returned: %08X\n", ppmReturn);
		}
	}
	
}

void my_co2_call_back(uint16_t ppm)
{
	// Here you can use the CO2 ppm value
	printf("\nCo2 ppm value: %08X\n", ppm);
	//save co2 ppm value
	ppmReturn=ppm;
}


