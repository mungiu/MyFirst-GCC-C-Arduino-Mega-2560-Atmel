#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"

#include "co2Test.h"
#include "../Model/Header Files/co2_data.h"

int tests_run = 0; //counts number of test performed
pco2_data co2_Data = NULL; //

void startTest() {
	free(co2_Data); //to make sure it's clear everytime

	co2_Data = (pco2_data)malloc(sizeof(co2_data_t)); //allocate memory to co2 object
}

char* test_one_plus_one(void) {
	MU_ASSERT("test 1+1", 2 == 1 + 1);
	return 0;
}

char* test_get_co2_data() {
	co2_Data = create_co2_data(32, false);
	MU_ASSERT("get_co2_data:", 32 == get_co2_data(co2_Data));
	return 0;
}



char* test_suite_c02(void) {
	MU_RUN_TEST(test_get_co2_data);

	return 0;

}