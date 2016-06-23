#include "msc_operations.hpp"

string msc::makePrimaryKey(string name, string reg_num) {
    name = removeSpaces(name).substr(0,3);
    reg_num = reg_num.substr(0,5);
    return name + reg_num;
}

string msc::removeSpaces(string str) {
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == ' ') {
            str.erase(str.begin() + i);
            i--;
        }
    }
    return str;
}
