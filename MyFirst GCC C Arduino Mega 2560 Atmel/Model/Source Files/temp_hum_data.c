#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <portmacro.h>
#include "..//Header Files/temp_hum_data.h"

 struct temp_hum_data_t {
	uint16_t temp_data_value;
	uint16_t hum_data_value;
	bool is_corrupt_data;
};
SemaphoreHandle_t temp_hum_SharedMutex;

void initialize_temp_hum_mutext() {
	temp_hum_SharedMutex = xSemaphoreCreateMutex();

}


typedef struct temp_hum_data_t {
	uint16_t temp_data_value;
	uint16_t hum_data_value;
	bool is_corrupt_data;
  SemaphoreHandle_t temp_hum_SharedMutex;
};


ptemp_hum_data create_temp_hum_data(uint16_t temp_data_value, uint16_t hum_data_value ,bool is_corrupt_data) {
	ptemp_hum_data t_h_data = (ptemp_hum_data)pvPortMalloc(sizeof(struct temp_hum_data_t));
	if (t_h_data == NULL) {
		return NULL;
	}
	else {
		t_h_data->temp_data_value= temp_data_value;
		t_h_data->hum_data_value = hum_data_value;
		t_h_data->is_corrupt_data = is_corrupt_data;
		t_h_data->temp_hum_SharedMutex = xSemaphoreCreateMutex();
		return t_h_data;
		
	}
}

void set_is_corrupt_data_th(ptemp_hum_data temp_hum_data, bool is_corrupt_data) {
	if (xSemaphoreTake(temp_hum_data->temp_hum_SharedMutex, portMAX_DELAY)) {
		temp_hum_data->is_corrupt_data = is_corrupt_data;
		xSemaphoreGive(temp_hum_data->temp_hum_SharedMutex);
	}
	else {
		//throw exception
	}
}

bool get_is_corrupt_data_th(ptemp_hum_data bool_currupt_data) {
	bool is_currupt = false;
	if (xSemaphoreTake(bool_currupt_data->temp_hum_SharedMutex, portMAX_DELAY)) {
		is_currupt = bool_currupt_data->is_corrupt_data;
		xSemaphoreGive(bool_currupt_data->temp_hum_SharedMutex);
	}
	else {
		//throw exception
	}
	return is_currupt;
}

void set_hum_data(ptemp_hum_data temp_hum_data, uint16_t hum_data_value) {
	if (xSemaphoreTake(temp_hum_data->temp_hum_SharedMutex, portMAX_DELAY)) {
		temp_hum_data->hum_data_value = hum_data_value;
		xSemaphoreGive(temp_hum_data->temp_hum_SharedMutex);
	}
}

uint16_t get_hum_data(ptemp_hum_data temp_hum_data) {
	uint16_t h_data = 0;
	if (xSemaphoreTake(temp_hum_data->temp_hum_SharedMutex, portMAX_DELAY)) {
		h_data = temp_hum_data->hum_data_value;
		xSemaphoreGive(temp_hum_data->temp_hum_SharedMutex);
	}
	return h_data;
}

void set_temp_data(ptemp_hum_data temp_hum_data, uint16_t temp_data_value) {
	if (xSemaphoreTake(temp_hum_data->temp_hum_SharedMutex, portMAX_DELAY)) {
		temp_hum_data->temp_data_value = temp_data_value;
		xSemaphoreGive(temp_hum_data->temp_hum_SharedMutex);
	}
}

uint16_t get_temp_data(ptemp_hum_data temp_hum_data) {
	uint16_t t_data = 0;
	if (xSemaphoreTake(temp_hum_data->temp_hum_SharedMutex, portMAX_DELAY)) {
		t_data = temp_hum_data->temp_data_value;
		xSemaphoreGive(temp_hum_data->temp_hum_SharedMutex);
	}
	
	return t_data;
}

void print_temp_hum_data(ptemp_hum_data temp_hum_data) {
	if (xSemaphoreTake(temp_hum_data->temp_hum_SharedMutex, portMAX_DELAY)) {
		printf("TEMPERATURE SENSOR INPUT: %d \n HUMDITY SENSOR INPUT: %d\n", temp_hum_data->temp_data_value, temp_hum_data->hum_data_value);
		xSemaphoreGive(temp_hum_data->temp_hum_SharedMutex);
	}
}
void destroy_temp_hum_data(ptemp_hum_data temp_hum_data) {
	if (xSemaphoreTake(temp_hum_data->temp_hum_SharedMutex, portMAX_DELAY)) {
	vPortFree(temp_hum_data);
	}
}