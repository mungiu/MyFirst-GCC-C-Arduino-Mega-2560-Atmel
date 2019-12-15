 //* Created: 14/12/2019 22:58:10
 //*  Author: maria
 //*/ 
#include <stdio.h>
//Minunit Test header
#include "minunit.h"
//Includes for testing co2 model
#include "../Model/Header Files/co2_data.h"
//includes for testing co2 sensor
#include "Controller/Co2Sensor.h"
//co2 sensor
#include <mh_z19.h>
//co2 model struct
 struct co2_data_t {
	uint16_t co2_data_value;
	bool is_corrupt_data;
};
//declaration of co2 pointer
pco2_data co2_Data = NULL; 
//co2 sensor return value
uint16_t ppmReturn_;
mh_z19_return_code_t rc_;

//counts number of test performed
tests_run = 0;
//Random tests variables made to test test setup
int foo = 7;
int bar = 5;


//Random tests methods made to test test setup
static char * test_foo() {
	MU_ASSERT("error, foo != 7", foo == 7);
	return 0;
}

static char * test_bar() {
	MU_ASSERT("error, bar != 5", bar == 5);
	return 0;
}
//testing co2 model get method
char* test_get_co2_data_value() {
	co2_Data = create_co2_data(32, false);
	MU_ASSERT("get_co2_data_value:", 32 == get_co2_data(co2_Data));
	return 0;
}
char* test_null_co2_obj(){
	
vPortFree(co2_Data);	
	co2_Data = NULL;
	MU_ASSERT("co2 null:", NULL == co2_Data);
	return 0;
}
char* test_destroy_co2_obj(){
	////	co2_Data = (pco2_data)malloc(sizeof( struct co2_data_t)); //allocate memory to co2 object

	//co2_Data = create_co2_data(12, false);
	//destroy_co2_data(co2_Data);
	//MU_ASSERT("destroy co2:", NULL == co2_Data);
	return 0;
}

char* test_getCo2SensorMeasurement(){
	ppmReturn_=0;
	//MU_ASSERT("ppm return:")
	return 0;
}
char*  test_get_my_co2_call_back(){
	return 0;
}
static char * all_tests() {
	MU_RUN_TEST(test_foo);
		MU_RUN_TEST(test_bar);
		MU_RUN_TEST(test_get_co2_data_value);
		MU_RUN_TEST(test_null_co2_obj);
		MU_RUN_TEST(test_destroy_co2_obj);
		MU_RUN_TEST(test_get_my_co2_call_back);
		MU_RUN_TEST(test_getCo2SensorMeasurement);
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
