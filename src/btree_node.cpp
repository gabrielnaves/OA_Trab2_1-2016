#include "btree_node.hpp"

BtreeNode::BtreeNode(int order) {
    this->order = order;
}

BtreeNode::BtreeNode(int order, vector<KeyType> keys, vector<int> prr, vector<Node> next_pages) {
    this->order = order;
    this->keys = keys;
    this->prr = prr;
    this->next_pages = next_pages;
}

void BtreeNode::show() {
    if (keys.size() != 0) {
        cout << "(" << keys[0] << "," << prr[0] << ")";
        for (int i = 1; i < keys.size(); ++i)
            cout << " " << "(" << keys[i] << "," << prr[i] << ")";
        cout << endl;
    }
}

bool BtreeNode::isLeaf() {
    for (auto ptr : next_pages)
        if (ptr != nullptr)
            return false;
    return true;
}
