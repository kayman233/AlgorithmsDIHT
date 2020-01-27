/*
	A. Вторая статистика (RMQ)
	Дано число N и последовательность из N целых чисел.
	Найти вторую порядковую статистику на заданных диапазонах.

	Для решения задачи используйте структуру данных Sparse Table. 
	Требуемое время обработки каждого диапазона O(1). Время подготовки структуры данных O(nlogn)
*/

#include <iostream>
#include <vector>
#include <algorithm>

using std::pair;

class SparseTable {
public:
	SparseTable(std::vector<int>& array);
	pair<int, int> getMin(pair<int, int> a, pair<int, int> b);
	void InitLog(int size);
	int get2ndStat(int l, int r);
private:
	std::vector < std::vector<pair<int, int>> > table;
	std::vector<int> log_2;
};

SparseTable::SparseTable(std::vector<int>& array) {

	int size = array.size();

	InitLog(size + 1);

	int table_size = log_2[size] + 1;
	table.resize(table_size);

	for (int i = 0; i < table_size; ++i) {
		table[i].resize(size, std::make_pair(0, 0));
	}

	for (int i = 0; i < size; ++i) {
		table[0][i] = std::make_pair(array[i], array[i]);
	}

	for (int i = 1; i < table_size; ++i) {
		int row_size = size - (1 << i) + 1;
		for (int j = 0; j < row_size; j++) {

			int gap = 1 << (i - 1);
			pair<int, int> first = table[i - 1][j];
			pair<int, int> second = table[i - 1][j + gap];

			pair<int, int> min_pair = getMin(first, second);
			table[i][j] = min_pair;
		}
	}
}

void SparseTable::InitLog(int size) {
	int log_value = 0;

	log_2.assign(size, 0);
	for (int i = 0; i < size; ++i) {

		if (i >= 1 << (log_value + 1)) {
			++log_value;
		}

		log_2[i] = log_value;
	}
}

pair<int, int> SparseTable::getMin(pair<int, int> a, pair<int, int> b) {
	pair<int, int> min_pair;

	std::vector<int> values;
	values.push_back(a.first);
	values.push_back(a.second);
	values.push_back(b.first);
	values.push_back(b.second);

	std::sort(values.begin(), values.end());

	min_pair.first = values[0];

	for (int i = 1; i < 4; ++i) {
		if (values[0] != values[i]) {
			min_pair.second = values[i];
			break;
		}
	}

	return min_pair;
}

int SparseTable::get2ndStat(int l, int r) {
	int i = log_2[r - l + 1];
	int gap = 1 << i;
	pair<int, int> min_pair = getMin(table[i][l], table[i][r - gap + 1]);
	int answer = min_pair.second;

	return answer;
}

int main() {

	int n = 0;
	int m = 0;

	std::cin >> n >> m;

	std::vector<int> array(n);

	for (int i = 0; i < n; ++i) {
		int element;
		std::cin >> element;

		array[i] = element;
	}

	SparseTable sparse_table(array);

	for (int i = 0; i < m; ++i) {
		int l = 0;
		int r = 0;
		std::cin >> l >> r;
		int answer = sparse_table.get2ndStat(l - 1, r - 1);

		std::cout << answer << std::endl;
	}

	return 0;
}