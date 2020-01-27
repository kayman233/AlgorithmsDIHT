/*
	2_1. Вычислить разницу глубин наивного дерева поиска и декартового дерева. Разница может быть отрицательна.

	Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.

	Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi). Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
	Добавление узла в декартово дерево выполняйте следующим образом: При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
	Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x, а во втором больше или равны x.
	Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый узел вставьте на место узла P.

	Построить также наивное дерево поиска по ключам Xi.
	Т.е., при добавлении очередного числа K в наивное дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root;
	иначе в левое поддерево root.

*/

#include <iostream>
#include <algorithm>

/////////////////
// Naive tree
/////////////////
struct Node {
	int key = 0;
	Node* left = nullptr;
	Node* right = nullptr;

	Node(int x) : key(x) {}
};

class Naive_tree {
public:
	Naive_tree();
	~Naive_tree();

	void InsertNode(int x);
	int TreeHeight();
private:
	Node* root;

	void InsertNode(Node* node, Node*& current_node);
	void DeleteTree(Node* node);
	int TreeHeight(Node* current);
};

Naive_tree::Naive_tree() {
	root = nullptr;
}
Naive_tree::~Naive_tree() {
	DeleteTree(root);
}

void Naive_tree::InsertNode(int x) {
	Node* node = new Node(x);

	InsertNode(node, root);
}

void Naive_tree::InsertNode(Node* node, Node*& current_node) {
	if (current_node == nullptr) {
		current_node = node;
		return;
	}

	if (node->key < current_node->key) {
		InsertNode(node, current_node->left);
		return;
	}

	InsertNode(node, current_node->right);
}

int Naive_tree::TreeHeight() {
	if (root == nullptr) {
		return 0;
	}
	return 1 + std::max(TreeHeight(root->left), TreeHeight(root->right));
}

int Naive_tree::TreeHeight(Node* current) {
	if (current == 0) {
		return 0;
	}
	return 1 + std::max(TreeHeight(current->left), TreeHeight(current->right));
}

void Naive_tree::DeleteTree(Node* p) {
	if (p != nullptr) {
		DeleteTree(p->left);
		DeleteTree(p->right);
		delete p;
	}
}

/////////////////
// Treap
/////////////////

struct TreapNode {
	int X = 0;
	int Y = 0;
	TreapNode* left = nullptr;
	TreapNode* right = nullptr;

	TreapNode(int x, int y) : X(x), Y(y) {}
};

class Treap {
public:
	Treap();
	~Treap();

	int TreeHeight();
	void InsertNode(int x, int y);
private:
	TreapNode* root;

	void Split(TreapNode* current_node, int key, TreapNode*& left, TreapNode*& right);
	void InsertNode(TreapNode* node, TreapNode*& current_node);
	void DeleteTree(TreapNode* node);
	int TreeHeight(TreapNode* top);
};

Treap::Treap() {
	root = nullptr;
}

Treap::~Treap() {
	DeleteTree(root);
}

void Treap::Split(TreapNode* current_node, int key, TreapNode*& left, TreapNode*& right) {
	if (current_node == nullptr) {
		left = 0;
		right = 0;
	} else {
		if (key < current_node->X) {
			Split(current_node->left, key, left, current_node->left);
			right = current_node;
		} else {
			Split(current_node->right, key, current_node->right, right);
			left = current_node;
		}
	}
}

void Treap::InsertNode(int x, int y) {
	TreapNode* node = new TreapNode(x, y);
	InsertNode(node, root);
}

void Treap::InsertNode(TreapNode* node, TreapNode*& current_node) {
	if (current_node == nullptr) {
		current_node = node;
		return;
	}

	if (node->Y < current_node->Y) {
		if (node->X < current_node->X) {
			InsertNode(node, current_node->left);
		} else {
			InsertNode(node, current_node->right);
		}
	} else {
		Split(current_node, node->X, node->left, node->right);
		current_node = node;
	}
}

int Treap::TreeHeight() {
	if (root == nullptr) {
		return 0;
	}
	return 1 + std::max(TreeHeight(root->left), TreeHeight(root->right));
}

int Treap::TreeHeight(TreapNode* current) {
	if (current == nullptr) {
		return 0;
	}
	return 1 + std::max(TreeHeight(current->left), TreeHeight(current->right));
}

void Treap::DeleteTree(TreapNode* p) {
	if (p != nullptr) {
		DeleteTree(p->left);
		DeleteTree(p->right);
		delete p;
	}
}

int main() {

	int n = 0;
	std::cin >> n;

	Naive_tree naive_tree;
	Treap treap;

	for (int i = 0; i < n; ++i) {

		int x = 0;
		int y = 0;
		std::cin >> x >> y;

		naive_tree.InsertNode(x);
		treap.InsertNode(x, y);
	}

	int difference = naive_tree.TreeHeight() - treap.TreeHeight();
	std::cout << difference << std::endl;

	return 0;
}