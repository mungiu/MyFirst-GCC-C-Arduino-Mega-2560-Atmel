#include <stdio.h>
#include "..//Header Files/final_data_bundle.h"
#include <malloc.h>
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/semphr.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/FreeRTOSConfig.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/ATMEGA_FreeRTOS.h"
#include "..//VIA_FreeRTOS_AVRMEGA-master/src/portmacro.h"



p_final_data create_final_data_bundle(plight_data light_data_obj, pco2_data co2_data_obj, ptemp_hum_data temp_hum_data_obj);
pco2_data get_co2_data_obj(p_final_data co2_ob);
plight_data get_light_data_obj(p_final_data light_data_obj);
ptemp_hum_data get_temp_hum_data_obj(p_final_data temp_hum_data_obj);
void destroy_final_data(p_final_data final_data_bundle);
