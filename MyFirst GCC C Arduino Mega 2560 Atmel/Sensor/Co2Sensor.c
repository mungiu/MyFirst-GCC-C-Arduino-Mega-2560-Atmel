#include <stdio.h>
#include <stdio_driver.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include <avr/io.h>
#include <avr/sfr_defs.h>

//---

//co2 sensor
#include <mh_z19.h>

uint16_t ppmReturn = 0;
mh_z19_return_code_t rc;

void my_co2_call_back(uint16_t ppm)
{
	// Here you can use the CO2 ppm value
	//printf("\nCo2 ppm value: %08X\n", ppm);
	//save co2 ppm value
	ppmReturn=ppm;
}

int main(void)
{
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
		//_delay_ms(500);
		rc = mh_z19_take_meassuring();
		if (rc != MHZ19_OK)
		{
			// Something went wrong
			puts("Something went wrong take_meassuring()");
		}
		
		/*Not in documentation put does not work either*/
		_delay_ms(500);

			printf("\nCo2 ppm value returned: %08d\n", ppmReturn);
		
	}
	
}




