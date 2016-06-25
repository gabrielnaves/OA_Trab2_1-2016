#ifndef BTREE_NODE_HPP
#define BTREE_NODE_HPP

#include "include_all_std.hpp"

typedef string KeyType;

struct BtreeNode : public enable_shared_from_this<BtreeNode> {
  public:
    BtreeNode(int order);
    BtreeNode(int order, vector<KeyType> keys, vector<shared_ptr<BtreeNode>> next_pages);

    void emptyTree();
    void show();
    bool isLeaf();

    shared_ptr<BtreeNode> findInsertionLocation(KeyType new_key);
    shared_ptr<BtreeNode> insert(KeyType new_key);
    shared_ptr<BtreeNode> insertionRebalancing();
    void insertionArrangeParentNode(KeyType promoted_key, shared_ptr<BtreeNode> sibling);

    shared_ptr<BtreeNode> splitNode();

    int order;
    vector<KeyType> keys;
    vector<shared_ptr<BtreeNode>> next_pages;
    shared_ptr<BtreeNode> parent_node;
};

#endif /* BTREE_NODE_HPP */
