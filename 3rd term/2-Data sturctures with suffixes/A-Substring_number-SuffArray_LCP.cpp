/*
	A. Количество различных подстрок

	Дана строка длины n. Найти количество ее различных подстрок. Используйте суффиксный массив.

	Построение суффиксного массива выполняйте за O(n log n). 
	Вычисление количества различных подстрок выполняйте за O(n)
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int alphabet_size = 256;

void BuildSuffArray(std::string& str, std::vector<int>& suf) {

	str += '\0';
	int length = str.length();
	suf.assign(length, 0);

	std::vector<int> classes(length, 0);
	std::vector<int> count(alphabet_size, 0);

	// for step = 0
	for (int i = 0; i < length; ++i) {
		++count[str[i]];
	}
	for (int i = 1; i < alphabet_size; ++i) {
		count[i] += count[i - 1];
	}
	for (int i = 0; i < length; ++i) {
		suf[--count[str[i]]] = i;
	}
	
	classes[suf[0]] = 0;

	int num_of_classes = 1;

	for (int i = 1; i < length; ++i) {
		if (str[suf[i]] != str[suf[i - 1]]) {
			++num_of_classes;
		}
		classes[suf[i]] = num_of_classes - 1;
	}

	std::vector<int> second_pair(length, 0); // to get sort for the second pair
	std::vector<int> first_pair(length, 0); // for the first

	// for next steps for n*log(n)
	for (int step = 0; (1 << step) < length; ++step) {

		for (int i = 0; i < length; ++i) {
			second_pair[i] = suf[i] - (1 << step);
			if (second_pair[i] < 0) {
				second_pair[i] += length;
			}
		}

		count.assign(num_of_classes, 0);

		for (int i = 0; i < length; ++i) {
			++count[classes[second_pair[i]]];
		}
		for (int i = 1; i < num_of_classes; ++i) {
			count[i] += count[i - 1];
		}
		for (int i = length - 1; i >= 0; --i) {
			suf[--count[classes[second_pair[i]]]] = second_pair[i];
		}

		first_pair[suf[0]] = 0;

		num_of_classes = 1;

		for (int i = 1; i < length; ++i) {
			int pos1 = (suf[i] + (1 << step)) % length;
			int pos2 = (suf[i - 1] + (1 << step)) % length;

			if ((classes[suf[i]] != classes[suf[i - 1]]) || 
				(classes[pos1] != classes[pos2])
				) {
				++num_of_classes;
			}
			first_pair[suf[i]] = num_of_classes - 1;
		}
		for (int i = 0; i < length; ++i) {
			classes[i] = first_pair[i]; // update
		}
	}
}

void BuildLCP(std::vector<int>& lcp, const std::string& str, const std::vector<int>& suf) {
	int length = str.length();

	std::vector<int> inverse_suf(length, 0);

	for (int i = 0; i < length; ++i) {
		inverse_suf[suf[i]] = i;
	}

	int position = 0;

	// linear building 
	for (int i = 0; i < length; ++i) {
		if (position > 0) {
			--position;
		}
		if (inverse_suf[i] == length - 1) {
			lcp[length - 1] = -1;
			position = 0;
			continue;
		}

		int j = suf[inverse_suf[i] + 1];
		while ((std::max(i + position, j + position) < length) &&
				(str[i + position] == str[j + position]) ) {
			++position;
		}
		lcp[inverse_suf[i]] = position;
	}
}

int CountSubstrings(const std::vector<int>& lcp, const std::string& str, const std::vector<int>& suf) {
	int answer = 0;
	int length = str.length();

	// to get the answer we should sum for each suffix
	for (int i = 1; i < length; ++i) {
		answer += length - 1 - suf[i];
	}
	// but we shouldn't forget to decrease if neighboring suffixes have the same prefixes
	for (int i = 0; i < length - 1; ++i) {
		answer -= lcp[i];
	}

	return answer;
}

int getSubstringNumber(std::string& str) {
	std::vector<int> suf(str.length(), 0);
	BuildSuffArray(str, suf);

	std::vector<int> lcp(str.length(), 0);
	BuildLCP(lcp, str, suf);

	return CountSubstrings(lcp, str, suf);
}

int main() {
	
	std::string str;
	std::cin >> str;

	std::cout << getSubstringNumber(str);

	return 0;
}