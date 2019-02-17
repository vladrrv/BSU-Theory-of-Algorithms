#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include<climits>
using namespace std;

class my_exception : exception {};

class BTree {
    struct Path;
    struct Node {
        int value;
        Node *left;
        Node *right;
        Node *parent;
        long long height;
        long long n_deepest;
        Path* p;
        Node() {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            height = 0;
            n_deepest = 1;
        }
        Node(int value) : value(value) {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            height = 0;
            n_deepest = 1;
        }
    };
// maxLevel : keeps track of maximum level seen so far.
// res :  Value of deepest node so far.
// level : Level of root
    static void find(Node *root, int level, int &maxLevel, Node*& res) {
        if (root != nullptr) {
            find(root->left, ++level, maxLevel, res);
            // Update level and resue
            if (level > maxLevel) {
                res = root;
                maxLevel = level;
            }
            find(root->right, level, maxLevel, res);
        }
    }
// Returns value of deepest node
    static Node* deepestNode(Node *root) {
        // Initialze result and max level
        Node* res = nullptr;
        int maxLevel = -1;

        // Updates value "res" and "maxLevel"
        // Note that res and maxLen are passed
        // by reference.
        find(root, 0, maxLevel, res);
        return res;
    }

    struct Path {
        Node* root;
        Node* left_leaf;
        Node* right_leaf;
        Path(Node* r) {
            if (r != nullptr) {
                root = r;
                left_leaf = deepestNode(r->left);
                right_leaf = deepestNode(r->right);
            }
        }
        Path(Node* r, Node* l_l, Node* r_l) {
            root = r;
            left_leaf = l_l;
            right_leaf = r_l;
        }
        int get_sum() {
            return left_leaf->value + right_leaf->value;
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
    void traverse_inorder(Path* p, Node* node, vector<int>& nodes) {
        if (node == nullptr || node->p != p) return;
        traverse_inorder(p, node->left, nodes);
        nodes.push_back(node->value);
        traverse_inorder(p, node->right, nodes);
    }
    int get_paths() {
        vector<Path> v;
        for (Node* r : max_roots) {
            Path p(r);
            if (p.left_leaf != nullptr && p.right_leaf != nullptr) {
                v.emplace_back(p.root, p.left_leaf->parent, p.right_leaf);
            }
            else if (p.left_leaf == nullptr && p.right_leaf != nullptr) {
                v.emplace_back(p.root, p.root, p.right_leaf);
            }
            if (p.right_leaf != nullptr && p.left_leaf != nullptr) {
                v.emplace_back(p.root, p.left_leaf, p.right_leaf->parent);
            }
            else if (p.right_leaf == nullptr && p.left_leaf != nullptr) {
                v.emplace_back(p.root, p.left_leaf, p.root);
            }/*
		if (p.right_leaf == nullptr && p.left_leaf == nullptr) {
			v.push_back(Path(p.root, p.root, p.root));
		}*/
        }
        if (v.empty()) throw my_exception();
        int min_sum = v[0].get_sum()+1;
        int min_root = v[0].root->value + 1;
        vector<Path> selected;
        for (Path p : v) {
            int cur_sum = p.get_sum();
            int cur_root = p.root->value;
            if (cur_sum < min_sum) {
                min_sum = cur_sum;
                min_root = cur_root;
                selected.clear();
                selected.push_back(p);
            }
            else if (cur_sum == min_sum && cur_root < min_root) {
                min_root = cur_root;
                selected.clear();
                selected.push_back(p);
            }
            else if (cur_sum == min_sum && cur_root == min_root) {
                selected.push_back(p);
            }
            cout << "Path: " << p.left_leaf->value << " " << p.right_leaf->value << endl;
        }
        int median;
        bool init = false;
        for (Path &p : selected) {
            cout << "Sel.Path: " << p.left_leaf->value << " " << p.right_leaf->value << endl;
            Node* n = p.left_leaf;
            while (n->parent != nullptr && n != p.root) {
                n->p = &p;
                n = n->parent;
            }
            p.root->p = &p;
            n = p.right_leaf;
            while (n->parent != nullptr && n != p.root) {
                n->p = &p;
                n = n->parent;
            }
            vector<int> vals;
            traverse_inorder(&p, p.root, vals);
            long long size = vals.size();
            int cur_median;
            if (size % 2 == 1) {
                cur_median = vals[size / 2];
                if (!init) {
                    median = cur_median;
                    init = true;
                }
                else if (cur_median != median) throw my_exception();
            }
            else throw my_exception();
        }
        return median;
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
        if (left_h < 0 || right_h < 0) {
            cur_len = left_h + right_h + 2;
        }
        else {
            cur_len = left_h + right_h + 1;
        }
        if (max_len < cur_len) {
            max_len = cur_len;
            max_roots.clear();
            max_roots.push_back(node);
        }
        else if (max_len == cur_len) {
            max_roots.push_back(node);
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
    Node *root;
    long long max_len;
    vector<Node*> max_roots;
};


int main() {
    BTree tree;
    ifstream infile("tst.in");
    ofstream outfile("tst.out");
    while (!infile.eof())
    {
        int temp;
        infile >> temp;
        tree.insert(temp);
    }
    tree.put_heights();
    try {
        int to_delete = tree.get_paths();
        tree.remove(to_delete);
    }
    catch (my_exception& ex) {

    }
    tree.preorder_write(outfile);
}