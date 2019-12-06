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

//Functions and variables must be declared before use for this C compiler to see them
void tsl2591Callback(tsl2591ReturnCode_t rc/*, tsl2591_data lightMeasurement*/);

int main(void)
{
	//initialize driver
	stdioCreate(0);
	puts("Program Started!");
	
	//initialize light sensor driver
	tsl2591Create/*driver*/(tsl2591Callback/*Function for the driver to call/utillize*/);
	
	//enable light sensor
	if ( TSL2591_OK == tsl2591Enable() )
	{
       // The power up command is now send to the sensor - it can be powered down with a call to tsl2591Disable()
	}
	
	//---------
	
	//No interrupts allowed before this line. Use after finalizing initialization.
	sei();
	
	//---------
	
    while (1) 
    {
		
		_delay_ms(1000);
		
		/*Try sensor driver*/
			if ( TSL2591_OK != tsl2591FetchData() )
			{
				// Something went wrong
				// Investigate the return code further
			}
			else
			{
				//The light data will be ready after the driver calls the call back function with TSL2591_DATA_READY.
			}
		
		
    }
}

void tsl2591Callback(tsl2591ReturnCode_t rc/*, tsl2591_data lightMeasurement*/)
{
	uint16_t _fullRaw;
	uint16_t _visibleRaw;
	uint16_t _infraredRaw;
	float _lux;
	switch (rc)
	{
		case TSL2591_DATA_READY:
		if ( TSL2591_OK == (rc = tsl2591GetFullSpectrumRaw(&_fullRaw)) )
		{
			printf("\nFull Raw:%04X\n", _fullRaw);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("\nFull spectrum overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetVisibleRaw(&_visibleRaw)) )
		{
			printf("Visible Raw:%04X\n", _visibleRaw);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Visible overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetInfraredRaw(&_infraredRaw)) )
		{
			printf("Infrared Raw:%04X\n", _infraredRaw);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Infrared overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetLux(&_lux)) )
		{
			//something is wrong with the formatter when using floating print format ex. %5.4f. Even when using syntax from the Internet or Ib s example. Using hex format to verify sensor response.
			printf("Lux: %04X\n", _lux);
			//printf("Lux: %5.4f\n", _lux);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Lux overflow - change gain and integration time\n");
		}

		if(TSL2591_OK)
		{
			// If last command performed successful then
			// save to struct.

			//lightMeasurement = { _fullRaw, _visibleRaw, _infraredRaw, _lux };
		}
	}

}


