#include <stdint.h>

void main(void) {
	
	//////////////////////////////////////////////// START
	// testing and printing out test results, NOTE tests that FAIL are only printed out
	char* result = test_suite_c02();                                        // assigning the result of "ALL TESTSS PASSED (0 or 1) into "result"
	printf("Number of tests run: %d\n", tests_run);
	printf("!!! NOTE: Only the first test that failed is presented below !!!\n");
	printf("!!! NOTE: If you se no notice of failed tests below, it means they all have passed !!!\n\n");
	if (result) printf("FAIL: %s\n", result);                            // if result is a string, we will print the string which is the failure message

	//removeAllItems();                                                    // clearing the list after tests
	//////////////////////////////////////////////// END
}