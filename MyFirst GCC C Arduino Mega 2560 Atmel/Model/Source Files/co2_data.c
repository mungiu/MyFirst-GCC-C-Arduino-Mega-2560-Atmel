#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <portmacro.h>
#include "..//Header Files/co2_data.h"

SemaphoreHandle_t co2ShareMutex;

void initialize_co2_mutext() {
	co2ShareMutex= xSemaphoreCreateMutex();
}

pco2_data create_co2_data(uint16_t co2_data_value, bool corrupt_data) {
	void initialize_co2_mutext();
	pco2_data co2_data = (pco2_data)malloc(sizeof(co2_data_t));
	if (co2_data == NULL) {
		return NULL;
	}
	else {
		co2_data->co2_data_value = co2_data_value;
		co2_data->is_corrupt_data = corrupt_data;
		return co2_data;
	}
}

void set_is_corrupt_data_c(pco2_data co2_data, bool is_corrupt_data) {
	if (xSemaphoreTake(co2ShareMutex, portMAX_DELAY)) {
		xSemaphoreGive(co2ShareMutex);
		co2_data->is_corrupt_data = is_corrupt_data;
	}
	else {
		//throw exception
	}
}

bool get_is_corrupt_data_c(pco2_data co2_data) {
	bool is_currupt = false;
	if (xSemaphoreTake(co2ShareMutex, portMAX_DELAY)) {
		is_currupt = co2_data->is_corrupt_data;
		xSemaphoreGive(co2ShareMutex);
	}
	else {
		//throw exception
	}
	return is_currupt;
}

void set_co2_data(pco2_data co2_data, uint16_t co2_data_value) {
	if (xSemaphoreTake(co2ShareMutex, portMAX_DELAY)) {
		co2_data->co2_data_value = co2_data_value;
		xSemaphoreGive(co2ShareMutex);
	}
}

void destroy_co2_data(pco2_data co2_data) {
	free(co2_data);
}

uint16_t get_co2_data(pco2_data co2_data) {
	uint16_t c_data = 0;
	if (xSemaphoreTake(co2ShareMutex, portMAX_DELAY)) {
		c_data = co2_data->co2_data_value;
		xSemaphoreGive(co2ShareMutex);
	}
	return c_data;
}


void print_co2_data(pco2_data co2_data) {
	printf("CO2 SENSOR INPUT: %d", co2_data->co2_data_value);
}