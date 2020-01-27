/*
	Быстрейшая сортировка
*/

#include <iostream>
#include <vector>
#include <algorithm>

template<class T>
class Stack { // custom stack
public:
	Stack() {
		a = new T[0];
		tail = 0;
		capacity = 0;
		current_size = 0;
	}
	~Stack() {
		delete[] a;
	}

	void Push(int value) {
		if (tail >= capacity) {
			grow();
		}
		a[tail++] = value;
		++current_size;
	}
	int Pop() {
		if (current_size == 0) {
			return -1;
		}

		--current_size;
		return a[--tail];
	}

	bool is_empty() {
		return current_size < 1;
	}

private:
	T* a;
	T tail;
	int capacity;
	int current_size;

	void grow() {
		int new_cap = std::max(capacity * 2, 4);
		T* new_a = new T[new_cap];

		for (int i = 0; i < capacity; ++i) {
			new_a[i] = a[i];
		}

		delete[] a;
		a = new_a;
		capacity = new_cap;

	}

};

template<class T>
void InsertSort(std::vector<T>& a, int left, int right) {
	for (int i = left; i <= right; ++i) {
		T temp = a[i];
		int j = i - 1;
		while (j >= 0 && temp < a[j]) {
			a[j + 1] = a[j];
			--j;
		}
		a[j + 1] = temp;
	}
}

template<class T>
T median(T a, T b, T c) {
	if (a > b) std::swap(a, b);
	if (b > c) std::swap(b, c);

	return b;
}

template<class T>
int partition(std::vector<T>& a, int left, int right) {

	T med = (left + right) / 2;

	int pivot = median(a[left], a[right], a[med]); //выбор опорного "медиана трех"

	if (pivot == a[med]) {
		std::swap(a[right], a[med]);
	}

	if (pivot == a[left]) {
		std::swap(a[left], a[right]);
	}

	int i = left;
	int j = right - 1;

	while (i <= j) { //проход с двух сторон
		for (; a[i] < a[right]; ++i) {}
		for (; j > 0 && a[j] > pivot; --j) {}

		if (i <= j) {
			std::swap(a[i++], a[j--]);
		}
	}

	std::swap(a[right], a[i]);

	return i;
}

template<class T>
void fastest_sort(std::vector<T>& a, int left, int right) {

	Stack<T> stack; // stack for recursion

	stack.Push(left);
	stack.Push(right);

	while (!stack.is_empty()) {
		int second = stack.Pop();
		int first = stack.Pop();

		if (second - first <= 50) { // call insertion sort
			InsertSort(a, first, second);
		} else {
			int part = partition(a, first, second);

			if (part < second) {
				stack.Push(part + 1);
				stack.Push(second);
			}

			if (part > first) {
				stack.Push(first);
				stack.Push(part - 1);
			}
		}
	}
}

int main() {

	std::iostream::sync_with_stdio(false);
	std::cin.tie(0);

	std::vector<int> arr;
	int t = 0;
	while (std::cin >> t) {
		arr.push_back(t);
		if (std::cin.get() == '\n') {
			break;
		}
	}

	fastest_sort(arr, 0, arr.size() - 1);

	for (int i = 9; i < arr.size(); i += 10) {
		std::cout << arr[i] << " ";
	}

	return 0;
}