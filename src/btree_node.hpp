#ifndef BTREE_NODE_HPP
#define BTREE_NODE_HPP

#include "include_all_std.hpp"

struct BtreeNode;

typedef string KeyType;
typedef shared_ptr<BtreeNode> Node;

struct BtreeNode {
  public:
    BtreeNode(int order);
    BtreeNode(int order, vector<KeyType> keys, vector<Node> next_pages);

    void show();
    bool isLeaf();

    int order;
    vector<KeyType> keys;
    vector<Node> next_pages;
};

#endif /* BTREE_NODE_HPP */
