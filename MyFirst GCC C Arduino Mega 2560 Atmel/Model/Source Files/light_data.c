#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>
#include <portmacro.h>
#include <semphr.h>

#include "..//Header Files/light_data.h"

//declaration of light data type to create variables
//three private fields
struct light_data_t {
	uint16_t _fullRaw;
	uint16_t _visibleRaw;
	uint16_t _infraredRaw;
	float _lux;
	bool corrupt_data;
	SemaphoreHandle_t lightSharedMutex;
}light_data_t;
//constructor to create light_data type
plight_data create_light_data(uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux, bool corrupt_data)
{//light_data  memory allocation
	//pvPortMalloc ensures it is thread safe
	plight_data light_data = (plight_data)pvPortMalloc(sizeof(struct light_data_t));
	if (light_data == NULL)
	{
		return NULL;
	}
	else
	{
		light_data->_fullRaw = _fullRaw;
		light_data->_visibleRaw = _visibleRaw;
		light_data->_infraredRaw = _infraredRaw;
		light_data->_lux = _lux;
		light_data->corrupt_data = corrupt_data;
		//we initialize the shared light mutex here so we have the priority inheritance
		light_data->lightSharedMutex = xSemaphoreCreateMutex();
		
		return light_data;
	}
}
// set if the light data is corrupt or not but only if it has the semaphore.
//after setting the boolean value, we give back the semaphore

void set_is_corrupt_data(plight_data light_data, bool bool_corrupt_data)
{
	if (xSemaphoreTake(light_data->lightSharedMutex, portMAX_DELAY))
	{
		light_data->corrupt_data = bool_corrupt_data;
		xSemaphoreGive(light_data->lightSharedMutex);
	}
	else
	{
		//throw exception
	}
}
//returns true if the light data is corrupt and false if it is not
bool get_is_corrupt_data(plight_data light_data)
{
	bool is_currupt = false;
	if (xSemaphoreTake(light_data->lightSharedMutex, portMAX_DELAY))
	{
		is_currupt = light_data->corrupt_data;
		xSemaphoreGive(light_data->lightSharedMutex);
	}
	else
	{
	//we timed out 
	}
	return is_currupt;
}
//sets all 4 values of light sensor
void set_light_data(plight_data light_data, uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux)
{
	if (xSemaphoreTake(light_data->lightSharedMutex, portMAX_DELAY))
	{
		light_data->_fullRaw = _fullRaw;
		light_data->_visibleRaw = _visibleRaw;
		light_data->_infraredRaw = _infraredRaw;
		light_data->_lux = _lux;
		
		xSemaphoreGive(light_data->lightSharedMutex);
	}
	else
	{
		//we timed out 
	}
}

//returns light fullRaw value
uint16_t get_fullRaw(plight_data fullRaw){
	uint16_t fr = 0;
	if (xSemaphoreTake(fullRaw->lightSharedMutex, portMAX_DELAY))
	{
		fr =fullRaw->_fullRaw;
		xSemaphoreGive(fullRaw->lightSharedMutex);
	}
	return fr;
}
//returns light visibleRaw value
uint16_t get_visibleRaw(plight_data visibleRaw){
	uint16_t vr = 0;
	if (xSemaphoreTake(visibleRaw->lightSharedMutex, portMAX_DELAY))
	{
		vr = visibleRaw->_visibleRaw;
		xSemaphoreGive(visibleRaw->lightSharedMutex);
	}
	return vr;
}
//returns light infrared value
uint16_t get_infraredRaw(plight_data infraredRaw){
	uint16_t ir = 0;
	if (xSemaphoreTake(infraredRaw->lightSharedMutex, portMAX_DELAY))
	{
		ir = infraredRaw->_infraredRaw;
		xSemaphoreGive(infraredRaw->lightSharedMutex);
	}
	return ir;
}

void print_light_data(plight_data light_data)
{
	if (xSemaphoreTake(light_data->lightSharedMutex, portMAX_DELAY)){
		printf("LIGHT SENSOR INPUT _fullRaw: %d\n", light_data->_fullRaw);
		printf("LIGHT SENSOR INPUT _visibleRaw: %d\n", light_data->_visibleRaw);
		printf("LIGHT SENSOR INPUT _infraredRaw: %d\n", light_data->_infraredRaw);
		printf("LIGHT SENSOR INPUT _lux: %f\n", light_data->_lux);
		xSemaphoreGive(light_data->lightSharedMutex);
	}
}
//Deallocate co2_data memory
void destory_light_data(plight_data light_data)
{
	if (xSemaphoreTake(light_data->lightSharedMutex, portMAX_DELAY)){
		vPortFree(light_data);
		light_data=NULL;
	}
}
