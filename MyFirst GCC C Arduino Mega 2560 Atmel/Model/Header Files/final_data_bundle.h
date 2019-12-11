#pragma once
#include "co2_data.h"
#include "light_data.h"
#include "temp_hum_data.h"
typedef struct final_data_bundle_t {
	plight_data light_data_obj;
	pco2_data co2_data_obj;
	ptemp_hum_data temp_hum_data_obj;
}final_data_bundle_t;
typedef struct final_data_bundle_t* pfinal_data_bundle;

pfinal_data_bundle create_final_data_bundle(plight_data light_data_obj, pco2_data co2_data_obj, ptemp_hum_data temp_hum_data_obj);
pco2_data get_co2_data_obj(pfinal_data_bundle co2_ob);
plight_data get_light_data_obj(pfinal_data_bundle light_data_obj);
ptemp_hum_data get_temp_hum_data_obj(pfinal_data_bundle temp_hum_data_obj);
void destroy_final_data(pfinal_data_bundle final_data_bundle);
void initialize_final_data_mutext();
