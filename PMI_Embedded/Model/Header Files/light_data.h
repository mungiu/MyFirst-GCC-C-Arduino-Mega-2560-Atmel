#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct light_data_t* plight_data;

plight_data create_light_data(uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux, bool corrupt_data);
void set_is_corrupt_data(plight_data light_data, bool bool_corrupt_data);
bool get_is_corrupt_data(plight_data light_data);
void set_light_data(plight_data light_data, uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux);
uint16_t get_fullRaw(plight_data fullRaw);
uint16_t get_visibleRaw(plight_data visibleRaw);
uint16_t get_infraredRaw(plight_data infraredRaw);

void print_light_data(plight_data light_data);
void destory_light_data(plight_data light_data);
