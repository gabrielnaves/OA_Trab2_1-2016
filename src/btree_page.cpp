#include "btree_page.hpp"

BtreePage::BtreePage(int order) {
    this->order = order;
}

void BtreePage::show() {
    if (keys.size() != 0) {
        printf("%s", keys[0].c_str());
        for (int i = 1; i < keys.size(); ++i)
            printf(" %s", keys[i].c_str());
        printf("\n");
    }
}
