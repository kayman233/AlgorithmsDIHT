/*
	Задано N точек на плоскости. Указать (N−1)-звенную несамопересекающуюся незамкнутую ломаную, проходящую через все эти точки.
	Указание: стройте ломаную в порядке возрастания x-координаты. Если имеются две точки с одинаковой x-координатой,
	то расположите раньше ту точку, у которой y-координата меньше.
*/

#include <iostream>
#include <vector>

struct Point {
	int x = 0;
	int y = 0;
};

bool operator<(const Point& first, const Point& second) {

	if (first.x < second.x) {
		return true;
	} else if (first.x == second.x){
		return first.y < second.y;
	}

	return false;
}

void InsertionSort(std::vector<Point>& a, int n) { // insertion sort

	for (int i = 1; i < n; ++i) {
		Point temp = a[i];
		int j = i - 1;

		while (j >= 0 && temp < a[j]) {
			a[j + 1] = a[j];
			--j;
		}

		a[j + 1] = temp;
	}

}

int main() {
	int n = 0;
	std::cin >> n;

	std::vector<Point> arr(n);

	for (int i = 0; i < n; ++i) {
		std::cin >> arr[i].x >> arr[i].y;
	}

	InsertionSort(arr, n);

	for (int i = 0; i < n; ++i) {
		std::cout << arr[i].x << " " << arr[i].y << std::endl;
	}

	return 0;
}