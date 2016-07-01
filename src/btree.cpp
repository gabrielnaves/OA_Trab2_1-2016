#include "btree.hpp"

Btree::Btree(int order, string data_fname, string tree_fname) {
    this->order = order;

    data_file.open(data_fname, std::fstream::in | std::fstream::out);
    if (not data_file) {
        cout << "Erro! Arquivo de dados não encontrado!" << endl;
        return;
    }

    index_file.open(tree_fname, std::fstream::in | std::fstream::out);
    if (not index_file) {
        // index_file.open(tree_fname, std::fstream::out);
        makeTreeFromDataFile();
    }
    else {
        unPackTree();
    }
}

Btree::~Btree() {
    packTree();
}

void Btree::makeTreeFromDataFile() {
    root = makeNewPage();
    data_file.seekg(0);
    string line;
    while (getline(data_file, line)) {
        string primary_key = msc::makePrimaryKey(line.substr(0,40), line.substr(41, 5));
        insert(primary_key);
    }
    packTree();
}

void Btree::unPackTree() {

}

void Btree::packTree() {

}

void Btree::insert(KeyType key) {
    insertOnNode(root, nullptr, key);
}

void Btree::insertOnNode(Node node, Node parent, KeyType key) {
    if (not node->isLeaf()) {
        int i = 0;
        for (; i < node->keys.size(); ++i)
            if (key < node->keys[i])
                break;
        insertOnNode(node->next_pages[i], node, key);
    }
    else
        insertKeyOnLeaf(node, key);
    if (node->keys.size() >= order)
        insertionRebalanceNode(node, parent);
}

void Btree::insertionRebalanceNode(Node node, Node parent) {
    // Get the key to promote
    int index_of_key_to_promote = floor(order/2.0);
    KeyType key_to_promote = node->keys[index_of_key_to_promote];
    // Splits the node
    vector<KeyType> keys_to_sibling(node->keys.begin()+index_of_key_to_promote+1, node->keys.end());
    vector<Node> refs_to_sibling(node->next_pages.begin()+index_of_key_to_promote+1, node->next_pages.end());
    Node sibling = makeNewPage(keys_to_sibling, refs_to_sibling);
    node->keys.erase(node->keys.begin()+index_of_key_to_promote, node->keys.end());
    node->next_pages.erase(node->next_pages.begin()+index_of_key_to_promote+1, node->next_pages.end());
    // Arranje parent node
    if (parent == nullptr) {
        root = parent = makeNewPage();
        parent->keys.push_back(key_to_promote);
        parent->next_pages.push_back(node);
        parent->next_pages.push_back(sibling);
    }
    else {
        int i = 0;
        for(; i < parent->keys.size(); ++i)
            if (key_to_promote < parent->keys[i])
                break;
        parent->keys.insert(parent->keys.begin()+i, key_to_promote);
        parent->next_pages.insert(parent->next_pages.begin()+i+1, sibling);
    }
}

void Btree::insertKeyOnLeaf(Node leaf_node, KeyType key) {
    leaf_node->keys.push_back(key);
    sort(leaf_node->keys.begin(), leaf_node->keys.end());
    leaf_node->next_pages.resize(leaf_node->keys.size());
}

Node Btree::makeNewPage() {
    return make_shared<BtreeNode>(order);
}

Node Btree::makeNewPage(vector<KeyType> keys, vector<Node> refs) {
    return make_shared<BtreeNode>(order, keys, refs);
}

void Btree::show() {
    showBtree(root);
}

void Btree::showBtree(Node page) {
    if (page != nullptr) {
        page->show();

        for (auto next_page : page->next_pages)
            showBtree(next_page);
    }
}
