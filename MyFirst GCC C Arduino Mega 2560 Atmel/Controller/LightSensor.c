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
void tsl2591Callback(tsl2591ReturnCode_t rc);

/*Doxygen: Reads a measurement from the light sensor tsl2591. The driver must be initialized before the method call.*/
void getLightSensorMeasurement() {
	puts("Hi from light");
	
	//enable light sensor - This powers up the light sensor.
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

/*Doxygen: Callback function to to called from the driver interface. The measurement value is saved to shared abstract data type to be accessed from other parts of the code.
\variable: data_light. A pointer to the shared data instance
\Variable x4, _fullRaw, _visibleRaw, _infraredRaw, _lux: Variables to store the return values from the driver interface.
The values are later saved into the shared data instance data_light*/
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