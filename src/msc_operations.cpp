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

string msc::removeTrailingSpaces(string str) {
    while(str[str.size()-1] == ' ')
        str.pop_back();
    return str;
}

int msc::strToInt(string str) {
    int res;
    sscanf(str.c_str(), "%d", &res);
    return res;
}

void msc::copyFile(string src, string dst) {
    ifstream srcf(src);
    ofstream dstf(dst);
    dstf << srcf.rdbuf();
}

string msc::completeNameToFileStandard(string name) {
    while (name.size() > 40)
        name.pop_back();
    while (name.size() < 40)
        name.push_back(' ');
    return name;
}
