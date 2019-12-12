#include <stdio.h>
#include "..//Header Files/final_data_bundle.h"
#include <malloc.h>
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/semphr.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/FreeRTOSConfig.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/ATMEGA_FreeRTOS.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/portmacro.h"

#include "..//VIA_FreeRTOS_AVRMEGA-master/src/semphr.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/FreeRTOSConfig.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/ATMEGA_FreeRTOS.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/portmacro.h"
SemaphoreHandle_t final_data_mutex;
void initialize_final_data_mutext() {
	final_data_mutex = xSemaphoreCreateMutex();
}


pfinal_data_bundle create_final_data_bundle(plight_data light_data_obj, pco2_data co2_data_obj, ptemp_hum_data temp_hum_data_obj) {
	pfinal_data_bundle final_data = (pfinal_data_bundle)malloc(sizeof(final_data_bundle_t));
	if (final_data == NULL) {
		return NULL;
	}
	else {
		void initialize_final_data_mutext() 

		final_data->co2_data_obj = co2_data_obj;
		final_data->light_data_obj = light_data_obj;
		final_data->temp_hum_data_obj = temp_hum_data_obj;

	}
}
pco2_data get_co2_data_obj(pfinal_data_bundle co2_obj) {
	pfinal_data_bundle temp_co2_obj;
	if (xSemaphoreTake(final_data_mutex, portMAX_DELAY)) {
	
		temp_co2_obj = co2_obj->co2_data_obj;
	xSemaphoreGive(final_data_mutex);
	}
	else {
		//do somehting
	}
	return temp_co2_obj;
}
plight_data get_light_data_obj(pfinal_data_bundle light_data_obj) {
	pfinal_data_bundle temp_light_obj;
	if (xSemaphoreTake(final_data_mutex, portMAX_DELAY)) {

		temp_light_obj = light_data_obj->light_data_obj;
		xSemaphoreGive(final_data_mutex);
	}
	else {
		//do somehting
	}
	return temp_light_obj;
}
ptemp_hum_data get_temp_hum_data_obj(pfinal_data_bundle temp_hum_data_obj) {
	pfinal_data_bundle temp_temp_hum_obj;
	if (xSemaphoreTake(final_data_mutex, portMAX_DELAY)) {

		temp_temp_hum_obj = temp_hum_data_obj->temp_hum_data_obj;
		xSemaphoreGive(final_data_mutex);
	}
	else {
		//do somehting
	}
	return temp_temp_hum_obj;
}
void destroy_final_data(pfinal_data_bundle final_data_bundle) {
	free(final_data_bundle);
}
