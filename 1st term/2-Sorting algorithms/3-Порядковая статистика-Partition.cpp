/*
	Реализуйте стратегию выбора опорного элемента “медиана трёх”
	Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу
*/

#include <iostream>
#include <vector>

int median(int a, int b, int c) //медиана трёх
{
	if (a > b) {
		std::swap(a, b);
	}

	if (b > c) {
		std::swap(b, c);
	}

	return b;
}

int partition(std::vector<int>& a, int left, int right) {
	int mid = (left + right) / 2;
	int pivot = median(a[left], a[right], a[mid]);

	if (pivot == a[mid]) {
		std::swap(a[right], a[mid]);
	}

	if (pivot == a[left]) {
		std::swap(a[right], a[left]);
	}

	int i = left;
	for (int j = left; j < right; ++j) { //from start to end
		if (a[j] < pivot) {
			std::swap(a[i], a[j]);
			i++;
		}
	}
	std::swap(a[i], a[right]);
	return i;
}

int order_statictics(std::vector<int>& a, int n, int k) { //find kth order statistic

	int left = 0;
	int right = n - 1;

	while (true) {

		int part = partition(a, left, right);

		if (part == k) {
			return a[k];
		}

		if (k < part) {
			right = part - 1;
		} else {
			left = part + 1;
		}

	}
}

int main() {

	int n = 0;
	std::cin >> n;

	int k = 0;
	std::cin >> k;

	std::vector<int> a(n);
	for (int i = 0; i < n; ++i) {
		std::cin >> a[i];
	}

	std::cout << order_statictics(a, n, k) << std::endl;

	return 0;
}