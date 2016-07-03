#ifndef MSC_OPERATIONS_HPP
#define MSC_OPERATIONS_HPP

#include "include_all_std.hpp"

namespace msc {

string makePrimaryKey(string name, string reg_num);
string removeSpaces(string str);
string removeTrailingSpaces(string str);
int strToInt(string str);

void copyFile(string src, string dst);

string completeNameToFileStandard(string name);

}

#endif /* MSC_OPERATIONS_HPP */
