#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct co2_data_t* pco2_data;
//constructor
pco2_data create_co2_data(uint16_t co2_data_value, bool is_corrupt_data);
//setter for boolean variable
void set_is_corrupt_data_c(pco2_data co2_data, bool is_corrupt_data);
//getter for boolean variable
bool get_is_corrupt_data_c(pco2_data co2_data);
//setter for co2 uint16_t variable
void set_co2_data(pco2_data co2_data, uint16_t co2_data_value);
//getter for co2 value
uint16_t get_co2_data(pco2_data co2_data_value);
void print_co2_data(pco2_data co2_data);

void destroy_co2_data(pco2_data co2_data);