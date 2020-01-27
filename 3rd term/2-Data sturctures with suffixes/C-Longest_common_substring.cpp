/*
	C. K-я общая подстрока

	Заданы две строки s, t и целое число k.
	Рассмотрим множество всех таких непустых строк, которые встречаются как подстроки в s и t одновременно. 
	Найдите k-ую в лексикографическом порядке строку из этого множества.

*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void BuildSuffArray(std::string& str1, std::vector<int>& suf) {

	int alphabet_size = 256;

	str1 += '\0';
	int length = str1.length();
	suf.assign(length, 0);

	std::vector<int> classes(length, 0);
	std::vector<int> count(alphabet_size, 0);

	for (int i = 0; i < length; ++i) {
		++count[str1[i]];
	}
	for (int i = 1; i < alphabet_size; ++i) {
		count[i] += count[i - 1];
	}
	for (int i = 0; i < length; ++i) {
		suf[--count[str1[i]]] = i;
	}

	classes[suf[0]] = 0;

	int num_of_classes = 1;

	for (int i = 1; i < length; ++i) {
		if (str1[suf[i]] != str1[suf[i - 1]]) {
			++num_of_classes;
		}
		classes[suf[i]] = num_of_classes - 1;
	}

	std::vector<int> second_pair(length, 0);
	std::vector<int> first_pair(length, 0);

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
			classes[i] = first_pair[i];
		}
	}
}

void BuildLCP(std::vector<int>& lcp, const std::string& str1, const std::vector<int>& suf) {
	int length = str1.length();

	std::vector<int> inverse_suf(length, 0);

	for (int i = 0; i < length; ++i) {
		inverse_suf[suf[i]] = i;
	}

	int position = 0;

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
			(str1[i + position] == str1[j + position])) {
			++position;
		}
		lcp[inverse_suf[i]] = position;
	}
}

bool getStringNum(int pos, int str1_len) {
	if (pos < str1_len) {
		return 0;
	}
	return 1;
}

std::string GetAnswer(std::string& str1, std::string& str2, long long k) {

	str1 += "$";
	int str1_len = str1.length();
	str1 += str2;

	//building suff array and lcp for it
	std::vector<int> suf(str1.length(), 0);
	BuildSuffArray(str1, suf);

	std::vector<int> lcp(str1.length(), 0);
	BuildLCP(lcp, str1, suf);

	long long answer_count = 0; // counter
	int additional = 0; // LCP with the last suffix from the different string

	for (int i = 1; i < str1.length(); ++i) {
		int previous = suf[i - 1];
		int current = suf[i];

		if (getStringNum(previous, str1_len) != getStringNum(current, str1_len)) {
			answer_count += std::max(0, lcp[i - 1] - additional); // add to the counter
			additional = lcp[i - 1]; // new value

			if (answer_count >= k) {
				std::string answer = ""; // get string
				for (int j = 0; j < lcp[i - 1] - (answer_count - k); ++j) {
					answer += str1[current + j];
				}
				return answer;
			}

		} else {
			additional = std::min(additional, lcp[i - 1]); // update LCP as it's the minimum in the range
		}
	}

	return "-1";
}

int main() {

	std::string str1;
	std::string str2;
	long long k = 0;

	std::cin >> str1;
	std::cin >> str2;
	std::cin >> k;

	std::cout << GetAnswer(str1, str2, k);

	return 0;
}