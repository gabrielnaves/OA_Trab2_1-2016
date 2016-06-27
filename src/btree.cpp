#include "btree.hpp"

Btree::Btree(int order) {
    this->order = order;
    root = makeNewPage();
}

Btree::~Btree() {}

void Btree::insert(KeyType key) {
    insertOnNode(root, nullptr, key);
}

void Btree::insertOnNode(Node node, Node parent, KeyType key) {
	if (not node->isLeaf()) {
		int i = 0;
		for (; i < node->keys.size(); ++i)
			if (key < node->keys[i])
				insertOnNode(node->next_pages[i], node, key);
		insertOnNode(node->next_pages[i], node, key);
	}
	else
		insertKeyOnLeaf(node, key);
	if (node->keys.size() >= order)
		insertionRebalanceNode(node, parent);
}

void Btree::insertionRebalanceNode(Node node, Node parent) {
	if (parent == nullptr) // Root node being rebalanced, create new root
		root = parent = makeNewPage();
	// Get the key to promote
	int index_of_key_to_promote = floor(order/2.0);
	KeyType key_to_promote = node->keys[index_of_key_to_promote];
	// Splits the node
	vector<KeyType> keys_to_sibling(node->keys.begin()+index_of_key_to_promote+1, node->keys.end());
	vector<Node> refs_to_sibling(node->next_pages.begin()+index_of_key_to_promote+1, node->next_pages.end());
	Node sibling = makeNewPage(keys_to_sibling, refs_to_sibling);
	node->keys.erase(node->keys.begin()+index_of_key_to_promote, node->keys.end());
	node->next_pages.erase(node->next_pages.begin()+index_of_key_to_promote, node->next_pages.end());
	// Arranje parent node
	int i = 0;
	for(; i < parent->keys.size(); ++i)
		if () // TODO: continue this part
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
