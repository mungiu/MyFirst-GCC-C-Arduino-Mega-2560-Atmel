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

//Humidity Temperature sensor
#include <hih8120.h>

float humidity;
float temperature;

int main(void)
{
	
		//initialize driver
		stdioCreate(0);
		puts("Program Started!");
		
			//---------
	
	//No interrupts allowed before this line. Use after finalizing initialization.
	sei();
		
		//initialize driver
		if ( HIH8120_OK == hih8120Create() )
		{
			puts("Driver created OK");
			// Driver created OK
			// Always check what hih8120Create() returns
		}
		
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
			puts("hih8120Wakeup() positive");
		}
		
		//After the hih8120Wakeup() call the sensor will need minimum 50 ms to be ready!
		_delay_ms(500);
		
		for (;;)
		{
			
			//------
			//Wakeup must be in measurement loop. Otherwise old measurement value is repeated. Se data in comment last in file.
					if ( HIH8120_OK != hih8120Wakeup() )
					{
						puts("Something went wrong hih8120Wakeup()");
						// Something went wrong
						// Investigate the return code further
					}
					else
					{
						puts("hih8120Wakeup() positive");
					}
					
					//After the hih8120Wakeup() call the sensor will need minimum 50 ms to be ready!
					_delay_ms(500);
			//------
		
		if ( HIH8120_OK !=  hih8120Meassure() )
		{
			puts("Something went wrong hih8120Meassure()");
			// Something went wrong
			// Investigate the return code further
		}
		else
		{
			puts("hih8120Meassure() positive");
		}
		
		//After the hih8120Meassure() call the two wire inteface (TWI) will need minimum 1 ms to fetch the results from the sensor! 
		_delay_ms(500);
		
		//Get the results: The result can now be retrieved by calling one of these methods hih8120GetHumidityPercent_x10(), hih8120GetTemperature_x10(), hih8120GetHumidity() or hih8120GetTemperature().
		humidity = hih8120GetHumidity();
		temperature = hih8120GetTemperature();
		
		//print results
		//something is wrong with the formatter when using floating print format ex. %5.4f. Even when using syntax from the Internet or Ib s example. Using hex format to verify sensor response.
		printf("\nHumidity: %08d\nTemperature: %08d\n", humidity, temperature);
		
		}
		
}

/*
Wakeup must be in measurement loop. At least with these delays (10000ms). Otherwise old measuremtn is fetched by i.e. hih8120GetHumidity(). After measuremnt maybe sensor goes to sleep.
-------
Without wakeup in loop
-------

8120Meassure() positive

Humidity: CD16
Temperature: 425A
hih8120Meassure() positive

Humidity: CD16
Temperature: 425A
hih8120Meassure() positive

Humidity: CD16
Temperature: 425A
hih8120Meassure() positive

Humidity: CD16
Temperature: 425A
hih8120Meassure() positive

Humidity: CD16
Temperature: 425A
hih8120Meassure() positive

Humidity: CD16
Temperature: 425A
hih8120Meassure() positive

Humidity: CD16
Temperature: 425A
hih8120Meassure() positive

----------
With wakeup in loop
----------

Program Started!
Driver created OK
hih8120Wakeup() positive
hih8120Wakeup() positive
hih8120Meassure() positive

Humidity: 727C
Temperature: 424F
hih8120Wakeup() positive
hih8120Meassure() positive

Humidity: EA17
Temperature: 4252
hih8120Wakeup() positive
hih8120Meassure() positive

Humidity: 407A
Temperature: 424F
hih8120Wakeup() positive
hih8120Meassure() positive

Humidity: 1549
Temperature: 4251
hih8120Wakeup() positive
hih8120Meassure() positive

Humidity: 5A0B
Temperature: 4251
hih8120Wakeup() positive
hih8120Meassure() positive

Humidity: EF7F
Temperature: 424F
hih8120Wakeup() positive
hih8120Meassure() positive
*/


