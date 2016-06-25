#ifndef BTREE_HPP
#define BTREE_HPP

#include "include_all_std.hpp"

#include "btree_node.hpp"

class Btree {
  public:
    Btree(int order);
    ~Btree();

    void show();
    void insert(KeyType key);

  private:

    void showBtree(shared_ptr<BtreeNode> page);
    shared_ptr<BtreeNode> makeNewPage(KeyType key);

    int order;
    shared_ptr<BtreeNode> root;
};

#endif /* BTREE_HPP */
