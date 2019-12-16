#pragma once
#include <stdbool.h>
#include <stdint.h>
//pointer to light_data_t  ADT
typedef struct light_data_t* plight_data;
//constructor for light data
plight_data create_light_data(uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux, bool corrupt_data);
//setters and getters
void set_is_corrupt_data(plight_data light_data, bool bool_corrupt_data);
bool get_is_corrupt_data(plight_data light_data);
void set_light_data(plight_data light_data, uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux);
uint16_t get_fullRaw(plight_data fullRaw);
uint16_t get_visibleRaw(plight_data visibleRaw);
uint16_t get_infraredRaw(plight_data infraredRaw);
//destroy method
void destory_light_data(plight_data light_data);
void print_light_data(plight_data light_data);