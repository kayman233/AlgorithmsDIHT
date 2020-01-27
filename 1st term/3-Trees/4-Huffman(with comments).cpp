#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "Huffman.h"
#include <algorithm>
#include <fstream>

typedef unsigned char byte;

struct Node { //структура для узла в дереве
	byte symbol;
	int frequency;

	Node* Left;
	Node* Right;

	Node() : Left(nullptr), Right(nullptr), symbol(0), frequency(0) {}

	Node(byte _sym) : symbol(_sym), Left(nullptr), Right(nullptr) {}

	Node(Node* _left, Node* _right) :
		Left(_left),
		Right(_right),
		frequency(_left->frequency + _right->frequency) {}
};

class HuffmanTree { //структура для создания дерева Хаффмана и создания таблицы символов и их кодов
private:
	Node* root;
	std::vector<bool> code; //вспомогательный вектор для определения кода для символа
	struct Comparison {
		bool operator()(const Node* Left, const Node* Right) const { return Left->frequency > Right->frequency; }
	};

public:
	
	std::map<byte, std::vector<bool> > table; //map, в котором хранится ключ-символ и значение в виде последовательности(зашифрованный код)

	void Build_Huffman_Tree(std::map<byte, int>& counts); //метод для построики дерева, ему передается map, в котором хранятся символы и их частота в файлу
	Node* Get_Root();

	void BuildTable(Node* root); //метод для создания таблицы
};

Node* HuffmanTree::Get_Root() {
	return root;
}

void HuffmanTree::BuildTable(Node* root) {
	if (root->Left != nullptr) {
		code.push_back(0);
		BuildTable(root->Left);
	}

	if (root->Right != nullptr) {
		code.push_back(1);
		BuildTable(root->Right);
	}

	if (root->Left == nullptr && root->Right == nullptr) {
		table[root->symbol] = code;
	}

	if (code.size() > 0) {
		code.pop_back();
	}
}

void HuffmanTree::Build_Huffman_Tree(std::map<byte, int>& counts) {
	//запись очереди с приоритетом
	std::priority_queue<Node*, std::vector<Node*>, Comparison> nodes_queue;

	for (auto itr = counts.begin(); itr != counts.end(); ++itr) {
		Node* new_node = new Node;
		new_node->symbol = itr->first;
		new_node->frequency = itr->second;
		nodes_queue.push(new_node);
	}

	//вывод
	/*while (!nodes_queue.empty()) {
		std::cout << nodes_queue.top()->symbol << " " << nodes_queue.top()->frequency << std::endl;
		nodes_queue.pop();
	}*/

	//построение дерева

	if (nodes_queue.size() == 1) {
		root = nodes_queue.top();
	}

	while (nodes_queue.size() != 1) {

		Node* Left = nodes_queue.top();
		nodes_queue.pop();

		Node* Right = nodes_queue.top();
		nodes_queue.pop();

		Node* parent = new Node(Left, Right);
		nodes_queue.push(parent);

	}

	root = nodes_queue.top();
};

class BitArray { //класс для создания закодированного дерева 
private:
	void write_bits(byte sym, bool is_char); // запись битов
public:
	std::vector<bool> buffer; // буффер, где хранится закодированное дерево
	void Build_encoded_tree(Node* root); //построение закодированного дерева
};

void BitArray::Build_encoded_tree(Node* root) {
	if (root->Left == nullptr) {
		write_bits(0, false);
		write_bits(root->symbol, true);
	} else {
		write_bits(1, false);
		Build_encoded_tree(root->Left);
		Build_encoded_tree(root->Right);
	}
}

void BitArray::write_bits(byte sym, bool is_char) {
	if (!is_char) {
		if (sym) {
			buffer.push_back(1);
		}
		else {
			buffer.push_back(0);
		}
	}
	else {
		for (int i = 7; i >= 0; --i) {
			buffer.push_back((sym >> i) & 1);
		}
	}
}

class Encoded_Array { //класс для раскодировки файла
private:
	std::vector<byte> encoded_file; //буффер, в котором хранится передаваемый "файл"
	int size_of_tree; //сколько бит занимает в файле дерево
	int bit_pos; //вспомогательная 
	int excessive_bits; //кол-во бит, которые не нужно использовать для раскодировки
	int stream_size; //кол-во бит для считывания, исключая первый и последний байт: в первом байте хранится размер дерева(кол-во листьев), в последнем сколько бит, нужно не учитывать
	byte Read_Byte(); //метод для считывания 8 бит( 1 байта ) и передачи в дерево
	void make_Tree(Node* root);

public:
	Encoded_Array(std::vector<byte>& _encoded_file) :
		encoded_file(_encoded_file),
		size_of_tree(10 * (_encoded_file[0]) - 1),
		bit_pos(0),
		excessive_bits(_encoded_file[_encoded_file.size() - 1]),
		stream_size((_encoded_file.size() - 2) * 8 - _encoded_file[_encoded_file.size() - 1])
	{}
	std::vector<bool> encoded_tree; //вектор, в котором хранится закодированное дерево
	std::vector<bool> encoded_message; //вектор, в котором хранится закодированное сообщение

	void Read_tree_and_message(); //метод для считывания дерева и сообщение
	Node* Make_Tree(); //создать дерево для раскодировки
};

void Encoded_Array::Read_tree_and_message() {
	for (auto itr = encoded_file.begin() + 1; itr != encoded_file.end() - 1; ++itr) {
		byte c = *itr;
		if (bit_pos < size_of_tree) { //считываем закодированное дерево 
			int i = 0;
			for (; i < 8 && bit_pos < size_of_tree; ++i, ++bit_pos) {
				encoded_tree.push_back((c >> (7 - i)) & 1);
			}

			if (bit_pos == size_of_tree) { //когда дошли до считывания сообщения
				for (int j = i; j < 8 && bit_pos < stream_size; ++j, ++bit_pos) {
					encoded_message.push_back((c >> (7 - j)) & 1);
				}
			}
		} else {
			if (bit_pos < stream_size) {
				for (int i = 0; i < 8 && bit_pos < stream_size; ++i, ++bit_pos) {
					encoded_message.push_back((c >> (7 - i)) & 1);
				}
			}
		}
	}
}

byte Encoded_Array::Read_Byte() {
	byte sym = 0;
	for (int pos = 7; pos >= 0; --pos) {
		sym |= (encoded_tree.back() << pos);
		encoded_tree.pop_back();
	}
	return sym;
}

Node* Encoded_Array::Make_Tree() {
	std::reverse(encoded_tree.begin(), encoded_tree.end());
	Node* node = new Node(0);

	if (encoded_tree.back() == 0) {
		encoded_tree.pop_back();
		node->symbol = Read_Byte();
	}
	else {
		encoded_tree.pop_back();
		node->Left = new Node();
		node->Right = new Node();

		make_Tree(node->Left);
		make_Tree(node->Right);
	}
	return node;
}

void Encoded_Array::make_Tree(Node* node) {
	if (encoded_tree.back() == 0) {
		encoded_tree.pop_back();
		node->symbol = Read_Byte();
		return;
	}
	else {
		encoded_tree.pop_back();
		node->Left = new Node();
		node->Right = new Node();

		make_Tree(node->Left);
		make_Tree(node->Right);
	}
}

struct MyCInputStream {
	std::vector<byte> buffer;
	int pos = 0;
};

struct MyCOutputStream {
	std::vector<byte> buffer;
};

void _Encode(MyCInputStream& original, MyCOutputStream& compressed, std::map<byte, int>& counts) {

	//строим дерево и таблицу кодов
	HuffmanTree Huffman_Tree;
	Huffman_Tree.Build_Huffman_Tree(counts);
	Huffman_Tree.BuildTable(Huffman_Tree.Get_Root());

	//запись в первый байт размер дерева
	byte size_of_tree = Huffman_Tree.table.size();
	compressed.buffer.push_back(size_of_tree);

	//кодируем дерево и добавляем в файл
	BitArray Encoded_Tree;
	Encoded_Tree.Build_encoded_tree(Huffman_Tree.Get_Root());

	if (Huffman_Tree.table.size() == 1) {
		std::vector<bool> null;
		null.push_back(0);
		Huffman_Tree.table[original.buffer[0]] = null;
	}
	int bit_pos = 7;
	byte sym = 0;
	for (auto it = Encoded_Tree.buffer.begin(); it != Encoded_Tree.buffer.end(); ++it) {
		sym |= (*it << bit_pos);

		if (bit_pos == 0) {
			compressed.buffer.push_back(sym);
			sym = 0;
			bit_pos = 7;
		} else {
			--bit_pos;
		}
	}
	//кодируем сообщение и добавляем в файл
	for (auto it = original.buffer.begin(); it != original.buffer.end(); ++it) {
		std::vector<bool> sequence = Huffman_Tree.table[*it];
		for (auto itr = sequence.begin(); itr != sequence.end(); ++itr) {
			sym |= (*itr << bit_pos);

			if (bit_pos == 0) {
				compressed.buffer.push_back(sym);
				sym = 0;
				bit_pos = 7;
			} else {
				--bit_pos;
			}
		}
	}

	if (bit_pos != 7) {
		compressed.buffer.push_back(sym);
	}

	//запись в последний бит кол-во лишних бит
	byte excessive = (bit_pos + 1) % 8;
	compressed.buffer.push_back(excessive);

}

// функция для записи в выходной поток расшифрованного сообщения
void Write_Stream(std::vector<bool> compressed, std::vector<byte>& buffer, Node* Decoding_tree) {
	Node* root = Decoding_tree;
	for (auto it = compressed.begin(); it != compressed.end(); ++it) {

		if (root->Left == nullptr && root->Left == nullptr) {
			buffer.push_back(root->symbol);
			root = Decoding_tree;
		}
		if (*it == 1) {
			root = root->Right;
		} else {
			root = root->Left;
		}
	}
	buffer.push_back(root->symbol);
}

void _Decode(MyCInputStream& compressed, MyCOutputStream& original) {

	//считываем входной файл
	Encoded_Array encoded(compressed.buffer);
	encoded.Read_tree_and_message();

	//строим дерево
	Node* Decoding_tree = encoded.Make_Tree();
	//записываем в выходной файл
	std::vector<byte> buffer;
	Write_Stream(encoded.encoded_message, buffer, Decoding_tree);
	for (auto it = buffer.begin(); it != buffer.end(); ++it) {
		byte c = *it;
		compressed.buffer.push_back(c);
		std::cout << c;
	}
}

//для контеста
void Encode(IInputStream& original, IOutputStream& compressed) {
	byte value;

	std::vector<byte> input;
	std::vector<byte> output;

	std::map<byte, int> counts;
	while (original.Read(value)) {
		++counts[value];
		input.push_back(value);
	}

	HuffmanTree Huffman_Tree;
	Huffman_Tree.Build_Huffman_Tree(counts);
	Huffman_Tree.BuildTable(Huffman_Tree.Get_Root());

	if (Huffman_Tree.table.size() == 1) {
		std::vector<bool> null;
		null.push_back(0);
		Huffman_Tree.table[input[0]] = null;
	}

	//запись в первый байт размер дерева
	//byte size_of_tree = Huffman_Tree.table.size();
	output.push_back(Huffman_Tree.table.size());

	//кодируем дерево и добавляем в файл
	BitArray Encoded_Tree;
	Encoded_Tree.Build_encoded_tree(Huffman_Tree.Get_Root());

	int bit_pos = 7;
	byte sym = 0;
	for (auto it = Encoded_Tree.buffer.begin(); it != Encoded_Tree.buffer.end(); ++it) {
		sym |= (*it << bit_pos);

		if (bit_pos == 0) {
			output.push_back(sym);
			sym = 0;
			bit_pos = 7;
		}
		else {
			--bit_pos;
		}
	}
	//кодируем сообщение и добавляем в файл
	for (auto it = input.begin(); it != input.end(); ++it) {
		std::vector<bool> sequence = Huffman_Tree.table[*it];
		for (auto itr = sequence.begin(); itr != sequence.end(); ++itr) {
			sym |= (*itr << bit_pos);

			if (bit_pos == 0) {
				output.push_back(sym);
				sym = 0;
				bit_pos = 7;
			}
			else {
				--bit_pos;
			}
		}
	}

	if (bit_pos != 7) {
		output.push_back(sym);
	}

	//запись в последний бит кол-во лишних бит
	byte excessive = (bit_pos + 1) % 8;
	output.push_back(excessive);

	for (auto it = output.begin(); it != output.end(); ++it) {
		byte c = *it;
		compressed.Write(c);
	}

}

void Decode(IInputStream& compressed, IOutputStream& original) {
	byte value;

	std::vector<byte> input;
	std::vector<byte> output;
	while (compressed.Read(value)) {
		input.push_back(value);
	}

	Encoded_Array encoded(input);
	encoded.Read_tree_and_message();

	//строим дерево
	Node* Decoding_tree = encoded.Make_Tree();
	//записываем в выходной файл
	Write_Stream(encoded.encoded_message, output, Decoding_tree);

	for (auto it = output.begin(); it != output.end(); ++it) {
		byte c = *it;
		original.Write(c);
	}
}