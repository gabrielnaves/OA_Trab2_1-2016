#include "btree.hpp"

Btree::Btree(int order) {
    this->order = order;
    root = nullptr;
}

Btree::~Btree() {
    root->emptyTree();
}

void Btree::insert(KeyType key) {
    if (not root)
        root = makeNewPage(key);
    shared_ptr<BtreeNode> insertion_node = root->findInsertionLocation(key);
    root = insertion_node->insert(key);
}

shared_ptr<BtreeNode> Btree::makeNewPage(KeyType key) {
    return make_shared<BtreeNode>(order);
}

void Btree::show() {
    showBtree(root);
}

void Btree::showBtree(shared_ptr<BtreeNode> page) {
    if (page != nullptr) {
        page->show();

        for (auto next_page : page->next_pages)
            showBtree(next_page);
    }
}
