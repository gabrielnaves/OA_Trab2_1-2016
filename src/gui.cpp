#include "gui.hpp"

bool GUI::is_ok = true;

bool GUI::ok() {
    return is_ok;
}

void GUI::run() {
    is_ok = false; // Leaves infinite loop, as nothing is being done here yet
    cout << "Nothing to do here." << endl;
}
