/*
	Задача 4 а). «Пятнашки»

	Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
	[ 1  2  3  4 ]
	[ 5  6  7  8 ]
	[ 9  10 11 12]
	[ 13 14 15 0 ]
	где 0 задает пустую ячейку.
	Достаточно найти хотя бы какое-то решение. 
	Число перемещений костяшек не обязано быть минимальным.
*/


#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

class Square {
public:
	unsigned long long square; //как и в восьминашках
	int zero_num;
	Square* parent;
	char move;

	Square(std::vector<int>& array);
	Square(unsigned long long num);

	Square& squareUp();
	Square& squareDown();
	Square& squareLeft();
	Square& squareRight();

	int CountInvariant();

	int ManhattanDistance(int place, int number);
	int CalcHeurusic();
	int GetDigit(int pos);
	int GetRow(int pos);

	void GetNext(std::vector<Square*>& tmp);

	Square& operator=(Square& a) {
		this->square = a.square;
		this->zero_num = a.zero_num;
		this->move = a.move;
		this->parent = a.parent;

		return *this;
	}
};

Square::Square(std::vector<int>& arr) {
	square = arr[0];
	square <<= 60;

	zero_num = 0;

	if (arr[0] == 0) {
		zero_num = 1;
	}

	unsigned long long tmp = 0;

	for (int i = 1; i < 16; ++i) {
		tmp = arr[i];
		tmp <<= (64 - ((i + 1) * 4));
		square += tmp;

		if (arr[i] == 0) {
			zero_num = i + 1;
		}
	}

	parent = nullptr;
}

Square::Square(unsigned long long num) {
	zero_num = 1;
	if (num == 0) {
		square = 0;
		zero_num = 0;
	} else {
		square = num;

		while ((num >> 60) % 16 != 0) {
			++zero_num;
			num <<= 4;
		}
	}

	parent = nullptr;
}

Square& Square::squareUp() {
	if (zero_num < 5) {
		Square* up_square = new Square(0);
		return *up_square;
	} else {
		unsigned long long num = square;
		unsigned long long up = square;

		for (int i = 0; i < 16 - (zero_num - 4); ++i) {
			num >>= 4;
		}

		num = num % 16;
		up = up - (num << 4 * (16 - (zero_num - 4)));
		up = up + (num << 4 * (16 - zero_num));
		Square* up_square = new Square(up);

		return *up_square;
	}
}

Square& Square::squareDown() {
	if (zero_num > 12) {
		Square* down_square = new Square(0);
		return *down_square;
	} else {
		unsigned long long num = square;
		unsigned long long down = square;

		for (int i = 0; i < 16 - (zero_num + 4); ++i) {
			num >>= 4;
		}

		num = num % 16;

		down = down - (num << 4 * (16 - (zero_num + 4)));

		down = down + (num << 4 * (16 - zero_num));

		Square* down_square = new Square(down);

		return *down_square;
	}
}

Square& Square::squareRight() {
	if (zero_num % 4 == 0) {
		Square* right_square = new Square(0);
		return *right_square;
	} else {
		unsigned long long num = square;
		unsigned long long right = square;

		for (int i = 0; i < 16 - (zero_num + 1); ++i) {
			num >>= 4;
		}

		num = num % 16;

		right = right - (num << 4 * (16 - (zero_num + 1)));
		right = right + (num << 4 * (16 - zero_num));

		Square* right_square = new Square(right);

		return *right_square;
	}
}

Square& Square::squareLeft() {
	if (zero_num % 4 == 1) {
		Square* left_square = new Square(0);
		return *left_square;
	} else {
		unsigned long long num = square;
		unsigned long long left = square;

		for (int i = 0; i < 16 - (zero_num - 1); ++i) {
			num >>= 4;
		}

		num = num % 16;

		left = left - (num << 4 * (16 - (zero_num - 1)));
		left = left + (num << 4 * (16 - zero_num));
		Square* left_square = new Square(left);

		return *left_square;
	}
}

int Square::CountInvariant() {
	int counter = 0;

	for (int i = 0; i < 16; ++i) {
		int digit1 = (square >> 60 - i * 4) % 16;
		if (digit1 < 0) {
			digit1 += 16;
		}

		for (int j = i + 1; j < 16; ++j) {
			int digit2 = (square >> 60 - j * 4) % 16;
			if (digit2 < 0) {
				digit2 += 16;
			}
			if (digit1 > digit2 && digit1 != 0 && digit2 != 0) {
				++counter;
			}

		}

	}

	return counter + ((zero_num - 1) / 4) + 1;
}

int Square::ManhattanDistance(int place, int number) {
	int dist = 0;

	if (number == 0) {
		number = 16;
	}

	if (place == number) {
		return 0;
	}

	dist = abs(((number - 1) / 4) - ((place - 1) / 4)) + abs(((number - 1) % 4) - ((place - 1) % 4));

	return dist;
}

int Square::CalcHeurusic() {
	int sum = 0;
	unsigned long long num = square;

	// Манхэтоновское расстояние
	for (int i = 0; i < 16; ++i) {
		int digit = GetDigit(i);
		sum += ManhattanDistance(i + 1, digit);
	}

	// Линейный конфликт
	std::set<int> visited;
	std::vector<int> rows(16, 0);
	for (int i = 0; i < 16; ++i) {
		int j = i + 1;
		int digit1 = GetDigit(i);
		if (GetRow(digit1 - 1) == GetRow(i)) {
			while (j % 4 != 0) {
				int digit2 = GetDigit(j);
				if (GetRow(digit2 - 1) == GetRow(i) && digit1 > digit2) {
					rows[i] = rows[i] + 1;
					visited.insert(digit1);
					visited.insert(digit2);
				}
				++j;
			}
		}

		if (i % 4 == 3) {
			bool is_null = true;
			for (j = i - 3; j < i + 1; ++j) {
				if (rows[j] > 0) {
					is_null = false;
				}
			}
			while (!is_null) {
				int max = rows[i - 3];
				int max_i = i - 3;
				for (j = i - 2; j < i + 1; ++j) {
					if (rows[j] > max) {
						max = rows[j];
						max_i = j;
					}
				}

				rows[max_i] = 0;
				for (j = i - 3; j < i + 1; ++j) {
					if (j > max_i && GetDigit(max_i) > GetDigit(j)) {
						rows[j] = rows[j] - 1;
					}
				}

				sum += 2;

				is_null = true;
				for (j = i - 3; j < i + 1; ++j) {
					if (rows[j] > 0) {
						is_null = false;
					}
				}
			}
		}
	}

	// Угловой конфликт
	int digit3 = GetDigit(2);
	int digit4 = GetDigit(3);
	int digit8 = GetDigit(7);

	int digit2 = GetDigit(1);
	int digit1 = GetDigit(0);
	int digit5 = GetDigit(4);

	int digit9 = GetDigit(8);
	int digit13 = GetDigit(12);
	int digit14 = GetDigit(13);

	if (visited.find(digit3) != visited.end() || visited.find(digit8) != visited.end()) {
		if (digit3 == 3 && digit8 == 8 && digit4 != 4) {
			sum += 2;
		}
	}

	if (visited.find(digit2) != visited.end() || visited.find(digit5) != visited.end()) {
		if (digit2 == 2 && digit5 == 5 && digit1 != 1) {
			sum += 2;
		}
	}

	if (visited.find(digit9) != visited.end() || visited.find(digit14) != visited.end()) {
		if (digit9 == 9 && digit14 == 14 && digit13 != 13) {
			sum += 2;
		}
	}

	return sum;
}

int Square::GetDigit(int pos) {
	int a = (square >> 60 - pos * 4) % 16;
	if (a < 0) a += 16;
	return a;
}

int Square::GetRow(int pos) {
	return (pos / 4) + 1;
}

void Square::GetNext(std::vector<Square*>& tmp) {
	tmp.resize(4);
	tmp[0] = &squareDown();
	tmp[0]->move = 'U';
	tmp[1] = &squareUp();
	tmp[1]->move = 'D';
	tmp[2] = &squareRight();
	tmp[2]->move = 'L';
	tmp[3] = &squareLeft();
	tmp[3]->move = 'R';
}


struct Comparator {
	bool operator() (const std::pair<int, Square*>& lhs,
		const std::pair<int, Square*>& rhs) const {
		if (lhs.first == rhs.first) {
			return lhs.second->square < rhs.second->square;
		}
		else {
			return lhs.first < rhs.first;
		}
	}
};

//Алгоритм А*, но эвристика без подсчета номера шага
void Solve(Square& square, std::string& solution) {

	std::set<std::pair<int, Square*>, Comparator> sq_set;

	int heur = square.CalcHeurusic();

	if (heur == 0) {
		return;
	}

	sq_set.insert(std::make_pair(heur, &square));

	std::set<unsigned long long> visited;

	while (sq_set.size() > 0) {
		std::pair<int, Square*> min = *(sq_set.begin());

		visited.insert(min.second->square);

		sq_set.erase(sq_set.begin());

		Square* vertex = min.second;

		std::vector<Square*> tmp;

		vertex->GetNext(tmp);

		for (int i = 0; i < tmp.size(); ++i) {
			if (tmp[i]->square != 0 && tmp[i]->square != vertex->square) {
				if (visited.find(tmp[i]->square) == visited.end()) {
					int heuristic = 0;

					tmp[i]->parent = vertex;

					heuristic = tmp[i]->CalcHeurusic();

					if (tmp[i]->square == 1311768467463790320) { //нужная конфигурация
						while (tmp[i]->parent != nullptr) {
							solution += tmp[i]->move;
							tmp[i] = tmp[i]->parent;
						}
						return;
					}

					sq_set.insert(std::make_pair(heuristic, tmp[i]));
				}

			}
		}
	}

	return;
}

int main() {

	std::vector<int> arr(17);

	for (int i = 0; i < 16; ++i) {
		int d = 0;
		std::cin >> d;
		arr[i] = d;
	}

	Square square(arr);

	if (square.CountInvariant() % 2 != 0) {
		std::cout << -1 << std::endl;
	} else {
		std::string solution;
		Solve(square, solution);
		std::cout << solution.size() << std::endl;
		int size = solution.size();
		for (int i = 0; i < size; ++i) {
			std::cout << solution[size - i - 1];
		}
	}

	return 0;
}