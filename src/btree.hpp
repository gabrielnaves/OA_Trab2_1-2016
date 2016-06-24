#ifndef BTREE_HPP
#define BTREE_HPP

#include "include_all_std.hpp"

#include "btree_page.hpp"

class Btree {
  public:
    Btree(int order);

    void showBtree();
    void insertKey(string key);

  private:

    void showBtree(shared_ptr<BtreePage> page);

    int order;
    shared_ptr<BtreePage> root;
};

#endif /* BTREE_HPP */
