/*
	A. Поиск подстроки

	Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
	p <= 30000, n <= 300000.

	- С помощью префикс-функции;
*/

#include <iostream>
#include <string>
#include <vector>

int getPrefixValue(int pos, const std::string& str, std::vector<int>& values, int& previous_value) {

	int value = previous_value;

	while (value > 0) {
		if (str[value] != str[pos]) {
			value = values[value - 1];
		}
		else {
			break;
		}
	}

	if (str[pos] == str[value]) {
		++value;
	}

	previous_value = value;

	return value;
}

void FindPattern(std::string& pattern, char delimiter) {

	std::vector<int> values(pattern.length() + 1, 0);		// O(p) memory

	int previous_value = values[0];

	for (int i = 1; i < pattern.length(); ++i) {
		values[i] = getPrefixValue(i, pattern, values, previous_value);
	}

	char next_sym;
	int pos = 0;
	std::string str = pattern + delimiter;

	while (std::cin >> next_sym) {		// finding positions of pattern in text

		str += next_sym;

		int result = getPrefixValue(str.length() - 1, str, values, previous_value);

		if (result == pattern.length()) {
			std::cout << pos - pattern.length() + 1 << " ";
		}

		str.erase(str.end() - 1);
		++pos;
	}
}

int main() {

	std::string pattern;
	char delimiter;

	std::getline(std::cin, pattern);
	delimiter = '#';

	FindPattern(pattern, delimiter);

	return 0;
}