
#include <stdio.h>
#include "Header Files/co2_data.h"

pco2_data create_co2_data(uint16_t co2_data_value, bool corrupt_data) {
    pco2_data co2_data = (pco2_data)malloc(sizeof(pco2_data));
    if (co2_data == NULL) {
        return NULL;
    }
    else {
        co2_data->co2_data_value = co2_data_value;
        co2_data->corrupt_data = corrupt_data;
        return co2_data;
    }

}
void set_if_corrupt_data_c(pco2_data co2_data_value, bool bool_corrupt_data) {
    co2_data_value->corrupt_data = bool_corrupt_data;

}
bool get_if_corrupt_data_c(pco2_data bool_curropt_data) {
    return bool_curropt_data->corrupt_data;

}
void set_co2_data(pco2_data co2_data, uint16_t co2_data_value) {
    co2_data->co2_data_value = co2_data_value;

}
uint16_t get_co2_data(pco2_data co2_data_value) {
    return co2_data_value->co2_data_value;

}
void print_co2_data(pco2_data co2_data) {
    printf("CO2 SENSOR INPUT: %d", co2_data->co2_data_value);

}