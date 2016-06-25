#include "btree_node.hpp"

BtreeNode::BtreeNode(int order) {
    this->order = order;
    parent_node = nullptr;
}

BtreeNode::BtreeNode(int order, vector<string> keys, vector<shared_ptr<BtreeNode>> next_pages) {
    this->order = order;
    this->keys = keys;
    this->next_pages = next_pages;
}

void BtreeNode::show() {
    if (keys.size() != 0) {
        printf("%s", keys[0].c_str());
        for (int i = 1; i < keys.size(); ++i)
            printf(" %s", keys[i].c_str());
        printf("\n");
    }
}

bool BtreeNode::isLeaf() {
    for (auto ptr : next_pages)
        if (ptr != nullptr)
            return false;
    return true;
}

shared_ptr<BtreeNode> BtreeNode::findInsertionLocation(string new_key) {
    if (not isLeaf()) {
        int i;
        for (i = 0; i < keys.size(); ++i)
            if (keys[i] > new_key)
                return next_pages[i]->findInsertionLocation(new_key);
        return next_pages[i]->findInsertionLocation(new_key);
    }
    return shared_from_this();
}

/**
 * Inserts a new key on a leaf node. Won't work properly if called on a non-leaf node.
 */
shared_ptr<BtreeNode> BtreeNode::insert(string new_key) {
    int i;
    for (i = 0; i < keys.size(); ++i)
        if (keys[i] > new_key)
            break;
    keys.insert(keys.begin()+i, new_key);
    next_pages.resize(keys.size()+1);

    return insertionRebalancing();
}

shared_ptr<BtreeNode> BtreeNode::insertionRebalancing() {
    if (keys.size() == order) {
        // Get the key to be promoted
        int index_to_remove = floor(order/2.0);
        string promoted_key = keys[index_to_remove];
        shared_ptr<BtreeNode> sibling = splitNode(); // Split current node
        insertionArrangeParentNode(promoted_key, sibling);
        return parent_node->insertionRebalancing();
    }
    return parent_node != nullptr ? parent_node->insertionRebalancing() : shared_from_this();
}

void BtreeNode::insertionArrangeParentNode(string promoted_key, shared_ptr<BtreeNode> sibling) {
    if (parent_node == nullptr) {
        vector<shared_ptr<BtreeNode>> parent_refs(2);
        parent_refs[0] = shared_from_this(); parent_refs[1] = sibling;
        parent_node = make_shared<BtreeNode>(order, vector<string>(1, promoted_key), parent_refs);
    }
}

/**
 * Splits the current node, and returns the new sibling node created by the splitting
 */
shared_ptr<BtreeNode> BtreeNode::splitNode() {
    int begin_index = floor(order/2.0) + 1;

    // Retrieve keys
    vector<string> keys_to_pass;
    for (int i = begin_index; i < keys.size(); ++i)
        keys_to_pass.push_back(keys[i]);
    keys.erase(keys.begin()+begin_index-1, keys.end());

    // Retrieve descendants
    vector<shared_ptr<BtreeNode>> descendants_to_pass;
    for (int i = begin_index; i < next_pages.size(); ++i)
        descendants_to_pass.push_back(next_pages[i]);
    next_pages.erase(next_pages.begin()+begin_index, next_pages.end());

    return make_shared<BtreeNode>(order, keys_to_pass, descendants_to_pass);
}
