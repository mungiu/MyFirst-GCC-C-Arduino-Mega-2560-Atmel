/*
 * LightData.c
 *
 * Created: 11/18/2019 6:54:30 PM
 *  Author: andre
 */ 
#pragma once
#include <stdio.h>
#include "Header Files/LightData.h"
#include <malloc.h>
#include <mh_z19.h> //for co2

#include "..//VIA_FreeRTOS_AVRMEGA-master/src/semphr.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/FreeRTOSConfig.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/ATMEGA_FreeRTOS.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/portmacro.h"
SemaphoreHandle_t lightSharedMutex;
void initialized_ligtht_mutext() {
    lightSharedMutex = xSemaphoreCreateMutex();
}

plight_data create_light_data(uint16_t light_data_value, bool corrupt_data) {
    plight_data light_data = (plight_data)malloc(sizeof(light_data_t));
    if (light_data == NULL) {
        return NULL;
    }
    else {
        light_data->light_data_value = light_data_value;
        light_data->corrupt_data = corrupt_data;
        return light_data;
    }
}
void set_is_corrupt_data(plight_data light_data, bool bool_corrupt_data){
    light_data->corrupt_data = bool_corrupt_data;
}
bool get_is_corrupt_data(plight_data bool_curropt_data) {
    return bool_curropt_data->corrupt_data;
}
void set_light_data(plight_data light_data, uint16_t light_data_value)
{
    light_data->light_data_value = light_data_value;
}
uint16_t get_light_data(plight_data light_data_value) {
    uint16_t ldata = 0;
    if (xSemaphoreTake(lightSharedMutex, portMAX_DELAY)) {
        ldata = light_data_value->light_data_value;
        xSemaphoreGive(lightSharedMutex);

    }
    else {
        //throw exception
    }
    return ldata;
}
void print_light_data(plight_data light_data_value) {
    printf("LIGHT SENSOR INPUT: %d", light_data_value->light_data_value);
}