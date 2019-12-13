/*
 * shared_variables.h
 *
 * Created: 13-12-2019 09:02:56
 *  Author: ASUS
 */ 


#ifndef SHARED_VARIABLES_H_
#define SHARED_VARIABLES_H_

#include "Header Files/temp_hum_data.h"
#include "Header Files/light_data.h"
#include "Header Files/co2_data.h"

//pointer to model data struct
extern ptemp_hum_data data_temp_hum;
extern plight_data data_light;
extern pco2_data data_co2;

#endif /* SHARED_VARIABLES_H_ */