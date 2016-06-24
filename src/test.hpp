#ifndef TEST_HPP
#define TEST_HPP

#include "include_all_std.hpp"

#include "msc_operations.hpp"
#include "btree.hpp"

class Test {
  public:
    static void runTests();

  private:
    static void testMiscOperations();
    static void testBtree();

    static void finalOutput();

    static void assertEqual(string a, string b);

    static int test_amount;
    static int failed_test_amount;
};

#endif /* TEST_HPP */
