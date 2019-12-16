/*
 * final_data_test_main.c
 *
 * Created: 15/12/2019 15:14:03
 *  Author: maria
 */ 
 //* Created: 14/12/2019 22:58:10
 //*  Author: maria
 //*/ 
#include <stdio.h>
//Minunit Test header
#include "minunit.h"
//Includes for testing final data model


#include "../Model/Header Files/light_data.h"
#include "../Model/Header Files/temp_hum_data.h"
#include "../Model/Header Files/co2_data.h"

#include "../Model/Header Files/final_data_bundle.h"


//co2 model struct
 struct finalData_t {
		plight_data light_data_obj;
		pco2_data co2_data_obj;
		ptemp_hum_data temp_hum_data_obj;
};
//declaration of final data pointer
pfinal_data_bundle finalData = NULL; 
	pco2_data co2=NULL;
	plight_data light=NULL;
	ptemp_hum_data temp_hum=NULL;


//counts number of test performed
tests_run = 0;
int foo = 7;



//Random tests methods made to test test setup
static char * test_foo() {
	MU_ASSERT("error, foo != 7", foo == 7);
	return 0;
}

//testing co2 model get method
char* test_get_light_obj() {
co2=create_co2_data(12,true);
 light=create_light_data(3,5,2,5,false);
 temp_hum=create_temp_hum_data(30,20,false);
	finalData = create_final_data_bundle(light, co2, temp_hum);
	MU_ASSERT("light obj:", light == get_light_data_obj(light));
	return 0;
}
//char* test_get_co2_obj() {
	//pco2_data co2=create_co2_data(12,true);
	//plight_data light=NULL;
	//ptemp_hum_data temp_hum=NULL;
	//finalData = create_final_data_bundle(light, co2, temp_hum);
	//MU_ASSERT("co2 obj :", co2 == get_co2_data_obj(co2));
	//return 0;
//}
//char* test_get_temp_hum_obj() {
	//pco2_data co2=NULL;
	//plight_data light=NULL;
	//ptemp_hum_data temp_hum=create_temp_hum_data(30,20,false);
	//finalData = create_final_data_bundle(light, co2, temp_hum);
	//MU_ASSERT("temp humdity obj:", temp_hum == get_temp_hum_data_obj(temp_hum));
	//return 0;
//}
////char* test_get_light_obj() {
	////pco2_data co2=create_co2_data(12,true);
	////plight_data light=create_light_data(3,5,2,5,false);
	////ptemp_hum_data temp_hum=create_temp_hum_data(30,20,false);
	////finalData = create_final_data_bundle(light, co2, temp_hum);
	////MU_ASSERT("final data:", light == get_light_data_obj(light));
	////return 0;
////}
//char* test_null_co2_obj(){
	//
//vPortFree(co2_Data);	
	//co2_Data = NULL;
	//MU_ASSERT("co2 null:", NULL == co2_Data);
	//return 0;
//}
//char* test_destroy_co2_obj(){
	//////	co2_Data = (pco2_data)malloc(sizeof( struct co2_data_t)); //allocate memory to co2 object
//
	////co2_Data = create_co2_data(12, false);
	////destroy_co2_data(co2_Data);
	////MU_ASSERT("destroy co2:", NULL == co2_Data);
	//return 0;
//}

static char * all_tests() {
		MU_RUN_TEST(test_foo);
		MU_RUN_TEST(test_get_light_obj);
		//MU_RUN_TEST(test_get_co2_obj);
		//MU_RUN_TEST(test_get_temp_hum_obj);
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
