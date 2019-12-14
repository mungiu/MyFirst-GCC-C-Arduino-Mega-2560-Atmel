#include <CppUTest/TestHarness.h>
#include <CppUTest/TestHarness_c.h>

extern "C" {
	//include header files from production code here
#include <co2_data.h>
};
TEST_GROUP(co2_Test) {
	void setup() {

	}
	void teardown() {

	}
};

TEST(co2_Test, get_co2_data_test) {
	FAIL("Test failled");

}