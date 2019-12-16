#include <stdio.h>
#include <stdio_driver.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/sfr_defs.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include "../FreeRTOSTraceDriver/FreeRTOSTraceDriver.h"

//---Model---//
#include "..//Model/Header Files/light_data.h"
#include "..//Model/shared_variables.h"
plight_data data_light;

//light sensor
#include <tsl2591.h>

//Functions and variables must be declared before use for this C compiler to see them
void tsl2591Callback(tsl2591ReturnCode_t rc/*, tsl2591_data lightMeasurement*/);

void getLightSensorMeasurement2() {
puts("Hi from light");}

void getLightSensorMeasurement() {
	puts("Hi from light");
	
	//enable light sensor
	if ( TSL2591_OK == tsl2591Enable() )
	{
		// The power up command is now send to the sensor - it can be powered down with a call to tsl2591Disable()
	}
	
	//---------
	
	_delay_ms(250);
	
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
	
};


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
			//something is wrong with the formatter when using floating print format ex. %5.4f. Even when using syntax from the Internet or Ib s example. Using hex format to verify sensor response.
			printf("Lux: %08d\n", _lux);
			//printf("Lux: %5.4f\n", _lux);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Lux overflow - change gain and integration time\n");
		}

		if(TSL2591_OK == rc)
		{
			// If last command performed successful then
			// save to struct.
			//data_light = create_light_data(_fullRaw, _visibleRaw, _infraredRaw, _lux, false);
			set_light_data(data_light,_fullRaw,_visibleRaw,_infraredRaw,_lux);
			set_is_corrupt_data(data_light,false);
		}
	}
}