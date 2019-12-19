/*
 * shared_variables.h
 *
 * Created: 13-12-2019 09:02:56
 *  Author: ASUS
 */ 


#ifndef SHARED_VARIABLES_H_
#define SHARED_VARIABLES_H_

/* Priorities at which the tasks are created. */
#define LED_TASK_PRIORITY				( tskIDLE_PRIORITY + 2 ) /*Must be highest priority*/
#define LORAWAN_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define CO2_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define	LIGHT_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define	HUMIDITY_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )

/* Task stack sizes*/
#define	TASK_MY_Co2_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_LIGHT_TASK_STACK		( configMINIMAL_STACK_SIZE )
#define	TASK_MY_HUMIDITY_TASK_STACK		( configMINIMAL_STACK_SIZE )

#include "Header Files/temp_hum_data.h"
#include "Header Files/light_data.h"
#include "Header Files/co2_data.h"

#include <event_groups.h>

//pointer to model data struct
extern ptemp_hum_data data_temp_hum;
extern plight_data data_light;
extern pco2_data data_co2;

extern EventGroupHandle_t contrlEvtGrp;

#endif /* SHARED_VARIABLES_H_ */