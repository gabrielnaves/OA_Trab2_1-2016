#include "btree_node.hpp"

BtreeNode::BtreeNode(int order) {
    this->order = order;
    parent_node = nullptr;
}

BtreeNode::BtreeNode(int order, vector<KeyType> keys, vector<shared_ptr<BtreeNode>> next_pages) {
    this->order = order;
    this->keys = keys;
    this->next_pages = next_pages;
}

/**
 * Resets all shared pointers on current node and, recursively, on all descendants.
 * Doesn't actually delete any pointers. If by the time this function is called someone else has a
 * shared_ptr to any node here, that node will not go away.
 */
void BtreeNode::emptyTree() {
    if (not isLeaf()) {
        for (int i = 0; i < next_pages.size(); ++i)
            next_pages[i]->emptyTree();
    }
    parent_node.reset();
    for (int i = 0; i < next_pages.size(); ++i)
        next_pages[i].reset();
}

void BtreeNode::show() {
    if (keys.size() != 0) {
        cout << keys[0];
        for (int i = 1; i < keys.size(); ++i)
            cout << " " << keys[i];
        cout << endl;
    }
}

bool BtreeNode::isLeaf() {
    for (auto ptr : next_pages)
        if (ptr != nullptr)
            return false;
    return true;
}

shared_ptr<BtreeNode> BtreeNode::findInsertionLocation(KeyType new_key) {
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
shared_ptr<BtreeNode> BtreeNode::insert(KeyType new_key) {
    int i;
    for (i = 0; i < keys.size(); ++i)
        if (keys[i] > new_key)
            break;
    keys.insert(keys.begin()+i, new_key);
    next_pages.resize(keys.size()+1);

    return insertionRebalancing();
}

shared_ptr<BtreeNode> BtreeNode::insertionRebalancing() {
    if (keys.size() >= order) {
        // Get the key to be promoted
        int index_to_remove = floor(order/2.0);
        KeyType promoted_key = keys[index_to_remove];
        shared_ptr<BtreeNode> sibling = splitNode(); // Split current node
        insertionArrangeParentNode(promoted_key, sibling); // Arrange parent node
        return parent_node->insertionRebalancing(); // Rebalance parent node
    }
    return parent_node == nullptr ? shared_from_this() : parent_node->insertionRebalancing();
}

void BtreeNode::insertionArrangeParentNode(KeyType promoted_key, shared_ptr<BtreeNode> sibling) {
    if (parent_node == nullptr) {
        vector<shared_ptr<BtreeNode>> parent_refs(2);
        parent_refs[0] = shared_from_this(); parent_refs[1] = sibling;
        parent_node = make_shared<BtreeNode>(order, vector<KeyType>(1, promoted_key), parent_refs);
        sibling->parent_node = parent_node;
    }
    else {
        int i = 0;
        for (; i < parent_node->keys.size(); ++i)
            if (parent_node->keys[i] > promoted_key)
                break;
        parent_node->keys.insert(parent_node->keys.begin() + i, promoted_key);
        parent_node->next_pages.insert(parent_node->next_pages.begin() + i + 1, sibling);
    }
}

/**
 * Splits the current node, and returns the new sibling node created by the splitting
 */
shared_ptr<BtreeNode> BtreeNode::splitNode() {
    int begin_index = floor(order/2.0) + 1;

    // Retrieve keys
    vector<KeyType> keys_to_pass;
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
