/*
	1_1. Выведите элементы в порядке in-order (слева направо).

	Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
	Требуется построить бинарное дерево, заданное наивным порядком вставки.
	Т.е., при добавлении очередного числа K в дерево с корнем root, если root->Key ≤ K, то узел K добавляется в правое поддерево root;
	иначе в левое поддерево root. Балансировку выполнять не требуется.

*/


#include <iostream>
#include <vector>

struct Node {
	int key = 0;
	Node* left = 0;
	Node* right = 0;

	Node(int x) : key(x) {}
};

void push(Node* node, Node*& top) {
	if (top == 0) {
		top = node;
		return;
	}

	if (node->key < top->key) {
		push(node, top->left);
		return;
	}

	push(node, top->right);
}

void print_in_order(Node* node) {

	std::vector<Node*> stack;

	while (stack.size() != 0 || node != nullptr) {

		if (node != nullptr) {
			stack.push_back(node);
			node = node->left;
		} else {
			node = stack.back();
			stack.pop_back();

			std::cout << node->key << " ";

			node = node->right;
		}

	}
}

int main() {

	int n = 0;
	std::cin >> n;

	Node* tree = 0;

	for (int i = 0; i < n; ++i) {

		int x = 0;
		std::cin >> x;

		push(new Node(x), tree);
	}

	print_in_order(tree);

	return 0;
}