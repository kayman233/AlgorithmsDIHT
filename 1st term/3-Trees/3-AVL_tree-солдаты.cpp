/*
	3_1 Солдаты

	В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая,
	то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
	Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие,
	а в конце – самые низкие. За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
	Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
	а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится. Требуемая скорость выполнения команды - O(log n).
*/

#include <iostream>
#include <algorithm>

class AvlTree {
private:
	struct Node {
		int key;
		unsigned char height;
		int count;
		Node* left;
		Node* right;

		Node(int key);
	};

	Node* root;
	unsigned char max_height;

	Node* InsertAndPrint(Node* p, int key, int position); //вставка узла и вывод позиции
	Node* remove(Node* p, int pos); //удаление по позиции

	Node* findMin(Node* p); //поиск узла с минимальным ключом для поддерева
	Node* removeMin(Node* p); //удаление узла с минимальным ключом из поддерева

	Node* balance(Node* p); //балансировка дерева
	Node* rotateRight(Node* p); //правый малый поворот узла
	Node* rotateLeft(Node* q); //левый малый поворот узла
	Node* find(Node* p, int key); //поиск узла

	unsigned char height(Node* p); //определение высоты узла
	int bFactor(Node* p); //разница высот
	void UpdateHeightAndCount(Node* p); //восстановление высоты и номера
	int count(Node* p); //определение номера
	int FindOrderStatistic(Node* p, int k);

	void DeleteSubTree(Node* p);
public:
	AvlTree();
	~AvlTree();

	bool InsertAndPrint(int key); //вставка и вывод позиции
	void Remove(int pos); //удаление по позиции в дереве

	int FindOrderStatistic(int k);
};

AvlTree::Node::Node(int _key)
	: key(_key),
	height(1),
	count(1),
	left(nullptr),
	right(nullptr)
{}

AvlTree::AvlTree()
	:root(nullptr),
	max_height(0)
{}

AvlTree::~AvlTree() {
	DeleteSubTree(root);
}

void AvlTree::DeleteSubTree(Node* p) {
	if (p != nullptr) {
		DeleteSubTree(p->left);
		DeleteSubTree(p->right);
		delete p;
	}
}

//==============================
//         Insertion
//==============================
bool AvlTree::InsertAndPrint(int key) {
	if (find(root, key)) {
		return false;
	}
	int position = 0;
	if (root != nullptr) {
		position = 1;
		if (root->right != nullptr) {
			position += (root->right->count); //если правое поддерево непустое, то увеличим позицию
		}
	}
	else {
		std::cout << position << std::endl; //если дерево пустое
	}
	root = InsertAndPrint(root, key, position); //будем передавать еще и позицию
	return true;
}

AvlTree::Node* AvlTree::InsertAndPrint(Node* p, int key, int position) {

	if (p == nullptr) {
		return new Node(key);
	}

	if (key < p->key) {

		if (p->left != nullptr) { //если идем налево, то в любом случае увеличиваем позицию на 1, а также смотрим кол-во элементов в правом поддереве
			++position;
			if (p->left->right != nullptr) {
				position += (p->left->right->count);
			}
		}
		else {
			std::cout << position << std::endl; //заранее смотрим, будет ли добавляться элемент, выводим позицию родителя
		}

		p->left = InsertAndPrint(p->left, key, position);

	}
	else {

		if (p->right != nullptr) { //аналогично
			--position;
			if (p->right->left != nullptr) {
				position = position - p->right->left->count;
			}
		}
		else {
			std::cout << position - 1 << std::endl; //выводим position-1, так как нужно указать позицию родителя
		}

		p->right = InsertAndPrint(p->right, key, position);

	}
	return balance(p);
}

//==============================
//         Removing
//==============================
void AvlTree::Remove(int pos) {
	int key = FindOrderStatistic(root->count - pos - 1); //определяем ключ элемента в этой позиции
	root = remove(root, key);
}

AvlTree::Node* AvlTree::findMin(Node* p) {
	return p->left ? findMin(p->left) : p;
}

AvlTree::Node* AvlTree::removeMin(Node* p) {
	if (p->left == 0) {
		return p->right;
	}
	p->left = removeMin(p->left);
	return balance(p);
}

AvlTree::Node* AvlTree::remove(Node* p, int key) {
	if (p == nullptr) {
		return nullptr;
	}
	if (key < p->key) {
		p->left = remove(p->left, key);
	}
	else {
		if (key > p->key) {
			p->right = remove(p->right, key);
		}
		else {
			Node* q = p->left;
			Node* r = p->right;
			delete p;
			if (r == nullptr)
				return q;
			Node* min = findMin(r);
			min->right = removeMin(r);
			min->left = q;
			return balance(min);
		}
	}
	return balance(p);
}

//==============================
//         Balancing
//==============================
AvlTree::Node* AvlTree::balance(Node* p) {
	UpdateHeightAndCount(p);
	if (bFactor(p) == 2) {
		if (bFactor(p->right) < 0) {
			p->right = rotateRight(p->right);
		}
		return rotateLeft(p);
	}
	if (bFactor(p) == -2) {
		if (bFactor(p->left) > 0) {
			p->left = rotateLeft(p->left);
		}
		return rotateRight(p);
	}
	return p;
}

AvlTree::Node* AvlTree::rotateRight(Node* p) {
	Node* q = p->left;
	p->left = q->right;
	q->right = p;
	UpdateHeightAndCount(p);
	UpdateHeightAndCount(q);
	return q;
}
AvlTree::Node* AvlTree::rotateLeft(Node* q) {
	Node* p = q->right;
	q->right = p->left;
	p->left = q;
	UpdateHeightAndCount(q);
	UpdateHeightAndCount(p);
	return p;
}

unsigned char AvlTree::height(Node* p) {
	return p ? p->height : 0;
}

int AvlTree::count(Node* p) {
	return p ? p->count : 0;
}

int AvlTree::bFactor(Node* p) {
	return height(p->right) - height(p->left);
}

void AvlTree::UpdateHeightAndCount(Node* p) {
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;

	p->count = count(p->left) + count(p->right) + 1;
}

AvlTree::Node* AvlTree::find(Node* p, int key) {
	if (p == nullptr) {
		return nullptr;
	}
	if (p->key == key) {
		return p;
	}
	if (key < p->key) {
		return find(p->left, key);
	}
	return find(p->right, key);
}

//==============================
//       Order Statistic
//==============================
int AvlTree::FindOrderStatistic(int k) {
	if (root == nullptr) {
		return -1;
	}
	if (k < 0 || k > root->count - 1) {
		return -1;
	}

	return FindOrderStatistic(root, k);
}

int AvlTree::FindOrderStatistic(Node* p, int k)
{
	int left_count = count(p->left);
	if (k == left_count) {
		return p->key;
	}

	if (k < left_count) {
		return FindOrderStatistic(p->left, k);
	}
	return FindOrderStatistic(p->right, k - left_count - 1);
}

int main() {
	AvlTree rank;

	int n = 0;
	std::cin >> n;

	for (int i = 0; i < n; ++i) {

		int command = 0;
		int height = 0;
		std::cin >> command >> height;

		if (command == 1) {
			rank.InsertAndPrint(height);
		}
		else {
			rank.Remove(height);
		}
	}

	return 0;
}