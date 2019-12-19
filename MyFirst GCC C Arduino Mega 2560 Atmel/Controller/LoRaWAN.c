/*
* loraWAN.c
*
* Created: 12/2/2019 9:40:49 PM
*  Author: andre
* This class has been created with the help of an IoT device interface provided at this link: https://ihavn.github.io/IoT_Semester_project/lora_driver_quickstart.html
*/

#include "loraWAN.h"
#include <stddef.h>
#include <iled.h>
#include "..//Model/shared_variables.h"

// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "1990c988b325074e"
#define LORA_appKEY "056881b45efe457a59b22d75cb469b14"

TickType_t xTimeOnEntering;
static char _out_buf[100];
void lora_handler_task( void* pvParameters );
static lora_payload_t _uplink_payload;

// Creates a Task which will be responsible for launching the LoRaWAN module.
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

// LoRaWan driver setup with active feedback.
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

// This method converts data from sensor structs into bytes and send them over the LoRaWAN network.
void lora_handler_task( void* pvParameters )
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

	_uplink_payload.len = 8;
	_uplink_payload.port_no = 2;

	for(;;)
	{
		//For timing next measurement initialization.
		xTimeOnEntering = xTaskGetTickCount();
		xEventGroupSetBits(contrlEvtGrp, 0b00000111);
		puts("Controller gave sensor signal semaphores");

		// THIS IS 5 SECONDS = "5000/portTICK_PERIOD_MS"
		TickType_t maxDelaySensor = 5000/portTICK_PERIOD_MS; // portTICK_PERIOD represent a ratio between tick and millisecond provided by this instance of FreeRTOS
		
		// wait to take semaphores back when available and save their values into uxBits value
		EventBits_t uxBits = xEventGroupWaitBits(contrlEvtGrp, 0b01110000, /*clear bits after wait*/ pdTRUE, /*wait for all bits AND logic not OR*/ pdTRUE, maxDelaySensor);
		puts("Controller took sensor result semaphores");
		
		puts("data saved in sensor structs by sensor tasks");
		print_co2_data(data_co2);
		print_light_data(data_light);
		print_temp_hum_data(data_temp_hum);

		vTaskDelay(pdMS_TO_TICKS(5000UL)); // TODO Investigate this task delay amount

		// TODO: replace dummy values with getMethods from Model classes
		_uplink_payload.bytes[0] = get_hum_data(data_temp_hum) >> 8;		// keeping the last 8 bits
		_uplink_payload.bytes[1] = get_hum_data(data_temp_hum) & 0xFF;		// keeping the first 8 bits
		_uplink_payload.bytes[2] = get_temp_data(data_temp_hum) >> 8;
		_uplink_payload.bytes[3] = get_temp_data(data_temp_hum) & 0xFF;
		_uplink_payload.bytes[4] = get_co2_data(data_co2) >> 8;
		_uplink_payload.bytes[5] = get_co2_data(data_co2) & 0xFF;
		_uplink_payload.bytes[6] = get_infraredRaw(data_light) & 0xFF;
		_uplink_payload.bytes[7] = uxBits;									// the byte which describes which sensors have hung

		led_short_puls(led_ST4);  // Led blink right before message upload
		printf("Upload Message >%s<\n", lora_driver_map_return_code_to_text(		lora_driver_sent_upload_message(false, &_uplink_payload)));

		puts("Controller sleeps - Delay until next measurement");
		
		TickType_t measurementsInterval = 150000/portTICK_PERIOD_MS;
		vTaskDelayUntil(&xTimeOnEntering, measurementsInterval);
	}
}
