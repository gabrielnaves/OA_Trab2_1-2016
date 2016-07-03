#include "test.hpp"

int Test::test_amount = 0;
int Test::failed_test_amount = 0;

void Test::runTests() {
    printf("----------------------------------------\n");
    testMiscOperations();
    testBtree();
    testSearch();
    finalOutput();
}

void Test::testMiscOperations() {
    assertEqual(msc::makePrimaryKey("Carlos Dias Takase", "62364"), "Car62364");
    assertEqual(msc::makePrimaryKey("Yasmin Torres de Carvalho Brandao", "24262"), "Yas24262");
    assertEqual(msc::removeSpaces("Carlos Dias Takase"), "CarlosDiasTakase");
    assertEqual(msc::removeSpaces("Carlos    "), "Carlos");
    assertEqual(msc::removeSpaces("    C a r l o s    "), "Carlos");
    assertEqual(msc::removeTrailingSpaces("a   "), "a");
    assertEqual(msc::removeTrailingSpaces("a  b "), "a  b");
    assertEqual(msc::removeTrailingSpaces("     "), "");
    assertEqual(msc::removeTrailingSpaces(""), "");
}

void Test::testBtree() {
    remove("testes/btree.txt");
    remove("testes/btree2.txt");
    remove("testes/btree3.txt");
    remove("testes/lista2.txt");
    msc::copyFile("testes/lista.txt","testes/lista2.txt");
    Btree a(4, "testes/lista.txt", "testes/btree.txt");
    Btree b(5, "testes/lista.txt", "testes/btree2.txt");
    Btree c(4, "testes/lista2.txt", "testes/btree3.txt");

    Reg reg = make_shared<Registry>();
    reg->name = "Gabriel Naves da Silva";
    reg->matr = "12001";
    reg->curso = "MC";
    reg->turma = "Z";
    c.insert(reg);
}

void Test::testSearch() {
    Registry reg = Search::search("testes/lista.txt", "testes/btree.txt", "Mig12997", 4);
    assertTrue(reg.good);
    assertEqual(reg.name, "Miguel Bianchini");
    assertEqual(reg.matr, "12997");
    assertEqual(reg.curso, "CC");
    assertEqual(reg.turma, "A");
    assertEqual(reg.nseeks, 3);

    reg = Search::search("testes/lista.txt", "testes/btree.txt", "Wil44654", 4);
    assertTrue(reg.good);
    assertEqual(reg.name, "William Fernandes Crepaldi");
    assertEqual(reg.matr, "44654");
    assertEqual(reg.curso, "CC");
    assertEqual(reg.turma, "A");
    assertEqual(reg.nseeks, 2);

    reg = Search::search("testes/lista.txt", "testes/btree2.txt", "And71929", 5);
    assertTrue(reg.good);
    assertEqual(reg.name, "Andre Alvim Batista");
    assertEqual(reg.matr, "71929");
    assertEqual(reg.curso, "EM");
    assertEqual(reg.turma, "B");
    assertEqual(reg.nseeks, 3);

    reg = Search::search("testes/lista2.txt", "testes/btree3.txt", "Gab12001", 4);
    assertTrue(reg.good);
    assertEqual(reg.name, "Gabriel Naves da Silva");
    assertEqual(reg.matr, "12001");
    assertEqual(reg.curso, "MC");
    assertEqual(reg.turma, "Z");
    assertEqual(reg.nseeks, 2);
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

void Test::assertEqual(int a, int b) {
    test_amount++;
    if (a != b) {
        failed_test_amount++;
        printf("Failed test [%d]: %d != %d\n", test_amount, a, b);
    }
}

void Test::assertTrue(bool a) {
    test_amount++;
    if (not a) {
        failed_test_amount++;
        printf("Failed test [%d]! Condition sent is not true.\n", test_amount);
    }
}

