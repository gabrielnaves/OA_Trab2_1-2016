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
    int offset = 0;
    while (getline(data_file, line)) {
        string primary_key = msc::makePrimaryKey(line.substr(0,40), line.substr(41, 5));
        insert(primary_key, offset);
        offset++;
    }
    packTree();
}

void Btree::unPackTree() {

}

void Btree::packTree() {

}

void Btree::insert(KeyType key, int offset) {
    insertOnNode(root, nullptr, key, offset);
}

void Btree::insertOnNode(Node node, Node parent, KeyType key, int offset) {
    if (not node->isLeaf()) {
        int i = 0;
        for (; i < node->keys.size(); ++i)
            if (key < node->keys[i])
                break;
        insertOnNode(node->next_pages[i], node, key, offset);
    }
    else
        insertKeyOnLeaf(node, key, offset);
    if (node->keys.size() >= order)
        insertionRebalanceNode(node, parent);
}

void Btree::insertionRebalanceNode(Node node, Node parent) {
    // Get the key and prr to promote
    int index_of_key_to_promote = floor(order/2.0);
    KeyType key_to_promote = node->keys[index_of_key_to_promote];
    int prr_to_promote = node->prr[index_of_key_to_promote];
    // Splits the node
    vector<KeyType> keys_to_sibling(node->keys.begin()+index_of_key_to_promote+1, node->keys.end());
    vector<int> prr_to_sibling(node->prr.begin()+index_of_key_to_promote+1, node->prr.end());
    vector<Node> refs_to_sibling(node->next_pages.begin()+index_of_key_to_promote+1, node->next_pages.end());
    Node sibling = makeNewPage(keys_to_sibling, prr_to_sibling, refs_to_sibling);
    node->keys.erase(node->keys.begin()+index_of_key_to_promote, node->keys.end());
    node->prr.erase(node->prr.begin()+index_of_key_to_promote, node->prr.end());
    node->next_pages.erase(node->next_pages.begin()+index_of_key_to_promote+1, node->next_pages.end());
    // Arrange parent node
    if (parent == nullptr) {
        root = parent = makeNewPage();
        parent->keys.push_back(key_to_promote);
        parent->prr.push_back(prr_to_promote);
        parent->next_pages.push_back(node);
        parent->next_pages.push_back(sibling);
    }
    else {
        int i = 0;
        for(; i < parent->keys.size(); ++i)
            if (key_to_promote < parent->keys[i])
                break;
        parent->keys.insert(parent->keys.begin()+i, key_to_promote);
        parent->prr.insert(parent->prr.begin()+i, prr_to_promote);
        parent->next_pages.insert(parent->next_pages.begin()+i+1, sibling);
    }
}

void Btree::insertKeyOnLeaf(Node leaf_node, KeyType key, int offset) {
    if (leaf_node->keys.size() == 0) {
        leaf_node->keys.push_back(key);
        leaf_node->prr.push_back(offset);
    }
    else {
        int i = 0;
        for (; i < leaf_node->keys.size(); ++i)
            if (leaf_node->keys[i] > key)
                break;
        leaf_node->keys.insert(leaf_node->keys.begin()+i, key);
        leaf_node->prr.insert(leaf_node->prr.begin()+i, offset);
    }
    leaf_node->next_pages.resize(leaf_node->keys.size());
}

Node Btree::makeNewPage() {
    return make_shared<BtreeNode>(order);
}

Node Btree::makeNewPage(vector<KeyType> keys, vector<int> prrs, vector<Node> refs) {
    return make_shared<BtreeNode>(order, keys, prrs, refs);
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
