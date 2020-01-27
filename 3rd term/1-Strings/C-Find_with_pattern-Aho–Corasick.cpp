/*
	C. Шаблон с ?

	Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”.
	Найти позиции всех вхождений шаблона в тексте длины n. Каждое вхождение шаблона предполагает, 
	что все обычные символы совпадают с соответствующими из текста, а вместо символа “?” в тексте встречается произвольный символ.

	Время работы - O(n + m + Z), 
	где Z - общее -число вхождений подстрок шаблона “между вопросиками” в исходном тексте. 
	m ≤ 5000, n ≤ 2000000.


*/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>

class Node {
public:

	Node* suff_link;
	Node* up_link;
	Node* parent;

	std::map<char, Node*> next_vert;
	std::map<char, Node*> links;

	std::vector<std::pair<int, int>> pattern_pos_len;

	Node() : next_vert(), leaf(0), symbol('\0'),			//2 constructors
		suff_link(nullptr), up_link(nullptr),
		parent(nullptr), links() {}
	Node(char symbol, Node* parent) : next_vert(), leaf(0),
		symbol(symbol), suff_link(nullptr), up_link(nullptr),
		parent(parent), links() {}

	char get_symbol() {
		return symbol;
	}

	void make_leaf() {
		leaf = true;
	}

	bool is_leaf() {
		return leaf;
	}

	void set_pos_len(int pos, int length) {
		pattern_pos_len.push_back(std::make_pair(pos, length));
	}

private:
	char symbol;
	bool leaf;
};


void AddPattern(const std::string& str, int pos, Node* node) {

	for (int i = 0; i < str.length(); ++i) {
		char sym = str[i];
		if (node->next_vert.find(sym) == node->next_vert.end()) {
			Node* new_node = new Node(sym, node);
			node->next_vert.insert(std::make_pair(sym, new_node));
		}
		node = node->next_vert.at(sym);
		
	}
	node->set_pos_len(pos, str.length());
	node->make_leaf();
}

Node* FindLink(Node* node, char sym, Node* root);

Node* find_suff_link(Node* node, Node* root) {
	if (node->suff_link == nullptr) {
		if ((node == root) || (node->parent == root)) {
			node->suff_link = root;
		} else {
			node->suff_link = FindLink(
				find_suff_link(node->parent, root), node->get_symbol(), root
			);
		}
	}
	return node->suff_link;
}

Node* FindLink(Node* node, char sym, Node* root) {

	auto ite = node->links.find(sym);

	if (ite == node->links.end()) {
		auto it = node->next_vert.find(sym);
		if (it != node->next_vert.end()) {
			node->links.insert(std::make_pair(sym, it->second));
		} else {
			if (node == root) {
				node->links.insert(std::make_pair(sym, root));
			} else {
				node->links.insert( 
					std::make_pair(
						sym, 
						FindLink( find_suff_link(node, root), sym, root)
					) 
				);
			}
		}
	}

	ite = node->links.find(sym);
	return ite->second;
}

Node* FindUpLink(Node* node, Node* root) {
	if (node->up_link == nullptr) {
		Node* suff_link = find_suff_link(node, root);
		if (suff_link->is_leaf()) {
			node->up_link = suff_link;
		} else {
			if (suff_link == root) {
				node->up_link = root;
			} else {
				node->up_link = FindUpLink(suff_link, root);
			}
		}
	}

	return node->up_link;
}

void BuildLinks(Node* root) {				// 2 bfs

	std::queue<Node*> queue;

	queue.push(root);						//for suffix_links
	while (queue.size() != 0) {
		Node* node = queue.front();
		queue.pop();

		find_suff_link(node, root);

		for (auto it = node->next_vert.begin();
				it != node->next_vert.end(); ++it) {
			queue.push(it->second);
		}
	}

	queue.push(root);						// for up_suffix_links
	while (queue.size() != 0) {
		Node* node = queue.front();
		queue.pop();

		FindUpLink(node, root);

		for (auto it = node->next_vert.begin(); it != node->next_vert.end(); ++it) {
			queue.push(it->second);
		}
	}
}

int CountAndAddPatterns(std::string& pattern, Node* root) {
	std::string str("");
	int pattern_num = 1;

	if (pattern[0] != '?') {
		str += pattern[0];
	}

	int i = 1;
	while (i < pattern.length()) {

		if (pattern[i] == '?') {
			if (pattern[i - 1] != '?') {
				AddPattern(str, i - str.length(), root);
				str = "";
				++pattern_num;
			}
		} else {
			str += pattern[i];
		}

		++i;
	}
	if (pattern[i - 1] != '?') {
		AddPattern(str, i - str.length(), root);
	} else {
		AddPattern(str, i - 1, root);
	}

	return pattern_num;
}

void FindPatterns(const std::string& str, Node* root, const int& pattern_num) {

	std::vector<int> num_of_entries(str.length(), 0);			// to get the answer

	Node* current_node = root;
	for (int i = 0; i < str.length(); ++i) {
		current_node = FindLink(current_node, str[i], root);

		Node* suf_node = current_node;

		while (suf_node != root) {
			if (suf_node->is_leaf()) {
				for (int j = 0; j < suf_node->pattern_pos_len.size(); ++j) {
					int index = i - suf_node->pattern_pos_len[j].second
								- suf_node->pattern_pos_len[j].first + 1;
					if ( index >= 0 ) {
						++num_of_entries[index];
					}
					
				}
			}
			suf_node = FindUpLink(suf_node, root);
		}

		if (suf_node->is_leaf()) {								// if the stroke have '?' in the end
			for (int j = 0; j < suf_node->pattern_pos_len.size(); ++j) {

				int index = i  - suf_node->pattern_pos_len[j].second
							- suf_node->pattern_pos_len[j].first;
				if (index >= 0) {
					++num_of_entries[index];
				}
			}
		}
	}

	for (int i = 0; i < num_of_entries.size(); ++i) {			//check number of entries
		if (num_of_entries[i] == pattern_num) {
			std::cout << i << " ";
		}
	}
}

int main() {

	std::string pattern;
	std::string text;

	std::cin >> pattern;
	std::cin >> text;

	Node* root = new Node();

	int pattern_num = CountAndAddPatterns(pattern, root);

	FindPatterns(text, root, pattern_num);

	return 0;
}