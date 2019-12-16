#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <portmacro.h>
#include "..//Header Files/co2_data.h"
//declaration of co2 data type to create variables
//three private fields
 struct co2_data_t {
	uint16_t co2_data_value; //co2 value
	bool is_corrupt_data; //true if co2 inputs is corropt
  SemaphoreHandle_t co2ShareMutex; //semaphore handler for co2 mutex
};

//constructor to create co2_data type
pco2_data create_co2_data(uint16_t co2_data_value, bool corrupt_data) {
	//co2_data  memory allocation
	//pvPortMalloc ensures it is thread safe
	pco2_data co2_data = (pco2_data)pvPortMalloc(sizeof(struct co2_data_t));
	if (co2_data == NULL) {
		return NULL;
	}
	else {
		co2_data->co2_data_value = co2_data_value;
		co2_data->is_corrupt_data = corrupt_data;
		co2_data->co2ShareMutex= xSemaphoreCreateMutex();
		return co2_data;
	}
}
// set if the co2 data is corrupt or not but only if it has the semaphore.
//after setting the boolean value, we give back the semaphore 

void set_is_corrupt_data_c(pco2_data co2_data, bool is_corrupt_data) {
	if (xSemaphoreTake(co2_data->co2ShareMutex, portMAX_DELAY)) {
		co2_data->is_corrupt_data = is_corrupt_data;
		//tell that we are done
		xSemaphoreGive(co2_data->co2ShareMutex);
	}
	else {
	//we timed out 
	}
}
//returns true if co2 the data is corrupt and false if it is not
bool get_is_corrupt_data_c(pco2_data co2_data) {
	//local boolean variable to set if data is corrupt
	bool is_currupt = false;
		//PortMAX_DELAY wait forever if necessary
	if (xSemaphoreTake(co2_data->co2ShareMutex, portMAX_DELAY)) {
		is_currupt = co2_data->is_corrupt_data;
		xSemaphoreGive(co2_data->co2ShareMutex);
	}
	else {
	//we timed out 
	}
	return is_currupt;
}
//sets the co2 value
void set_co2_data(pco2_data co2_data, uint16_t co2_data_value) {
	if (xSemaphoreTake(co2_data->co2ShareMutex, portMAX_DELAY)) {
		co2_data->co2_data_value = co2_data_value;
		xSemaphoreGive(co2_data->co2ShareMutex);
	}
}
//Deallocate co2_data memory
void destroy_co2_data(pco2_data co2_data) {
	if (xSemaphoreTake(co2_data->co2ShareMutex, portMAX_DELAY)) {
	vPortFree(co2_data);
co2_data=NULL;
;
	}
}

//returns an integer holding the co2 value
uint16_t get_co2_data(pco2_data co2_data) {
	uint16_t c_data = 0;//local variable that is then returned
	if (xSemaphoreTake(co2_data->co2ShareMutex, portMAX_DELAY)) {
		c_data = co2_data->co2_data_value;
		xSemaphoreGive(co2_data->co2ShareMutex);
	}
	return c_data;
}

void print_co2_data(pco2_data co2_data) {
	if (xSemaphoreTake(co2_data->co2ShareMutex, portMAX_DELAY)) {
		printf("CO2 SENSOR INPUT: %d\n", co2_data->co2_data_value);
		xSemaphoreGive(co2_data->co2ShareMutex);
	}
}
