#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Node {
    int key;
    Node* left;
    Node* right;
    int depth;
    int height;
    bool* paths;
    int* type;
public:
    Node(int val) {
        key = val;
        left = nullptr;
        right = nullptr;
    }

    int getKey() const {
        return key;
    }

    void setKey(int key) {
        Node::key = key;
    }

    Node *getLeft() const {
        return left;
    }

    void setLeft(Node *left) {
        Node::left = left;
    }

    Node *getRight() const {
        return right;
    }

    void setRight(Node *right) {
        Node::right = right;
    }

    int getDepth() const {
        return depth;
    }

    void setDepth(int depth) {
        Node::depth = depth;
    }

    int getHeight() const {
        return height;
    }

    void setHeight(int height) {
        Node::height = height;
    }

    bool *getPaths() const {
        return paths;
    }

    void setPaths(bool *paths) {
        Node::paths = paths;
    }

    int* getType() const {
        return type;
    }

    void setType(int* type) {
        Node::type = type;
    }
};

class Tree {
    Node* root;

public:
    Tree() {
        root = nullptr;
    }
    Node *getRoot() const {
        return root;
    }

    void setRoot(Node *root) {
        Tree::root = root;
    }

    void addNode(int val, Node* node) {
        if (node == nullptr) {
            auto newNode = new Node(val);
            setRoot(newNode);
        }
        else {
            while (true) {
                if (node->getKey() > val)
                    if (node->getLeft() != nullptr)
                        node = node->getLeft();
                    else {
                        auto newNode = new Node(val);
                        node->setLeft(newNode);
                        break;
                    }
                else if (node->getKey() <= val)
                    if (node->getRight() != nullptr)
                        node = node->getRight();
                    else {
                        auto newNode = new Node(val);
                        node->setRight(newNode);
                        break;
                    }
            }
        }
    }

    int getHeight() {
        return getRoot()->getHeight();
    }

    void setPaths(Node* r, int h) {
        if (r != nullptr) {
            bool* arr = new bool[h];
            int* t = new int[h];
            for (int i = 0; i < h; i++) {
                arr[i] = false;
                t[i] = -1; //none
            }
            if (r->getLeft() == nullptr && r->getRight() == nullptr) {
                arr[0] = true;
                t[0] = 2; //both
            }
            else if (r->getRight() == nullptr && r->getLeft() != nullptr) {
                setPaths(r->getLeft(), h);
                bool* left = r->getLeft()->getPaths();
                for (int i = 0; i < h; i++)
                    if (left[i] == true) {
                        int j = i + 1;
                        arr[j] = true;
                        t[j] = 0; //left
                    }
            }
            else if (r->getLeft() == nullptr && r->getRight() != nullptr) {
                setPaths(r->getRight(), h);
                bool* right = r->getRight()->getPaths();
                for (int i = 0; i < h; i++)
                    if (right[i] == true) {
                        int j = i + 1;
                        arr[j] = true;
                        t[j] = 1; //right
                    }
            }
            else {
                setPaths(r->getLeft(), h);
                setPaths(r->getRight(), h);
                bool* left = r->getLeft()->getPaths();
                bool* right = r->getRight()->getPaths();
                for (int i = 0; i < h; i++)
                    if (left[i] == true) {
                        int j = i + 1;
                        arr[j] = true;
                        if (right[i] == true)
                            t[j] = 2;
                        else
                            t[j] = 0;
                    } else if (right[i] == true) {
                        int j = i + 1;
                        arr[j] = true;
                        t[j] = 1; //right
                    }
                }
            r->setPaths(arr);
            r->setType(t);
        }
    }

    void semipath(Node* r, int h, vector<Node*>& arr) {
        if (r == nullptr)
            return;
        else {
            if (r->getLeft() != nullptr)
                semipath(r->getLeft(), h, arr);
            for (int i = 1; i <= h/2; i++) {
                if (r->getPaths()[i])
                    if (r->getType()[i] == 2) {
                        if (i * 2 == h)
                            if (!arr.empty()) {
                                if (r->getDepth() == arr[0]->getDepth())
                                    arr.push_back(r);
                                else if (r->getDepth() > arr[0]->getDepth()) {
                                    arr.clear();
                                    arr.push_back(r);
                                }
                            }
                            else
                                arr.push_back(r);
                    } else if (r->getType()[i] == 0) {
                        if (r->getPaths()[h - i] && ((h - i) > i))
                            if ((r->getType()[h - i] == 1) || (r->getType()[h - i] == 2))
                                if (!arr.empty()) {
                                    if (r->getDepth() == arr[0]->getDepth())
                                        arr.push_back(r);
                                    else if (r->getDepth() > arr[0]->getDepth()) {
                                        arr.clear();
                                        arr.push_back(r);
                                    }
                                }
                                else
                                    arr.push_back(r);
                    } else if (r->getType()[i] == 1) {
                        if (r->getPaths()[h - i])
                            if ((r->getType()[h - i] == 0) || (r->getType()[h - i] == 2))
                                if (!arr.empty()) {
                                    if (r->getDepth() == arr[0]->getDepth())
                                        arr.push_back(r);
                                    else if (r->getDepth() > arr[0]->getDepth()) {
                                        arr.clear();
                                        arr.push_back(r);
                                    }
                                }
                                else
                                    arr.push_back(r);
                    }
            }
            if (r->getRight() != nullptr)
                semipath(r->getRight(), h, arr);
        }
    }

    int allSetHeight(Node* r) {
        if (r != nullptr) {
            if (r->getLeft() == nullptr && r->getRight() == nullptr) {
                r->setHeight(0);
                return 0;
            }
            else {
                int left = allSetHeight(r->getLeft());
                int right = allSetHeight(r->getRight());
                r->setHeight(max(left, right) + 1);
                return r->getHeight();
            }
        }
        else
            return -1;
    }
    void setDepths(Node* r, int d) {
        if (r == nullptr)
            return;
        if (r->getLeft() == nullptr && r->getRight() == nullptr) {
            r->setDepth(d);
            return;
        }
        else {
            r->setDepth(d);
            d++;
            setDepths(r->getLeft(), d);
            setDepths(r->getRight(), d);
        }
    }

    void preOrder(Node* r, vector<int>& pre) {
        if (r != nullptr) {
            pre.push_back(r->getKey());
            /*cout << "\n\nKey: " << r->getKey() << " Height " << r->getHeight() << " Depth " << r->getDepth();
            for (int i = 0; i <= this.getHeight(); i++)
                cout << i << " ";
            cout << endl;
            for (int i = 0; i <= this.getHeight(); i++)
                cout << r->getPaths()[i] << " ";
            cout << endl;
            for (int i = 0; i <= this.getHeight(); i++)
                cout << r->getType()[i] << " ";*/
            preOrder(r->getLeft(), pre);
            preOrder(r->getRight(), pre);
        }
    }

    Node* remove(Node* root, int key) {
        if (root == nullptr)
            return root;
        if (key < root->getKey()) {
            root->setLeft(remove(root->getLeft(), key));
            return root;
        } else if (key > root->getKey()) {
            root->setRight(remove(root->getRight(), key));
            return root;
        }
        if (root->getLeft() == nullptr)
            return root->getRight();
        else if (root->getRight() == nullptr)
            return root->getLeft();
        else {
            Node* left = findMin(root->getRight());
            root->setKey(left->getKey());
            root->setRight(remove(root->getRight(), left->getKey()));
            return root;
        }
    }

    void preorder_write(ostream& os) {
        os << root->getKey() << flush;
        preorder_write(root->getLeft(), os);
        preorder_write(root->getRight(), os);
    }
    void preorder_write(Node *node, ostream& os) {
        if (node != nullptr) {
            os << endl << node->getKey() << flush;
            preorder_write(node->getLeft(), os);
            preorder_write(node->getRight(), os);
        }
    }
    Node* findMin(Node* root) {
        if (root->getLeft() == nullptr)
            return root;
        else
            return findMin(root->getLeft());
    }
};


int main() {
    ifstream infile("tst.in");
    ofstream outfile("tst.out");
    Tree tree;
    while (!infile.eof()) {
        int temp;
        infile >> temp;
        tree.addNode(temp, tree.getRoot());
    }
    tree.allSetHeight(tree.getRoot());
    int h = tree.getHeight();
    tree.setDepths(tree.getRoot(), 0);
    tree.setPaths(tree.getRoot(), (h + 1));
    vector<Node*> arr;
    tree.semipath(tree.getRoot(), h, arr);
    //cout << "\n\n\nROOTS";
    //for (int i = 0; i < arr.size(); i++)
    //    cout << arr[i]->getKey() << " ";
    if (!arr.empty())
        if (arr.size() % 2 == 1)
            tree.setRoot(tree.remove(tree.getRoot(), arr[arr.size() / 2]->getKey()));
    tree.preorder_write(outfile);
    return 0;
}