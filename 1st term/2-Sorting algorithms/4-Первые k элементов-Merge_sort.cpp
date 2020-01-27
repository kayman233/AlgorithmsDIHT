/*
	Дана очень длинная последовательность целых чисел длины n. Требуется вывести в отсортированном виде её наименьшие k элементов.
	Последовательность может не помещаться в память. Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.

	Будем отсортировать массивы длины k(или меньше, если в конце последовательности) и сливать в результатирующий массив, длины k.
	Для этого сначала создаем этот результатирующий массив и сортируем его. Далее с помощью доп массива сортируем следующие k элементов(или меньше).
	После этого сливаем эти 2 массива.
	В итоге, получим массив, в котором отсортированны первые k элементов последовательности.

	Время работы O(n * log(k)), т.к сортировка слияением работает за O(k * log(k)) и будет выполнена n/k раз
*/

#include <iostream>
#include <vector>

void Merge(std::vector<int>& arr, int left, int middle, int right) {  // merge arrays
	// split into 2 arrays
	int first_size = middle - left + 1; 
	int second_size = right - middle;

	std::vector<int> first(first_size);
	std::vector<int> second(second_size);

	for (int i = 0; i < first_size; i++) {
		first[i] = arr[left + i];
	}

	for (int j = 0; j < second_size; j++) {
		second[j] = arr[middle + 1 + j];
	}

	// merge
	int i = 0;
	int j = 0;
	int k = left;
	while (i < first_size && j < second_size) {
		if (first[i] <= second[j]) {
			arr[k++] = first[i++];
		} else {
			arr[k++] = second[j++];
		}
	}
	while (i < first_size) {
		arr[k++] = first[i++];
	}
	while (j < second_size) {
		arr[k++] = second[j++];
	}
}

void MergeSort(std::vector<int>& arr, int left, int right) {
	if (left < right) {
		int middle = left + (right - left) / 2;
		MergeSort(arr, left, middle);
		MergeSort(arr, middle + 1, right);

		Merge(arr, left, middle, right);
	}
}

void MergeStatic(std::vector<int>& main_arr, std::vector<int>& arr, int first_len, int second_len) { //слияние результатирующего массива длины first_len с текущим

	std::vector<int> temp(first_len);
	int i = 0;
	int j = 0;

	int pos = 0;

	while (i < first_len && j < second_len && pos < first_len) {
		if (main_arr[i] <= arr[j]) {
			temp[pos++] = main_arr[i++];
		}
		else {
			temp[pos++] = arr[j++];
		}
	}

	while (i < first_len && j < first_len && pos < first_len) {
		temp[pos++] = main_arr[i++];
	}

	for (int i = 0; i < first_len; ++i) {
		main_arr[i] = temp[i];
	}
}

int main() {

	int n = 0;
	std::cin >> n;
	int k = 0;
	std::cin >> k;

	std::vector<int> main_arr(k);
	std::vector<int> additional_arr(k);

	for (int i = 0; i < k; ++i) { //считываем первые k элементов в массив, где будет хранится результат
		std::cin >> main_arr[i];
	}

	MergeSort(main_arr, 0, k - 1); //сортируем его

	int pos = k;
	while (pos < n) {
		int i = 0;
		for (; i < k && pos < n; ++i) {
			std::cin >> additional_arr[i];
			++pos;
		}
		MergeSort(additional_arr, 0, i - 1); //сортируем массив следующих элементов длины k(или меньше)

		MergeStatic(main_arr, additional_arr, k, i); //сливаем с результатирующим массивом
	}

	for (int i = 0; i < k; ++i) {
		std::cout << main_arr[i] << " ";
	}

	return 0;
}