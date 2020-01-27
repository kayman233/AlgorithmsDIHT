/*
	Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
	Хранимые строки непустые и состоят из строчных латинских букв. Начальный размер таблицы должен быть равным 8-ми.
	Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
	Хеш-функцию строки реализуйте с помощью вычисления значения многочлена методом Горнера.

	Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
	1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
	g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/

#include <iostream>
#include <vector>
#include <string>

class HashTable {
public:
	HashTable();

	bool AddString(const std::string& message);
	bool RemoveString(const std::string& message);
	bool Find(const std::string& message);

private:
	std::vector<std::pair<std::string, int>> hash_table;
	int table_size;
	int current_size;

	const int Size = 8;
	const int ResizePercentage = 75;
	const int PrimeNumber = 31;

	int HashStr(std::string message);
	bool isNil(int status);
	void Resize();
	void isFull();
};

HashTable::HashTable() {
	hash_table.reserve(Size);
	table_size = Size;

	for (int i = 0; i < table_size; ++i) {
		hash_table.push_back(std::make_pair("", 0));
	}
}

void HashTable::Resize() {
	table_size = table_size * 2;

	std::vector<std::pair<std::string, int>> new_hash_table;
	for (int i = 0; i < table_size; ++i) {
		new_hash_table.push_back(std::make_pair("", 0));
	}

	for (int j = 0; j < hash_table.size(); ++j) {
		if (hash_table[j].second == 1) {

			int hash = HashStr(hash_table[j].first);
			int probing = 0;
			int i = 0;

			while (new_hash_table[hash % table_size].second != 0) { // collision resolution
				++i;
				probing = (probing + i) % table_size;
				hash += probing;
			}

			new_hash_table[hash % table_size] = std::make_pair(hash_table[j].first, 1);

		}
	}

	hash_table = new_hash_table;
}

int HashTable::HashStr(const std::string message) {
	int hash = 0;
	for (int i = 0; i < message.size(); ++i) {
		hash = (hash * PrimeNumber + message[i]) % table_size;
	}
	return hash;
}

void HashTable::isFull() {
	if (current_size >= (table_size * ResizePercentage) / 100) {
		Resize();
	}
}

bool HashTable::AddString(const std::string& message) {

	if (Find(message)) {
		return false;
	} else {

		int hash = HashStr(message);
		int probing = 0;
		int i = 0;

		while (!isNil(hash_table[hash % table_size].second)) {
			++i;
			probing = (probing + i) % table_size;
			hash += probing;
		}

		hash_table[hash % table_size] = std::make_pair(message, 1);

		++current_size;
		isFull();

		return true;
	}
}

bool HashTable::RemoveString(const std::string& message) {
	if (!Find(message)) {
		return false;
	} else {

		int hash = HashStr(message);
		int probing = 0;
		int i = 0;

		while (!(hash_table[hash % table_size].first == message)) {
			++i;
			probing = (probing + i) % table_size;
			hash += probing;
		}

		hash_table[hash % table_size] = std::make_pair("", 2);

		return true;
	}
}

bool HashTable::Find(const std::string& message) {

	int hash = HashStr(message);
	int probing = 0;
	int i = 0;

	while (hash_table[hash % table_size].second != 0) {
		if (hash_table[hash % table_size].first == message) {
			break;
		}

		++i;
		probing = (probing + i) % table_size;
		hash += probing;
	}

	if (hash_table[hash % table_size].second == 0) {
		return false;
	}

	return true;
}

bool HashTable::isNil(int status) {
	if (status == 0 || status == 2) {
		return true;
	}
	return false;
}

bool is_okay(std::string& message, HashTable& table) {

	char command = message[0];
	message.erase(0, 1);

	switch (command) {
	case '+':
		if (table.AddString(message)) {
			return true;
		} else {
			return false;
		}
	case '-':
		if (table.RemoveString(message)) {
			return true;
		} else {
			return false;
		}
	case '?':
		if (table.Find(message)) {
			return true;
		} else {
			return false;
		}
	}
}

int main() {

	std::string message;

	HashTable table;

	while (std::getline(std::cin, message)) {

		if (is_okay(message, table)) {
			std::cout << "OK" << std::endl;
		} else {
			std::cout << "FAIL" << std::endl;
		}
	}

	return 0;
}