#ifndef BTREE_NODE_HPP
#define BTREE_NODE_HPP

#include "include_all_std.hpp"

struct BtreeNode;

typedef string KeyType;
typedef shared_ptr<BtreeNode> Node;

struct BtreeNode {
  public:
    BtreeNode(int order);
    BtreeNode(int order, vector<KeyType> keys, vector<int> prr, vector<Node> next_pages);

    void show();
    bool isLeaf();

    int order;
    int pos_on_file;
    vector<KeyType> keys;
    vector<int> prr;
    vector<Node> next_pages;
};

#endif /* BTREE_NODE_HPP */
