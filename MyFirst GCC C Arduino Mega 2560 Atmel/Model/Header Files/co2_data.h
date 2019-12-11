#pragma once
#include <stdbool.h>

#include <stdint.h>
typedef struct co2_data_t {
	uint16_t co2_data_value;
	bool is_corrupt_data;
}co2_data_t;

typedef struct co2_data_t* pco2_data;



pco2_data create_co2_data(uint16_t co2_data_value, bool is_corrupt_data);
void set_is_corrupt_data_c(pco2_data co2_data, bool is_corrupt_data);
bool get_is_corrupt_data_c(pco2_data is_curropt_data);
void set_co2_data(pco2_data co2_data, uint16_t co2_data_value);
uint16_t get_co2_data(pco2_data co2_data_value);
void print_co2_data(pco2_data co2_data);
void initialize_co2_mutext();
void destroy_co2_data(pco2_data co2_data);