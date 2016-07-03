#include "btree.hpp"

Btree::Btree(int order, string data_fname, string tree_fname) {
    this->order = order;
    this->data_fname = data_fname;
    this->tree_fname = tree_fname;

    openDataFile(); ///
        if (not data_file) {
            cout << "Erro! Arquivo de dados não encontrado!" << endl;
            is_good = false;
            return;
        }
    closeDataFile(); ///
    is_good = true;

    openIndexFile(); ///
        bool index_file_exists = index_file;
    closeIndexFile(); ///

    if (index_file_exists)
        unPackTree();
    else
        makeTreeFromDataFile();
}

Btree::~Btree() {
    packTree();
}

void Btree::makeTreeFromDataFile() {
    vector<string> primary_keys;
    vector<int> offsets;
    openDataFile(); ///
        root = makeNewPage();
        string line;
        int offset = 0;
        while (getline(data_file, line)) {
            primary_keys.push_back(msc::makePrimaryKey(line.substr(0,40), line.substr(41, 5)));
            offsets.push_back(offset);
            offset++;
        }
    closeDataFile(); ///
    for (int i = 0; i < primary_keys.size(); ++i)
        insert(primary_keys[i], offsets[i]);

    index_file.open(tree_fname, std::fstream::out);
    closeIndexFile();
    packTree();
}

void Btree::unPackTree() {
    openIndexFile();
        string line;
        getline(index_file, line);
        int root_node = msc::strToInt(line.substr(6));
        vector<Node> tree_pages;
        while (getline(index_file, line)) {
            int node_num;
            int node_size;
            vector<KeyType> primary_keys;
            vector<int> offsets;
            vector<string> refs;

            // Node number
            node_num = msc::strToInt(line.substr(0,3));
            line.erase(line.begin(), line.begin()+6);

            // Node size
            node_size = msc::strToInt(line.substr(0,3));
            line.erase(line.begin(), line.begin()+6);

            // Primary keys on this node
            for (int i = 0; i < order-1; ++i) {
                if (i < node_size)
                    primary_keys.push_back(line.substr(1,8));
                line.erase(line.begin(), line.begin()+9);
            }
            line.erase(line.begin(), line.begin()+3);

            // Prrs on this node
            for (int i = 0; i < order-1; ++i) {
                if (i < node_size)
                    offsets.push_back(msc::strToInt(line.substr(1,3)));
                line.erase(line.begin(), line.begin()+4);
            }
            line.erase(line.begin(), line.begin()+3);

            // References on this node
            for (int i = 0; i < order; ++i) {
                refs.push_back(line.substr(1,3));
                line.erase(line.begin(), line.begin()+4);
            }

            tree_pages.push_back(makeNewPage());
            tree_pages[node_num]->pos_on_file = node_num;
            tree_pages[node_num]->keys = primary_keys;
            tree_pages[node_num]->prr = offsets;
            for (auto ref : refs)
                if (ref != "###")
                    tree_pages[node_num]->next_pages.push_back(tree_pages[msc::strToInt(ref)]);
        }
        root = tree_pages[tree_pages.size()-1];
    closeIndexFile();
}

void Btree::packTree() {
    openIndexFile();
        index_file << "root: " << setw(3) << "" << endl;
        tmp = 0;
        packStoreNode(root);
        index_file.seekg(0);
        index_file << "root: " << setw(3) << root->pos_on_file << endl;
    closeIndexFile();
}

void Btree::packStoreNode(Node node) {
    if (node) {
        for (auto next : node->next_pages)
            packStoreNode(next);

        node->pos_on_file = tmp;
        index_file << setw(3) << tmp++ << " | " << setw(3) << node->keys.size() << " | ";
        // Primary keys
        for (int i = 0; i < order - 1; ++i) {
            if (i < node->keys.size())
                index_file << " " << node->keys[i];
            else
                index_file << " " << setw(8) << setfill('#') << "" << setfill(' ');
        }
        index_file << " | ";
        // Prrs
        for (int i = 0; i < order - 1; ++i) {
            if (i < node->prr.size())
                index_file << " " << setw(3) << node->prr[i];
            else
                index_file << " " << setw(3) << setfill('#') << "" << setfill(' ');
        }
        index_file << " | ";
        // Pointers
        for (int i = 0; i < order; ++i) {
            if (i < node->next_pages.size()) {
                if (node->next_pages[i] != nullptr)
                    index_file << " " << setw(3) << node->next_pages[i]->pos_on_file;
                else
                    index_file << " " << setw(3) << setfill('#') << "" << setfill(' ');
            }
            else {
                index_file << " " << setw(3) << setfill('#') << "" << setfill(' ');
            }
        }
        index_file << endl;
    }
}

void Btree::insert(KeyType key, int offset) {
    insertOnNode(root, nullptr, key, offset);
    packTree();
}

void Btree::insert(Reg reg) {
    string key = msc::makePrimaryKey(reg->name, reg->matr);
    insertOnNode(root, nullptr, key, -1, reg);
    packTree();
}

void Btree::insertOnNode(Node node, Node parent, KeyType key, int offset, Reg reg) {
    if (not node->isLeaf()) {
        int i = 0;
        for (; i < node->keys.size(); ++i)
            if (key < node->keys[i])
                break;
        insertOnNode(node->next_pages[i], node, key, offset, reg);
    }
    else
        insertKeyOnLeaf(node, key, offset, reg);
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

void Btree::insertKeyOnLeaf(Node leaf_node, KeyType key, int offset, Reg reg) {
    for (auto k : leaf_node->keys)
        if (k == key) {
            if (reg != nullptr)
                reg->good = false;
            return;
        }
    if (reg != nullptr)
        offset = insertOnDataFile(reg);
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
    leaf_node->next_pages.resize(leaf_node->keys.size()+1);
}

int Btree::insertOnDataFile(Reg reg) {
    int offset = 0;
    // Calculate offset
    openDataFile();
    {
        string line;
        while (getline(data_file, line))
            ++offset;
    }
    closeDataFile();
    // Insert at end of data file
    openDataFile();
    {
        data_file.seekp(0, ios_base::end);
        data_file << msc::completeNameToFileStandard(reg->name) << " " << reg->matr
                  << "  " << reg->curso << "  " << reg->turma << endl;
    }
    closeDataFile();
    return offset;
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

void Btree::openDataFile() {
    data_file.open(data_fname, std::fstream::in | std::fstream::out);
}

void Btree::closeDataFile() {
    data_file.close();
}

void Btree::openIndexFile() {
    index_file.open(tree_fname, std::fstream::in | std::fstream::out);
}

void Btree::closeIndexFile() {
    index_file.close();
}

