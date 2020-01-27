/*
	B. Сумма на подотрезке
	Дан массив из целых чисел a1, a2, ..., an (индексация с 1!).
	Для каждого запроса [left, right] найдите такой подотрезок 
	al, al+1, ..., ar этого массива (1 <= left <= l <= r <= right <= n),
	что сумма чисел al + al+1 + ... + ar является максимально возможной.
	Требуемое время ответа на запрос - O(log n).
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define inf 100000

struct Node {
	int sum;
	int max_segment;

	int prefix_max_sum;
	int suffix_max_sum;
	Node();
};

Node::Node() {
	sum = -inf;
	max_segment = -inf;

	prefix_max_sum = -inf;
	suffix_max_sum = -inf;
}

Node MergeNodes(Node left, Node right) {
	Node new_node;

	new_node.sum = left.sum + right.sum;
	new_node.max_segment = std::max({ left.max_segment, right.max_segment, left.suffix_max_sum + right.prefix_max_sum }); // учитываем и внутреннее - то есть суффикс в левой и префикс в правом

	new_node.prefix_max_sum = std::max(left.prefix_max_sum, left.sum + right.prefix_max_sum); //также учитываем, что есть в правом
	new_node.suffix_max_sum = std::max(right.suffix_max_sum, right.sum + left.suffix_max_sum); //аналогично

	return new_node;
}

class SegmentTree {
public:
	SegmentTree(std::vector<int>& leaves);
	void BuildTree(std::vector<int>& leaves, int vertex, int l, int r);

	int GetAnswer(int l, int r);
	Node GetAnswer(int vertex, int cur_l, int cur_r, int l, int r);
private:
	std::vector<Node> tree;
	int tree_size;
};

SegmentTree::SegmentTree(std::vector<int>& leaves) {
	int size = leaves.size();
	tree_size = size - 1;
	tree.resize(size * 4);

	BuildTree(leaves, 0, 0, size - 1);
}

void SegmentTree::BuildTree(std::vector<int>& leaves, int vertex, int l, int r) {
	if (l == r) {
		tree[vertex].sum = leaves[l];
		tree[vertex].max_segment = leaves[l];

		tree[vertex].suffix_max_sum = leaves[l];
		tree[vertex].prefix_max_sum = leaves[l];
	} else {
		int m = (l + r) / 2;
		BuildTree(leaves, 2 * vertex + 1, l, m);
		BuildTree(leaves, 2 * vertex + 2, m + 1, r);

		tree[vertex] = MergeNodes(tree[2 * vertex + 1], tree[2 * vertex + 2]);
	}
}


int SegmentTree::GetAnswer(int l, int r) {
	return GetAnswer(0, 0, tree_size, l, r).max_segment;
}

Node SegmentTree::GetAnswer(int vertex, int cur_l, int cur_r, int l, int r) {
	// Если полностью в сегменте 
	if (l <= cur_l && r >= cur_r) {
		return tree[vertex];
	}

	// Если не входит
	if (cur_r < l || cur_l > r) {
		Node node_none;

		return node_none;
	}

	// Если частично, то по сыновьям 
	int m = (cur_l + cur_r) / 2;
	Node left = GetAnswer(2 * vertex + 1, cur_l, m, l, r);
	Node right = GetAnswer(2 * vertex + 2, m + 1, cur_r, l, r);

	Node new_node = MergeNodes(left, right);

	return new_node;
}

int main() {
	int n = 0;
	while (std::cin >> n) {
		int m = 0;

		std::cin >> m;
		std::vector<int> segment;
		for (int i = 0; i < n; ++i) {
			int num = 0;
			std::cin >> num;
			segment.push_back(num);
		}

		SegmentTree segment_tree(segment);

		for (int i = 0; i < m; ++i) {
			int r = 0;
			int l = 0;
			std::cin >> l >> r;
			int answer = segment_tree.GetAnswer(l - 1, r - 1);
			std::cout << answer << std::endl;
		}
	}

	return 0;
}