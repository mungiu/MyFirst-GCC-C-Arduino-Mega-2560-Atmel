#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct light_data_t {
	uint16_t light_data_value;
	bool corrupt_data;
}light_data_t;

typedef struct light_data_t* plight_data;

plight_data create_light_data(uint16_t light_data_value, bool corrupt_data);
void set_is_corrupt_data(plight_data light_data, bool bool_corrupt_data);
bool get_is_corrupt_data(plight_data light_data);
void set_light_data(plight_data light_data, uint16_t light_data_value);
uint16_t get_light_data(plight_data light_data);
void print_light_data(plight_data light_data);
void destory_light_data(plight_data light_data);
