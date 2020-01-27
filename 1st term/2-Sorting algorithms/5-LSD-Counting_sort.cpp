/*
	Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 1000000.
	Отсортировать массив методом поразрядной сортировки LSD по байтам.


	В качестве устойчивой сортировки будем использовать сортировку подсчетом.
*/

#include <iostream>
#include <vector>

const int Size = 256;

int extractKthByte(const long long &number, int k) {
	const int lastByte = 0xFF;
	return number >> (k * 8) & lastByte;
}

void counting_sort(std::vector<long long>& a, int n, int byte) { //сортировка подсчетом
	std::vector<int> count(Size);

	for (int i = 0; i < Size; ++i) {
		count[i] = 0;
	}

	for (int i = 0; i < n; ++i) {
		int d = extractKthByte(a[i], byte);
		++count[d];
	}

	int sum = 0;
	for (int i = 0; i < Size; ++i) {
		int tmp = count[i];
		count[i] = sum;
		sum += tmp;
	}

	std::vector<long long> temp(n);

	for (int i = 0; i < n; ++i) {
		int d = extractKthByte(a[i], byte);
		temp[count[d]++] = a[i];
	}

	a = temp;
}

void LSD_sort(std::vector<long long>& a, int n) {
	for (int i = 0; i < sizeof(long long); ++i) {
		counting_sort(a, n, i);
	}
}

int main() {

	int n = 0;
	std::cin >> n;

	std::vector<long long> a(n);

	std::cin >> a[0];
	long long max = a[0];

	for (int i = 1; i < n; ++i) {
		std::cin >> a[i];
	}

	LSD_sort(a, n);

	for (int i = 0; i < n; ++i) {
		std::cout << a[i] << " ";
	}

	return 0;
}