#include <iostream>
#include <fstream>
#include <utility>
using namespace std;


struct Node {
	int value;
	Node *left;
	Node *right;
};

class BTree {
public:
	BTree();
	~BTree();

	void insert(int key);
	void remove(int key);
	pair<Node*, Node*> search(int key);
	void destroy_tree();
	void inorder_print();
	void postorder_print();
	void preorder_print();
	void preorder_write(ostream& os);
	int find_min(Node* node);

private:
	Node* remove(Node* v, int key);
	void destroy_tree(Node *leaf);
	void insert(int key, Node *leaf);
	pair<Node*,Node*> search(int key, Node *leaf, Node *parent);
	void inorder_print(Node *leaf);
	void postorder_print(Node *leaf);
	void preorder_print(Node *leaf);
	void preorder_write(Node *leaf, ostream& os);
	Node *root;
};


BTree::BTree() {
	root = NULL;
}

BTree::~BTree() {
	destroy_tree();
}

void BTree::remove(int key) {
	if (root != NULL) {
		root = remove(root, key);
	}
}
void BTree::destroy_tree(Node *leaf) {
	if (leaf != NULL) {
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}

void BTree::insert(int key, Node *leaf) {

	if (key < leaf->value) {
		if (leaf->left != NULL) {
			insert(key, leaf->left);
		}
		else {
			leaf->left = new Node;
			leaf->left->value = key;
			leaf->left->left = NULL;
			leaf->left->right = NULL;
		}
	}
	else if (key > leaf->value) {
		if (leaf->right != NULL) {
			insert(key, leaf->right);
		}
		else {
			leaf->right = new Node;
			leaf->right->value = key;
			leaf->right->right = NULL;
			leaf->right->left = NULL;
		}
	}

}

void BTree::insert(int key) {
	if (root != NULL) {
		insert(key, root);
	}
	else {
		root = new Node;
		root->value = key;
		root->left = NULL;
		root->right = NULL;
	}
}
/*
void BTree::remove(int key) {
	if (root == NULL) {
		return;
	}
	pair<Node*, Node*> node_p = search(key);
	Node* node = node_p.first;
	
	if (node != NULL) {
		Node* right = node->right;
		Node* left = node->left;
		if (right != NULL) {
			if (right->left == NULL) {
				node->value = right->value;
				node->right = right->right;
				delete right;
			}
			else {
				Node* prev = right;
				while (right->left != NULL) {
					prev = right;
					right = right->left;
				}
				node->value = right->value;
				if (right->right != NULL) {
					right->value = right->right->value;
					right->left = right->right->left;
					right->right = right->right->right;
					delete right->right;
				}
				else {

					if (right == prev->left) {
						prev->left = NULL;
					}
					else {
						prev->right = NULL;
					}
					delete right;
				}
			}
		}
		else if (left != NULL) {
			node->value = left->value;
			node->left = left->left;
			node->right = left->right;
			delete left;
		}
		else {
			if (node != root) {
				if (node == node_p.second->left) {
					node_p.second->left = NULL;
				}
				else {
					node_p.second->right = NULL;
				}
			}
			else {
				root = NULL;
			}
			delete node;
		}
	}
}*/
pair<Node*, Node*> BTree::search(int key, Node *leaf, Node *parent) {
	if (leaf != NULL) {
		if (key == leaf->value) {
			return pair<Node*, Node*>(leaf, parent);
		}
		if (key < leaf->value) {
			return search(key, leaf->left, leaf);
		}
		else {
			return search(key, leaf->right, leaf);
		}
	}
	else {
		return pair<Node*, Node*>(NULL, parent);
	}
}

pair<Node*, Node*> BTree::search(int key) {
	return search(key, root, NULL);
}

void BTree::destroy_tree() {
	destroy_tree(root);
}

void BTree::inorder_print() {
	inorder_print(root);
	cout << "\n";
}

void BTree::inorder_print(Node *leaf) {
	if (leaf != NULL) {
		inorder_print(leaf->left);
		cout << leaf->value << ",";
		inorder_print(leaf->right);
	}
}

void BTree::postorder_print() {
	postorder_print(root);
	cout << "\n";
}

void BTree::postorder_print(Node *leaf) {
	if (leaf != NULL) {
		inorder_print(leaf->left);
		inorder_print(leaf->right);
		cout << leaf->value << ",";
	}
}

void BTree::preorder_print() {
	preorder_print(root);
	cout << "\n";
}

void BTree::preorder_print(Node *leaf) {
	if (leaf != NULL) {
		cout << leaf->value << ",";
		preorder_print(leaf->left);
		preorder_print(leaf->right);
	}
}
void BTree::preorder_write(ostream& os) {
	preorder_write(root, os);
}
void BTree::preorder_write(Node *leaf, ostream& os) {
	if (leaf != NULL) {
		os << leaf->value << endl;
		preorder_write(leaf->left, os);
		preorder_write(leaf->right, os);
	}
}
int BTree::find_min(Node* node) {
	if (node->left != NULL) {
		return find_min(node->left);
	}
	else {
		return node->value;
	}
}

Node* BTree::remove(Node* v, int x)
{
	if (v == NULL) 
		return NULL;

	if (x < v->value) {
		v->left = remove(v->left, x);
		return v;
	}
	else if (x > v->value) {
		v->right = remove(v->right, x);
		return v;
	}
	
	if (v->left == NULL)
		return v->right;
	else if (v->right == NULL)
		return v->left;
	else {
		int min_key = find_min(v->right);
		v->value = min_key;
		v->right = remove(v->right, min_key);
		return v;
	}
}




void main() 
{
	BTree tree;
	ifstream infile("input.txt");
	ofstream outfile("output.txt");
	int key;
	infile >> key;
	while (!infile.eof())
	{
		int temp;
		infile >> temp;
		tree.insert(temp);
	}
	tree.remove(key);
	tree.preorder_write(outfile);
	//system("pause");
}