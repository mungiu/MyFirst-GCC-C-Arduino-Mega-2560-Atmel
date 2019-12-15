 //* Created: 14/12/2019 22:58:10
 //*  Author: maria
 //*/ 
#include <stdio.h>
#include "minunit.h"
#include "../Model/Header Files/co2_data.h"
 struct co2_data_t {
	uint16_t co2_data_value;
	bool is_corrupt_data;
};

 tests_run = 0; //counts number of test performed
pco2_data co2_Data = NULL; //

int foo = 7;
int bar = 5;

static char * test_foo() {
	MU_ASSERT("error, foo != 7", foo == 7);
	return 0;
}

static char * test_bar() {
	MU_ASSERT("error, bar != 5", bar == 5);
	return 0;
}
char* test_co2_data_value() {
	co2_Data = create_co2_data(32, false);
	MU_ASSERT("get_co2_data:", 32 == get_co2_data(co2_Data));
	return 0;
}
char* test_null_co2_obj(){
	
vPortFree(co2_Data);
	//	co2_Data = (pco2_data)malloc(sizeof( struct co2_data_t)); //allocate memory to co2 object
	
	
	co2_Data = NULL;
	MU_ASSERT("get_co2_data:", NULL == co2_Data);
	return 0;
}
static char * all_tests() {
	MU_RUN_TEST(test_foo);
		MU_RUN_TEST(test_bar);
		MU_RUN_TEST(test_co2_data_value);
		MU_RUN_TEST(test_null_co2_obj);
	return 0;
}



int main(int argc, char **argv) {
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	}
	else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);
	
	return result != 0;
}