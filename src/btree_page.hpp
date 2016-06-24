#ifndef BTREE_PAGE_HPP
#define BTREE_PAGE_HPP

#include "include_all_std.hpp"

struct BtreePage {
    BtreePage(int order);

    void show();

    int order;
    vector<string> keys;
    vector<int> reg_pos;
    vector<shared_ptr<BtreePage>> nextPages;
};

#endif /* BTREE_PAGE_HPP */
