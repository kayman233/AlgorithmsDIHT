/*
2.2
Заявки на переговоры

В большой IT-фирме есть только одна переговорная комната. Желающие посовещаться заполняют заявки с желаемым временем начала и конца.
Ваша задача определить максимальное количество заявок, которое может быть удовлетворено. Число заявок ≤ 100000.

Док-во оптиммальности:
	Допустим, алгоритм не оптимальный
	Если в какой-то момент в оптимальном решении была выбрана другая встреча, но тогда по времени начинается позже, а заканчивается в то же
	время, что и встреча, которая была выбрана алгоритмом. Но это не влияет на результат решения в этот моментю
	Рассуждая аналогично и в остальные моменты работы алгоритма убеждаемся, что алгоритм работает оптимально.

*/

#include <iostream>
#include <vector>
#include <algorithm>

bool sort_second(std::pair<int, int> &a, std::pair<int, int> &b) {
	return(a.second < b.second);
};

int number_of_conversations(std::vector< std::pair <int, int> >& queue) {

	std::sort(queue.begin(), queue.end(), sort_second);

	int count = 1;
	if (queue[0].second == 0) {
		count = 0;
	}
	int time = queue[0].second;

	for (auto it = queue.begin() + 1; it != queue.end(); ++it) {
		if (it->first >= time) {
			++count;
			time = it->second;
		}
	}

	return count;
}

int main() {

	std::vector< std::pair <int, int> > queue;
	int in = 0;
	int out = 0;
	while (std::cin >> in, std::cin >> out) {
		queue.push_back(std::make_pair(in, out));
	}

	std::cout << number_of_conversations(queue) << std::endl;

	return 0;
}