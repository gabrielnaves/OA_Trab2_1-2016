#include "btree.hpp"

Btree::Btree(int order) {
    this->order = order;
    root = nullptr;
}

void insertKey(string key) {

}

void Btree::showBtree() {
    showBtree(root);
}

void Btree::showBtree(shared_ptr<BtreePage> page) {
    if (page != nullptr) {
        page->show();

        for (int i = 0; i < page->nextPages.size(); ++i)
            showBtree(page->nextPages[i]);
    }
}
