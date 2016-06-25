#ifndef BTREE_HPP
#define BTREE_HPP

#include "include_all_std.hpp"

#include "btree_node.hpp"

class Btree {
  public:
    Btree(int order);

    void show();
    void insert(string key);

  private:

    void showBtree(shared_ptr<BtreeNode> page);
    shared_ptr<BtreeNode> makeNewPage(string key);

    int order;
    shared_ptr<BtreeNode> root;
};

#endif /* BTREE_HPP */
