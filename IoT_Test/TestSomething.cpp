#include <CppUTest/TestHarness.h>
#include <CppUTest/TestHarness_c.h>

extern "C" {
	//include header files from production code here
};
TEST_GROUP(TestSomething) {
	void setup() {

	}
	void teardown() {

	}
};

TEST(TestSomething, myFristTEst) {
	FAIL("Test failled");

}