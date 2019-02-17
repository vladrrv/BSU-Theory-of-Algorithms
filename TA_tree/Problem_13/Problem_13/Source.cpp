#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include<climits>
using namespace std;

// C++ implementation of worst case linear time algorithm
// to find k'th smallest element

int partition(int arr[], int l, int r, int k);

// A simple function to find median of arr[].  This is called
// only for an array of size 5 in this program.
int findMedian(int arr[], int n)
{
	sort(arr, arr + n);  // Sort the array
	return arr[n / 2];   // Return middle element
}

// Returns k'th smallest element in arr[l..r] in worst case
// linear time. ASSUMPTION: ALL ELEMENTS IN ARR[] ARE DISTINCT
int kthSmallest(int arr[], int l, int r, int k)
{
	// If k is smaller than number of elements in array
	if (k > 0 && k <= r - l + 1)
	{
		int n = r - l + 1; // Number of elements in arr[l..r]

						   // Divide arr[] in groups of size 5, calculate median
						   // of every group and store it in median[] array.
		int i, *median = new int[(n + 4) / 5]; // There will be floor((n+4)/5) groups;
		for (i = 0; i<n / 5; i++)
			median[i] = findMedian(arr + l + i * 5, 5);
		if (i * 5 < n) //For last group with less than 5 elements
		{
			median[i] = findMedian(arr + l + i * 5, n % 5);
			i++;
		}

		// Find median of all medians using recursive call.
		// If median[] has only one element, then no need
		// of recursive call
		int medOfMed = (i == 1) ? median[i - 1] :
			kthSmallest(median, 0, i - 1, i / 2);

		// Partition the array around a random element and
		// get position of pivot element in sorted array
		int pos = partition(arr, l, r, medOfMed);

		// If position is same as k
		if (pos - l == k - 1)
			return arr[pos];
		if (pos - l > k - 1)  // If position is more, recur for left
			return kthSmallest(arr, l, pos - 1, k);

		// Else recur for right subarray
		return kthSmallest(arr, pos + 1, r, k - pos + l - 1);
	}

	// If k is more than number of elements in array
	return INT_MAX;
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// It searches for x in arr[l..r], and partitions the array 
// around x.
int partition(int arr[], int l, int r, int x)
{
	// Search for x in arr[l..r] and move it to end
	int i;
	for (i = l; i<r; i++)
		if (arr[i] == x)
			break;
	swap(&arr[i], &arr[r]);

	// Standard partition algorithm
	i = l;
	for (int j = l; j <= r - 1; j++)
	{
		if (arr[j] <= x)
		{
			swap(&arr[i], &arr[j]);
			i++;
		}
	}
	swap(&arr[i], &arr[r]);
	return i;
}

struct Node {
	int value;
	Node *left;
	Node *right;
	Node *parent;
	int height;
};

class my_exception : exception {

};

// maxLevel : keeps track of maximum level seen so far.
// res :  Value of deepest node so far.
// level : Level of root
void find(Node *root, int level, int &maxLevel, Node*& res)
{
	if (root != NULL)
	{
		find(root->left, ++level, maxLevel, res);

		// Update level and resue
		if (level > maxLevel)
		{
			res = root;
			maxLevel = level;
		}

		find(root->right, level, maxLevel, res);
	}
}

// Returns value of deepest node
Node* deepestNode(Node *root)
{
	// Initialze result and max level
	Node* res = NULL;
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
		if (r != NULL) {
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
	void put_heights();
	void get_leaves();
	int get_paths();
private:
	Node* remove(Node* v, int key);
	void destroy_tree(Node *leaf);
	void insert(int key, Node *leaf);
	pair<Node*,Node*> search(int key, Node *leaf, Node *parent);

	void inorder_print(Node *leaf);
	void postorder_print(Node *leaf);
	void preorder_print(Node *leaf);
	void preorder_write(Node *leaf, ostream& os);
	int put_heights(Node* node);
	void get_leaves(Node *node);
	Node *root;
	vector<Node*> max_roots;
	vector<Node*> leaves;
};


BTree::BTree() {
	root = NULL;
}

BTree::~BTree() {
	destroy_tree();
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
			leaf->left->parent = leaf;
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
			leaf->right->parent = leaf;
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
		root->parent = NULL;
	}
}

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
		cout << leaf->value << "," << leaf->height << "\n";
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

void BTree::remove(int key) {
	if (root != NULL) {
		root = remove(root, key);
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


void BTree::put_heights() {
	put_heights(root);
}
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
int BTree::put_heights(Node* node) {
	static int max_len = 0;
	if (node != NULL) {
		int left_h = put_heights(node->left);
		int right_h = put_heights(node->right);
		int cur_len;
		if (left_h < 0 || right_h < 0) {
			cur_len = left_h + right_h + 2;
		}
		else {
			cur_len = left_h + right_h + 1;
		}
		node->height = MAX(left_h, right_h) + 1;
		if (max_len < cur_len) {
			max_len = cur_len;
			max_roots.clear();
			max_roots.push_back(node);
		}
		else if (max_len == cur_len) {
			max_roots.push_back(node);
		}
		return node->height;
	}
	return -1;
}


void BTree::get_leaves() {
	get_leaves(root);/*
	for (Node* node : leaves) {
		cout << "Leaf: " << node->value << "," << node->height << "\n---\n";
	}*/

}
void BTree::get_leaves(Node *node) {
	if (node != NULL) {
		if (node->left == NULL && node->right == NULL) {
			leaves.push_back(node);
		}
		else {
			get_leaves(node->left);
			get_leaves(node->right);
		}
	}
}

int BTree::get_paths() {
	vector<Path> v;
	for (Node* r : max_roots) {
		Path p(r);
		if (p.left_leaf != NULL && p.right_leaf != NULL) {
			v.push_back(Path(p.root, p.left_leaf->parent, p.right_leaf));
		}
		else if (p.left_leaf == NULL && p.right_leaf != NULL) {
			v.push_back(Path(p.root, p.root, p.right_leaf));
		}
		if (p.right_leaf != NULL && p.left_leaf != NULL) {
			v.push_back(Path(p.root, p.left_leaf, p.right_leaf->parent));
		}
		else if (p.right_leaf == NULL && p.left_leaf != NULL) {
			v.push_back(Path(p.root, p.left_leaf, p.root));
		}/*
		if (p.right_leaf == NULL && p.left_leaf == NULL) {
			v.push_back(Path(p.root, p.root, p.root));
		}*/
	}
	if (v.empty()) return NULL;
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
		//cout << "Path: " << p.left_leaf->value << " " << p.right_leaf->value << endl;
	}
	int median;
	bool init = false;
	for (Path p : selected) {
		vector<Node*> nds;
		//cout << "Sel.Path: " << p.left_leaf->value << " " << p.right_leaf->value << endl;
		Node* n = p.left_leaf;
		while (n->parent != NULL && n != p.root)
		{
			nds.push_back(n);
			n = n->parent;
		}
		nds.push_back(p.root);
		n = p.right_leaf;
		while (n->parent != NULL && n != p.root)
		{
			nds.push_back(n);
			n = n->parent;
		}
		int size = nds.size();
		int cur_median;
		if (size % 2 == 1) {
			int* vals = new int[size];
			int k = 0;
			for (Node* n : nds) {
				vals[k++] = n->value;
			}
			cur_median = kthSmallest(vals, 0, size-1, size / 2 +1);
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

void main() 
{
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
	catch (my_exception ex) {

	}
	//tree.preorder_print();
	tree.preorder_write(outfile);
	//system("pause");
}