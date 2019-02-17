#include <iostream>
#include <fstream>
using namespace std;

class my_exception : exception {};

class BTree {
    struct Node {
        int value;
        Node *left;
        Node *right;
        Node *parent;
        long long n_left_children;
        long long n_right_children;
        Node() {
            value = INT32_MAX;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            n_left_children = 0;
            n_right_children = 0;
        }
        explicit Node(int value) : Node() {
            Node::value = value;
        }
        bool has_left() {
            return left != nullptr;
        }
        bool has_right() {
            return right != nullptr;
        }
        bool has_both() {
            return has_left() && has_right();
        }
        bool has_one() {
            return has_left() != has_right();
        }
        bool has_parent() {
            return parent != nullptr;
        }
        Node* not_null_child() {
            if (has_left()) return left;
            if (has_right()) return right;
        }
        bool is_left() {
            return (parent != nullptr && this == parent->left);
        }
        bool is_right() {
            return (parent != nullptr && this == parent->right);
        }
    };
public:
    BTree() {
        root = nullptr;
        size = 0;
    }
    static int analyze(BTree& a, BTree& b) {
        if (abs(a.size - b.size) > 1 || a.size == b.size) throw my_exception();
        Node
                *del = nullptr,
                *root_a = a.root,
                *root_b = b.root;
        if (a.size < b.size) {
            Node* temp = root_a;
            root_a = root_b;
            root_b = temp;
        }
        traverse_parallel(root_a, root_b, del);
        if (del != nullptr) {
            //cout << del->value;
            set_max(del);
            //cout << del->value;
        }
        else {
            throw my_exception();
        }
        return del->value;
    }
    void insert(int key) {
        if (root != nullptr) {
            insert(key, root);
        }
        else {
            size++;
            root = new Node(key);
        }
    }
private:
    static void set_max(Node*& node) {
        if (node->is_left()) {
            if (!node->has_right() || (!node->parent->has_right() && !node->parent->is_right())) {
                do {
                    node = node->parent;
                    if (node->is_right() || node->has_right()) break;
                } while (node->has_parent());
                if (node->has_one()) set_max(node);
            }
            return;
        }

        if (node->is_right()) {
            Node *temp1 = node, *temp2 = node;
            if (node->has_right() && (!node->right->has_left())) {
                temp1 = node->right;
            }
            else if (node->has_right()) {
                return;
            }
            while (temp2->is_right()) {
                temp2 = temp2->parent;
                if (temp2->is_left() && temp2->parent->has_right()) {
                    temp1= temp2->parent;
                    break;
                }
            }
            node = temp1;
        }
    }
    static void traverse_parallel_eq(Node* node_x, Node* node_y) {
        if (node_x != nullptr && node_y != nullptr) {
            if (node_x->n_left_children != node_y->n_left_children || node_x->n_right_children != node_y->n_right_children)
                throw my_exception();
            traverse_parallel_eq(node_x->left, node_y->left);
            traverse_parallel_eq(node_x->right, node_y->right);
        }
        else if (node_x != nullptr || node_y != nullptr) throw my_exception();
    }
    static void traverse_parallel(Node* node_x, Node* node_y, Node*& del) {
        if (node_y == nullptr) {
            del = node_x;
            return;
        }
        if (node_x->has_one() && node_y->has_both()) {
            del = node_x;
            traverse_parallel_eq(node_x->not_null_child(), node_y);
            return;
        }
        if (node_x->has_one() && node_y->has_one() && (node_x->has_left() != node_y->has_left())) {
            del = node_x;
            traverse_parallel_eq(node_x->not_null_child(), node_y);
            return;
        }
        long long
                d_l = node_x->n_left_children - node_y->n_left_children,
                d_r = node_x->n_right_children - node_y->n_right_children;
        if (d_l == 0 && d_r == 1) {
            traverse_parallel_eq(node_x->left, node_y->left);
            traverse_parallel(node_x->right, node_y->right, del);
        }
        else if (d_r == 0 && d_l == 1) {
            traverse_parallel_eq(node_x->right, node_y->right);
            traverse_parallel(node_x->left, node_y->left, del);
        }
        else throw my_exception();
    }
    int find_min(Node* node) {
        if (node->left != nullptr) {
            return find_min(node->left);
        }
        else {
            return node->value;
        }
    }
    void insert(int key, Node *node) {
        if (key < node->value) {
            ++(node->n_left_children);
            if (node->left != nullptr) {
                insert(key, node->left);
            }
            else {
                node->left = new Node(key);
                node->left->parent = node;
                ++size;
            }
        }
        else if (key > node->value) {
            ++(node->n_right_children);
            if (node->right != nullptr) {
                insert(key, node->right);
            }
            else {
                node->right = new Node(key);
                node->right->parent = node;
                ++size;
            }
        }
    }

    long long size;
    Node *root;
};


int main() {
    BTree tree1, tree2, *tr_in = &tree1;
    ifstream infile("tst.in");
    ofstream outfile("tst.out");

    while (!infile.eof()) {
        char temp[32];
        infile.getline(temp, 32);
        if (temp[0] == '*') {
            tr_in = &tree2;
            continue;
        }
        else if (temp[0] == 0) break;
        tr_in->insert(atoi(temp));
    }
    try {
        int to_delete = BTree::analyze(tree1, tree2);
        outfile << "YES\n" << to_delete;
    }
    catch (my_exception& ex) {
        outfile << "NO";
    }
    return 0;
}