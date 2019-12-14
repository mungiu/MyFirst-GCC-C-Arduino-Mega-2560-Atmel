#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct temp_hum_data_t* ptemp_hum_data;

ptemp_hum_data create_temp_hum_data(uint16_t temp_data_value, uint16_t hum_data_value, bool is_corrupt_data);
void set_is_corrupt_data_th(ptemp_hum_data temp_hum_data, bool is_corrupt_data);
bool get_is_corrupt_data_th(ptemp_hum_data bool_currupt_data);

void set_hum_data(ptemp_hum_data temp_hum_data, uint16_t hum_data_value);
uint16_t get_hum_data(ptemp_hum_data temp_hum_data);

void set_temp_data(ptemp_hum_data temp_hum_data, uint16_t temp_data_value);
uint16_t get_temp_data(ptemp_hum_data temp_hum_data);
void print_temp_hum_data(ptemp_hum_data temp_hum_data);

void initialize_temp_hum_mutext();
void destroy_temp_hum_data(ptemp_hum_data temp_hum_data);