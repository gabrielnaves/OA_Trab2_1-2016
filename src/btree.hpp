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

    // Insertion methods
    void insertOnNode(Node node, Node parent, KeyType key);
	void insertKeyOnLeaf(Node leaf_node, KeyType key);
    void insertionRebalanceNode(Node node, Node parent);
	

    void showBtree(Node page);
    Node makeNewPage();
	Node makeNewPage(vector<KeyType> keys, vector<Node> refs);

    int order;
    Node root;
};

#endif /* BTREE_HPP */
