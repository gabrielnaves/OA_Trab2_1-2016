#include "test.hpp"

int Test::test_amount = 0;
int Test::failed_test_amount = 0;

void Test::runTests() {
    printf("----------------------------------------\n");
    testMiscOperations();
    testBtree();
    finalOutput();
}

void Test::testMiscOperations() {
    assertEqual(msc::makePrimaryKey("Carlos Dias Takase", "62364"), "Car62364");
    assertEqual(msc::makePrimaryKey("Yasmin Torres de Carvalho Brandao", "24262"), "Yas24262");
    assertEqual(msc::removeSpaces("Carlos Dias Takase"), "CarlosDiasTakase");
    assertEqual(msc::removeSpaces("Carlos    "), "Carlos");
    assertEqual(msc::removeSpaces("    C a r l o s    "), "Carlos");
}

void Test::testBtree() {
    Btree btree(4);
    btree.insert(5);
    btree.insert(7);
    btree.insert(3);
    btree.insert(8);
    // btree.insert(6);
    // btree.insert(11);
    // btree.insert(1);
    // btree.insert(2);
    // btree.insert(4);
    // btree.insert(9);
    // btree.insert(10);
    btree.show();
}

void Test::finalOutput() {
    if (failed_test_amount > 0)
        printf("----------------------------------------\n");
    printf("Ran %d test(s) with %d failure(s).\n", test_amount, failed_test_amount);
    if (failed_test_amount == 0)
        printf("--------\nAll OK!!\n");
}

void Test::assertEqual(string a, string b) {
    test_amount++;
    if (a != b) {
        failed_test_amount++;
        printf("Failed test [%d]: %s != %s\n", test_amount, a.substr(0,20).c_str()
                                             , b.substr(0,20).c_str());
    }
}
