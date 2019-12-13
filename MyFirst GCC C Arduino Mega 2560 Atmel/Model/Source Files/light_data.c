#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>
#include <portmacro.h>
#include <semphr.h>

#include "..//Header Files/light_data.h"

SemaphoreHandle_t lightSharedMutex;

void initializ_ligtht_mutext()
{
	lightSharedMutex = xSemaphoreCreateMutex();
}

plight_data create_light_data(uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux, bool corrupt_data)
{
	//we initialize the shared light mutex here so we have the priority inheritance
	initializ_ligtht_mutext();
	plight_data light_data = (plight_data)malloc(sizeof(light_data_t));
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
		
		return light_data;
	}
}

void set_is_corrupt_data(plight_data light_data, bool bool_corrupt_data)
{
	if (xSemaphoreTake(lightSharedMutex, portMAX_DELAY))
	{
		xSemaphoreGive(lightSharedMutex);
		light_data->corrupt_data = bool_corrupt_data;
	}
	else
	{
		//throw exception
	}
}

bool get_is_corrupt_data(plight_data light_data)
{
	bool is_currupt = false;
	if (xSemaphoreTake(lightSharedMutex, portMAX_DELAY))
	{
		is_currupt = light_data->corrupt_data;
		xSemaphoreGive(lightSharedMutex);
	}
	else
	{
		//throw exception
	}
	return is_currupt;
}

/*void set_light_data(plight_data light_data, uint16_t light_data_value)
{
	if (xSemaphoreTake(lightSharedMutex, portMAX_DELAY))
	{
		light_data->light_data_value = light_data_value;
		xSemaphoreGive(lightSharedMutex);
	}
}

uint16_t get_light_data(plight_data light_data)
{
	uint16_t ldata = 0;
	if (xSemaphoreTake(lightSharedMutex, portMAX_DELAY))
	{
		ldata = light_data->light_data_value;
		xSemaphoreGive(lightSharedMutex);
	}
	return ldata;
}*/

void print_light_data(plight_data light_data)
{
	printf("LIGHT SENSOR INPUT _fullRaw: %d\n", light_data->_fullRaw);
	printf("LIGHT SENSOR INPUT _visibleRaw: %d\n", light_data->_visibleRaw);
	printf("LIGHT SENSOR INPUT _infraredRaw: %d\n", light_data->_infraredRaw);
	printf("LIGHT SENSOR INPUT _lux: %d\n", light_data->_lux);
}

void destory_light_data(plight_data light_data)
{
	free(light_data);
}