#include "include_all_std.hpp"

#include "test.hpp"
#include "gui.hpp"

#define RUN_TESTS

int main() {
#ifdef RUN_TESTS
    cout << "Testing build" << endl;
    Test::runTests();
#else
    while (GUI::ok())
        GUI::run();
#endif
    return 0;
}
