#include "test.hpp"

int Test::test_amount = 0;
int Test::failed_test_amount = 0;

void Test::runTests() {
    printf("----------------------------------------\n");
    assertEqual(msc::makePrimaryKey("Carlos Dias Takase", "62364"), "Car62364");
    finalOutput();
}

void Test::finalOutput() {
    if (failed_test_amount > 0)
        printf("----------------------------------------\n");
    printf("Ran %d test(s) with %d failure(s).\n", test_amount, failed_test_amount);
}

void Test::assertEqual(string a, string b) {
    test_amount++;
    if (a != b) {
        failed_test_amount++;
        printf("Failed test [%d]: %s != %s\n", test_amount, a.c_str(), b.c_str());
    }
}
