/*
	Задача 3. Восьминашки

	«Восьминашки» – упрощенный вариант известной головоломки «Пятнашки».
	Восемь костяшек, пронумерованных от 1 до 8, расставлены по ячейкам игровой доски 3 на 3,
	одна ячейка при этом остается пустой. За один ход разрешается передвинуть одну из костяшек,
	расположенных рядом с пустой ячейкой, на свободное место.
	Цель игры – для заданной начальной конфигурации игровой доски за 
	минимальное число ходов получить выигрышную конфигурацию (пустая ячейка обозначена нулем):
	1 2 3
	4 5 6
	7 8 0

*/

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

// Класс для конфигурации
class Square {
public:
	int square; // храним в int
	int zero_num; // номер позиции нуля
	Square* parent; // храним родителя, чтобы найти решение
	char move; // также храним сам переход
	int count; // и кол-во шагов, чтобы перейти в данную конфигурацию

	Square(std::vector<int>& array);
	Square(int num);

	// изменение конфигурации
	Square& squareUp();
	Square& squareDown();
	Square& squareLeft();
	Square& squareRight();

	// Для подсчета эвристики
	int CountInvariant();
	int ManhattanDistance(int place, int number);
	int CalcHeurusic();

	void GetNext(std::vector<Square*>& tmp);
	void print();

	Square& operator=(Square& a) {
		this->square = a.square;
		this->zero_num = a.zero_num;
		this->move = a.move;
		this->parent = a.parent;

		return *this;
	}
};

Square::Square(std::vector<int>& array) {
	square = 1;
	zero_num = 0;
	for (int i = 0; i < array.size() - 1; ++i) {
		square = square * 10 + array[9 - i];
		if (array[9 - i] == 0) {
			zero_num = 8 - i;
		}
	}

	parent = nullptr;
}

Square::Square(int num) {
	zero_num = 0;
	if (num == 0) {
		square = 0;
		zero_num = 0;
	} else {
		square = num;
		while (num % 10 != 0) {
			++zero_num;
			num /= 10;
		}
	}

	parent = nullptr;
}

Square& Square::squareUp() {
	if (zero_num > 5) {
		Square* up_square = new Square(0);
		return *up_square;
	} else {
		int num = square;
		int up = square;
		int dec = 1;
		for (int i = 0; i < zero_num + 3; ++i) {
			num /= 10;
			dec *= 10;
		}

		num = num % 10;
		up = up - dec * num;

		for (int i = 0; i < 3; ++i) {
			dec /= 10;
		}

		up = up + dec * num;
		Square* up_square = new Square(up);

		return *up_square;
	}
}

Square& Square::squareDown() {
	if (zero_num < 3) {
		Square* down_square = new Square(0);
		return *down_square;
	} else {
		int num = square;
		int down = square;
		int dec = 1;
		for (int i = 0; i < zero_num - 3; ++i) {
			num /= 10;
			dec *= 10;
		}

		num = num % 10;
		down = down - dec * num;

		for (int i = 0; i < 3; ++i) {
			dec *= 10;
		}

		down = down + dec * num;
		Square* up_square = new Square(down);

		return *up_square;
	}
}

Square& Square::squareLeft() {
	if (zero_num % 3 == 2) {
		Square* left_square = new Square(0);
		return *left_square;
	} else {
		int num = square;
		int left = square;
		int dec = 1;
		for (int i = 0; i < zero_num + 1; ++i) {
			num /= 10;
			dec *= 10;
		}

		num = num % 10;
		left = left - dec * num;
		left = left + dec * num / 10;
		Square* left_square = new Square(left);

		return *left_square;
	}
}

Square& Square::squareRight() {
	if (zero_num % 3 == 0) {
		Square* right_square = new Square(0);
		return *right_square;
	} else {
		int num = square;
		int right = square;
		int dec = 1;
		for (int i = 0; i < zero_num - 1; ++i) {
			num /= 10;
			dec *= 10;
		}

		num = num % 10;
		right = right - dec * num;
		right = right + dec * num * 10;
		Square* right_square = new Square(right);

		return *right_square;
	}
}

void Square::print() {
	if (square == 0) {
		std::cout << 0 << std::endl;
		return;
	}

	std::cout << zero_num << std::endl;
	int num = square;
	int place = 100000000;

	for (int i = 0; i < 9; ++i) {
		std::cout << (num / place) % 10 << " ";
		place /= 10;
		if (i % 3 == 2) {
			std::cout << std::endl;
		}
	}

	std::cout << std::endl;
}

int Square::CountInvariant() {
	int counter = 0;
	int dec = 1;
	for (int i = 0; i < 9; ++i) {
		int num = square;
		int digit1 = (square / dec) % 10;
		int decrease = dec * 10;
		for (int j = i + 1; j < 9; ++j) {
			int digit2 = (square / decrease) % 10;
			if (digit1 < digit2 && digit1 != 0 && digit2 != 0) {
				++counter;
			}
			decrease *= 10;
		}
		dec *= 10;
	}

	return counter;
}

int Square::ManhattanDistance(int place, int number) {
	int dist = 0;

	if (number == 0) {
		number = 9;
	}

	if (place == number) {
		return 0;
	}

	dist = abs(((number - 1) / 3) - ((place - 1) / 3)) + abs(((number - 1) % 3) - ((place - 1) % 3));

	return dist;
}

int Square::CalcHeurusic() {
	int sum = 0;
	int num = square;
	int dec = 1;
	for (int i = 0; i < 9; ++i) {
		sum += ManhattanDistance(9 - i, (num / dec) % 10);
		dec *= 10;
	}

	return sum + count;
}

void Square::GetNext(std::vector<Square*>& tmp) {
	tmp.resize(4);
	tmp[0] = &squareDown();
	tmp[0]->move = 'D';
	tmp[1] = &squareUp();
	tmp[1]->move = 'U';
	tmp[2] = &squareRight();
	tmp[2]->move = 'R';
	tmp[3] = &squareLeft();
	tmp[3]->move = 'L';
}


struct Comparator {
	bool operator() (const std::pair<int, Square*>& lhs,
		const std::pair<int, Square*>& rhs) const {
		if (lhs.first == rhs.first) {
			return lhs.second->square < rhs.second->square;
		} else {
			return lhs.first < rhs.first;
		}
	}
};

// Алгоритм А*
void Solve(Square& square, std::string& solution) {
	square.count = 0;
	std::set<std::pair<int, Square*>, Comparator> sq_set; //сет пар - расстояние, вершина как и в алгоритме Дейкстры

	int heur = square.CalcHeurusic();

	if (heur == 0) {
		return;
	}

	sq_set.insert(std::make_pair(heur, &square));

	std::set<int> visited;

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
					tmp[i]->count = vertex->count + 1;
					heuristic = tmp[i]->CalcHeurusic();


					if (tmp[i]->square == 1123456780) {
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

	std::vector<int> arr(10);

	for (int i = 0; i < 9; ++i) {
		int d = 0;
		std::cin >> d;
		arr[9 - i] = d;
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