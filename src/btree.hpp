#ifndef BTREE_HPP
#define BTREE_HPP

#include "include_all_std.hpp"

#include "btree_node.hpp"
#include "msc_operations.hpp"

class Btree {
  public:
    Btree(int order, string data_fname, string tree_fname);
    ~Btree();

    void show();
    void insert(KeyType key, int offset = -1);

  private:

    void makeTreeFromDataFile();
    void packTree();
    void unPackTree();

    // Insertion methods
    void insertOnNode(Node node, Node parent, KeyType key, int offset = -1);
    void insertKeyOnLeaf(Node leaf_node, KeyType key, int offset = -1);
    void insertionRebalanceNode(Node node, Node parent);

    void showBtree(Node page);
    Node makeNewPage();
    Node makeNewPage(vector<KeyType> keys, vector<int> prrs, vector<Node> refs);

    void openDataFile();
    void closeDataFile();
    void openIndexFile();
    void closeIndexFile();

    int order;
    Node root;

    fstream data_file;
    fstream index_file;
    string data_fname;
    string tree_fname;
};

#endif /* BTREE_HPP */
