#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;


class my_exception : exception {};

class BTree {
    struct Node {
        int value;
        Node *left;
        Node *right;
        Node *parent;
        long long height;
        long long n_paths;
        long long n_deepest;
        long long n_rt;
        Node() {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            height = 0;
            n_paths = 0;
            n_deepest = 1;
            n_rt = 0;
        }
        Node(int value) : value(value) {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            height = 0;
            n_paths = 0;
            n_deepest = 1;
            n_rt = 0;
        }
    };
public:
	BTree() {
        root = nullptr;
        max_len = -1;
    }
	~BTree() {
        destroy_tree();
    }

	void insert(int key) {
        if (root != nullptr) {
            insert(key, root);
        }
        else {
            root = new Node(key);
        }
    }
	void remove(int key) {
        if (root != nullptr) {
            root = remove(root, key);
        }
    }
	void destroy_tree() {
        destroy_tree(root);
    }
	void preorder_write(ostream& os) {
        preorder_write(root, os);
    }
	int find_min(Node* node) {
        if (node->left != nullptr) {
            return find_min(node->left);
        }
        else {
            return node->value;
        }
    }
	void put_heights() {
        put_heights(root);
    }
    void delete_node() {
        put_heights();
        traverse(root);
        search_node(root);
        if (selected.empty()) throw my_exception();
        int k = *min_element(selected.begin(), selected.end());
        remove(k);
    }
private:
    void insert(int key, Node *node) {

        if (key < node->value) {
            if (node->left != nullptr) {
                insert(key, node->left);
            }
            else {
                node->left = new Node(key);
                node->left->parent = node;
            }
        }
        else if (key > node->value) {
            if (node->right != nullptr) {
                insert(key, node->right);
            }
            else {
                node->right = new Node(key);
                node->right->parent = node;
            }
        }

    }
	Node* remove(Node* node, int key) {
        if (node == nullptr)
            return nullptr;

        if (key < node->value) {
            node->left = remove(node->left, key);
            return node;
        }
        else if (key > node->value) {
            node->right = remove(node->right, key);
            return node;
        }

        if (node->left == nullptr)
            return node->right;
        else if (node->right == nullptr)
            return node->left;
        else {
            int min_key = find_min(node->right);
            node->value = min_key;
            node->right = remove(node->right, min_key);
            return node;
        }
    }
	void destroy_tree(Node *leaf) {
        if (leaf != nullptr) {
            destroy_tree(leaf->left);
            destroy_tree(leaf->right);
            delete leaf;
        }
    }

	void preorder_write(Node *node, ostream& os) {
        if (node != nullptr) {
            os << node->value << endl;
            preorder_write(node->left, os);
            preorder_write(node->right, os);
        }
    }
    long long put_heights(Node* node) {
        if (node == nullptr) return -1;
        long long left_h = put_heights(node->left);
        long long right_h = put_heights(node->right);
        long long cur_len;
        cur_len = left_h + right_h + 2;
        if (max_len < cur_len) {
            max_len = cur_len;
        }
        node->height = max(left_h, right_h) + 1;

        if (node->left != nullptr && node->right != nullptr) {
            if (left_h > right_h) {
                node->n_deepest = node->left->n_deepest;
            }
            else if (left_h < right_h) {
                node->n_deepest = node->right->n_deepest;
            }
            else {
                node->n_deepest = node->left->n_deepest + node->right->n_deepest;
            }
        }
        else if (node->left != nullptr) {
            node->n_deepest = node->left->n_deepest;
        }
        else if (node->right != nullptr) {
            node->n_deepest = node->right->n_deepest;
        }

        return node->height;
    }

    void traverse(Node* node) {
        if (node == nullptr) return;
        long long left_h = -1;
        long long right_h = -1;
        long long left_d = 1;
        long long right_d = 1;
        Node* left = node->left;
        Node* right = node->right;
        if (left != nullptr) {
            left_h = left->height;
            left_d = left->n_deepest;
        }
        if (right != nullptr) {
            right_h = right->height;
            right_d = right->n_deepest;
        }
        if (left_h + right_h + 2 == max_len) {
            node->n_rt = left_d * right_d;
        }
        if (left != nullptr && right != nullptr) {
            if (left_h > right_h) {
                left->n_paths = node->n_rt + node->n_paths;
                right->n_paths = node->n_rt;
            } else if (left_h < right_h) {
                right->n_paths = node->n_rt + node->n_paths;
                left->n_paths = node->n_rt;
            } else {
                long long f_left = node->n_paths * left_d / (left_d + right_d);
                left->n_paths = node->n_rt + f_left;
                long long f_right = node->n_paths * right_d / (left_d + right_d);
                right->n_paths = node->n_rt + f_right;
            }
        }
        else if (left != nullptr) {
            left->n_paths = node->n_rt + node->n_paths;
        }
        else if (right != nullptr) {
            right->n_paths = node->n_rt + node->n_paths;
        }
        traverse(left);
        traverse(right);
    }
    void search_node(Node* node) {
        if (node == nullptr) return;
        long long max_paths = node->n_paths+node->n_rt;
        if (max_paths > 0 && max_paths % 2 == 0) {
            selected.push_back(node->value);
        }
        search_node(node->left);
        search_node(node->right);
    }
	Node *root;
    long long max_len;
    vector<int> selected;
};




int main() {
	BTree tree;
	ifstream infile("tst.in");
	ofstream outfile("tst.out");
	while (!infile.eof()) {
		int temp;
		infile >> temp;
		tree.insert(temp);
	}
	try {
        tree.delete_node();
	}
	catch (my_exception ex) {

	}
	tree.preorder_write(outfile);
	//system("pause");
    return 0;
}