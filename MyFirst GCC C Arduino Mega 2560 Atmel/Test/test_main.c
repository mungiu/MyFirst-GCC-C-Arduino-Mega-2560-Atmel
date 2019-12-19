 /*
 * co2_test_main.c
 *
 * Created: 14/12/2019 22:58:10
 *  Author: maria
 */ 
#include <stdio.h>
//--------------------MINUNIT---------------------------//
#include "minunit.h"
//--------------Model header files---------------------//
#include "../Model/Header Files/co2_data.h"
#include "../Model/Header Files/light_data.h"
#include "../Model/Header Files/temp_hum_data.h"



plight_data light_Data = NULL;
pco2_data co2_Data = NULL; 
ptemp_hum_data temp_hum=NULL;
//counts number of test performed
tests_run = 0;
//Random tests methods made to test test setup
static char * test_bar() {
	int bar = 5;
	MU_ASSERT("error, bar != 5", bar == 5);
	return 0;
}
//test get_co2_value
static char* test_get_co2_data_value() {
	int co2_value=150;
	co2_Data = create_co2_data(co2_value, false);
	MU_ASSERT("error, co2_value != get_co2_data(co2_Data)", co2_value == get_co2_data(co2_Data));
	return 0;
}

static char* test_set_co2_value(){
		int co2_value=150;
		co2_Data = create_co2_data(co2_value, false);
		co2_value=200;
		set_co2_data(co2_Data,co2_value);
		MU_ASSERT("Error: co2_Data.co2_data_value!= 200 ", co2_value == get_co2_data(co2_Data));

}
char* test_null_co2_obj(){
	co2_Data = NULL;
	MU_ASSERT("error: co2_Data != NULL", NULL == co2_Data);
	return 0;
}

char* test_create_light_and_get_infrared(){
	light_Data=create_light_data(12,23,12,23,false);
	MU_ASSERT("error:  infraredRaw!=23", 23 == get_infraredRaw(light_Data));
	return 0;
}
char* test_set_light_infrared(){
	light_Data=create_light_data(12,23,12,23,false);
	set_light_data(light_Data,10,10,10,19);
	MU_ASSERT("error: light_Data!=12 ",0!= get_infraredRaw(light_Data));
return 0;
	
}

char* test_create_tempHum_and_get_hum(){
	temp_hum=create_temp_hum_data(35,900,false);
	MU_ASSERT("error: humidity!=900", 900==get_hum_data(temp_hum));
	return 0;
}
char* test_create_tempHum_and_get_temp(){
	temp_hum=create_temp_hum_data(29,700,false);
	MU_ASSERT("error: temperature!=29", 29==get_temp_data(temp_hum));
	return 0;
}
static char * all_tests() {

		MU_RUN_TEST(test_bar);
		MU_RUN_TEST(test_get_co2_data_value);
		MU_RUN_TEST(test_null_co2_obj);
		MU_RUN_TEST(test_set_co2_value);
		MU_RUN_TEST(test_create_light_and_get_infrared);
		MU_RUN_TEST(test_set_light_infrared);
		MU_RUN_TEST(test_create_tempHum_and_get_temp);
		MU_RUN_TEST(test_create_tempHum_and_get_hum);
		
		return 0;
}



int main(int argc, char **argv) {
	
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	}
	else {
		printf("ALL TESTS PASSED\n" );
	}
	printf("Tests run: %d\n", tests_run);
	
	return result != 0;
}
