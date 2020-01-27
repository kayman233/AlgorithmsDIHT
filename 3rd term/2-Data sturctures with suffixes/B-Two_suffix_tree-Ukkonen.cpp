/*
	B. Two suffix tree

	Даны строки s и t. Постройте сжатое суффиксное дерево, которое содержит все суффиксы строки s и строки t.
	Найдите такое дерево, которое содержит минимальное количество вершин.
*/


#include <iostream>
#include <vector>
#include <string>
#include <map>

class SuffTreeBuilder {
public:
	struct Node {
	public:
		std::map<char, Node*> children;

		Node(int start, int* end, Node* root) :
			start(start), end(end),
			suffix_link(root), children() {}

		int getEnd() { return (*end); }
		int getStart() { return start; };
		void addToStart(int length) { start += length; }
		int getLength() { return *(end)-(start) + 1; }

		bool hasNext(char c);
		Node* getNext(char c);

		void addChild(char c, Node* node);

		Node* getSuffLink() { return suffix_link; }
		void setSuffLink(Node* node) { suffix_link = node; }

	private:
		int start;
		int *end;
		Node* suffix_link;
	};

	Node* root;
	
	SuffTreeBuilder(std::string& str, int first_str_len) :
		root(nullptr), last_new_node(nullptr), active_node(nullptr),
		active_edge(-1), active_length(0), remainder(0),
		leaf_end(-1), split_end(nullptr), str(str), size(1), 
		first_str_len(first_str_len) 
	{
		BuildTree();
	}

	void BuildTree(); // to build

	void PrintTreeAndSize();
	void Print(Node* node, int& num_in_dfs, int parent_num);
private:
	Node* last_new_node;
	Node* active_node;

	int active_edge;
	int active_length;
	int remainder;

	int leaf_end;
	int* split_end;

	std::string str;

	// to get answer for the task
	int size;
	int first_str_len;

	void ExtendTree(int pos);
};

bool SuffTreeBuilder::Node::hasNext(char c) {
	if (children.find(c) != children.end()) {
		return true;
	}
	return false;
}

SuffTreeBuilder::Node* SuffTreeBuilder::Node::getNext(char c) {
	auto it = children.find(c);
	return it->second;
}

void SuffTreeBuilder::Node::addChild(char c, Node* node) {
	auto it = children.find(c);
	if (it == children.end()) {
		children.insert(std::make_pair(c, node));
	} else {
		it->second = node; //rewrite if needed
	}
}

void SuffTreeBuilder::BuildTree() { // Ukkonen's algorithm
	int length = str.length();
	int* root_end = new int;
	*root_end = -1;

	root = new Node(-1, root_end, root);

	active_node = root;
	for (int i = 0; i < length; i++) {
		ExtendTree(i);
	}
}

void SuffTreeBuilder::ExtendTree(int pos) {
	last_new_node = nullptr;
	leaf_end = pos;

	++remainder;
	while (remainder > 0) {

		if (active_length == 0) {
			active_edge = pos;
		}

		if (active_node->hasNext(str[active_edge])) {
			Node* next = active_node->getNext(str[active_edge]); // go to this node

			int next_len = next->getLength();
			if (active_length >= next_len) { //if we need to go to the next node
				active_node = next;

				active_length -= next_len;
				active_edge += next_len;
				continue;
			}

			int pos_in = next->getStart() + active_length;
			if (str[pos_in] == str[pos]) {
				if (last_new_node != nullptr && active_node != root) { //set suffix link
					last_new_node->setSuffLink(active_node);
					last_new_node = nullptr;
				}
				++active_length; // we just need to remember new length
				break;
			}

			// if active_pointer is on the edge, we need to split for new node

			int* split_end = new int;
			*split_end = pos_in - 1;

			++size;
			Node* new_node = new Node(next->getStart(), split_end, root);
			active_node->addChild(str[next->getStart()], new_node);

			// create new leaf 
			++size;
			Node* new_leaf = new Node(pos, &leaf_end, root);
			new_node->addChild(str[pos], new_leaf);
			next->addToStart(active_length);
			new_node->children[str[next->getStart()]] = next;

			if (last_new_node != nullptr) {
				last_new_node->setSuffLink(new_node);
			}

			last_new_node = new_node;
		} else {
			++size;
			Node* new_leaf = new Node(pos, &leaf_end, root); // new leaf
			active_node->addChild(str[active_edge], new_leaf);

			if (last_new_node != nullptr) { // set suffix link from previous step
				last_new_node->setSuffLink(active_node);
				last_new_node = nullptr;
			}
		}

		--remainder;

		if (active_node == root && active_length > 0) {
			--active_length;
			active_edge = pos - remainder + 1; // new edge
		}
		else if (active_node != root) {
			active_node = active_node->getSuffLink(); // go to suffix_link
		}
	}
}

void SuffTreeBuilder::Print(Node* node, int& num_in_dfs, int parent_num) {
	if (node->getStart() < first_str_len) {
		if (node->getEnd() < first_str_len) {
			printf("%d %d %d %d\n", parent_num, 0, node->getStart(), node->getEnd() + 1);
		} else {
			printf("%d %d %d %d\n", parent_num, 0, node->getStart(), first_str_len);
		}
	} else {
		printf("%d %d %d %d\n", parent_num, 1, node->getStart() - first_str_len, node->getEnd() + 1 - first_str_len);
	}

	if (node->children.size() == 0) { // if it's a leaf node
		return;
	}

	parent_num = num_in_dfs;
	for (auto it = node->children.begin(); it != node->children.end(); ++it) { // go in children
		++num_in_dfs;
		Print(it->second, num_in_dfs, parent_num);
	}
}

void SuffTreeBuilder::PrintTreeAndSize() {
	printf("%d\n", size); // printf for faster Print
	int num_in_dfs = 0;
	for (auto it = root->children.begin(); it != root->children.end(); ++it) {
		++num_in_dfs;
		Print(it->second, num_in_dfs, 0); // go in children
	}
}

int main() {

	std::string str1;
	std::string str2;
	std::cin >> str1 >> str2;

	str1 += str2;

	SuffTreeBuilder suff_tree_builder(str1, str1.length() - str2.length());

	suff_tree_builder.PrintTreeAndSize();

	return 0;
}