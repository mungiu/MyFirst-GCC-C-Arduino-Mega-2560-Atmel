/*
* loraWAN.c
*
* Created: 12/2/2019 9:40:49 PM
*  Author: andre
*/
#include "loraWAN.h"

#include <stddef.h>
#include <iled.h>

//---Model---//
#include "..//Model/shared_variables.h"
#include "..//Model/Header Files/final_data_bundle.h"

#include "..//Model/Header Files/temp_hum_data.h"
#include "..//Model/Header Files/light_data.h"
#include "..//Model/Header Files/co2_data.h"

// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "1990c988b325074e"
#define LORA_appKEY "056881b45efe457a59b22d75cb469b14"

//declare
TickType_t xTimeOnEntering;

static char _out_buf[100];

void lora_handler_task( void* pvParameters );

static lora_payload_t _uplink_payload;

void lora_handler_create(UBaseType_t lora_handler_task_priority, EventGroupHandle_t xCreatedEventGroup)
{
	xTaskCreate(
	lora_handler_task
	,  (const portCHAR *)"LRHand"		// A name just for humans
	,  configMINIMAL_STACK_SIZE+200		// This stack size can be checked & adjusted by reading the Stack Highwater
	,  xCreatedEventGroup				// pass the event group as a parameter
	,  lora_handler_task_priority		// Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

static void _lora_setup(void)
{
	e_LoRa_return_code_t rc;
	led_slow_blink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_map_return_code_to_text(lora_driver_rn2483_factory_reset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_map_return_code_to_text(lora_driver_configure_to_eu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_get_rn2483_hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_map_return_code_to_text(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_map_return_code_to_text(lora_driver_set_device_identifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_map_return_code_to_text(lora_driver_set_otaa_identity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_map_return_code_to_text(lora_driver_save_mac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_map_return_code_to_text(lora_driver_set_adaptive_data_rate(LoRa_ON)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 5;
	do {
		rc = lora_driver_join(LoRa_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_map_return_code_to_text(rc));

		if ( rc != LoRa_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			led_long_puls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LoRa_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		led_led_on(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		led_led_off(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		led_fast_blink(led_ST1); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

/*-----------------------------------------------------------*/
void lora_handler_task( void* pvParameters )
{
	for(;;)
	{
		//fetch controller eventgroup from cast parameters
		EventGroupHandle_t contrlEvtGrp;
		contrlEvtGrp = (EventGroupHandle_t) pvParameters;
		static e_LoRa_return_code_t rc;
		
		// Hardware reset of LoRaWAN transceiver
		lora_driver_reset_rn2483(1);
		vTaskDelay(2);
		lora_driver_reset_rn2483(0);
		vTaskDelay(150);				// Give it a chance to wakeup
		lora_driver_flush_buffers();	// get rid of first version string from module after reset!
		_lora_setup();

		_uplink_payload.len = 6;
		_uplink_payload.port_no = 2;

		//For timing next measurement initialization.
		xTimeOnEntering = xTaskGetTickCount();

		//give semaphores
		xEventGroupSetBits(contrlEvtGrp, 0b00000111);

		puts("Controller gave sensor signal semaphores");
		
		// THIS IS 5 SECONDS = "5000/portTICK_PERIOD_MS"
		TickType_t maxDelaySensor = 5000/portTICK_PERIOD_MS; // portTICK_PERIOD represent a ratio between tick and milisecond provided by this instance of FreeRTOS
		// wait to take semaphores back when available
		EventBits_t uxBits = xEventGroupWaitBits(contrlEvtGrp, 0b01110000, /*clear bits after wait*/ pdTRUE, /*wait for all bits AND logic not OR*/ pdTRUE, maxDelaySensor);
		puts("Controller took sensor result semaphores");

		//-------------
		/*Send data to Lorawan part*/
		
		/*Interesting - We could use these flags from the sensors to denote if a new value was written to the modelstruct to be extracted or not if the sensor is hanging. Forward the flag from the controller instead of clearing it immidiately. then after the lorawantask has signaled finish to the controller then controller clears the flags just before gooing to sleep.*/
		// Masking out the bits that we are interested in and checking their values against expected values
		
		vTaskDelay(pdMS_TO_TICKS(5000UL)); // TODO Investigate this task delay amount
		
		// Some dummy payload
		uint16_t hum = 12345; // Dummy humidity
		int16_t temp = 675; // Dummy temp
		uint16_t co2_ppm = 1050; // Dummy CO2
		
		
		// TODO: replace dummy values with getMethods from Model classes
		_uplink_payload.bytes[0] = hum >> 8;		// keeping the last 8 bits
		_uplink_payload.bytes[1] = hum & 0xFF;		// keeping the first 8 bits
		_uplink_payload.bytes[2] = temp >> 8;
		_uplink_payload.bytes[3] = temp & 0xFF;
		_uplink_payload.bytes[4] = co2_ppm >> 8;
		_uplink_payload.bytes[5] = co2_ppm & 0xFF;
		_uplink_payload.bytes[6] = uxBits;			// the byte which describes which sensors have hung
		
		
		led_short_puls(led_ST4);  // OPTIONAL
		printf("Upload Message >%s<\n", lora_driver_map_return_code_to_text(		lora_driver_sent_upload_message(false, &_uplink_payload)));
		
		puts("Controller sleeps - Delay until next measurement");
		vTaskDelayUntil(&xTimeOnEntering, 5000/portTICK_PERIOD_MS);
	}
}
