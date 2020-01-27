/*
	В супермаркете решили оптимизировать показ рекламы.
	Известно расписание прихода и ухода покупателей (два целых числа).
	Каждому покупателю необходимо показать минимум 2 рекламы. Рекламу можно транслировать только в целочисленные моменты времени.
	Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
	В каждый момент времени может показываться только одна реклама. Считается, что реклама показывается мгновенно.
	Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.

	Требуется определить минимальное число показов рекламы.
*/

#include <iostream>
#include <vector>
#include <algorithm>

struct Customer {
	int arrival = 0;
	int departure = 0;
};

bool operator<(const Customer& first, const Customer& second) {

	if (first.departure < second.departure) {
		return true;
	} else if (first.departure == second.departure) {
		return first.arrival < second.arrival;
	}

	return false;
}

void SiftDown(std::vector<Customer>& arr, int size, int index) {
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	int largest = index;

	if (left < size && arr[largest] < arr[left]) {
		largest = left;
	}

	if (right < size && arr[largest] < arr[right]) {
		largest = right;
	}

	if (largest != index) {
		std::swap(arr[largest], arr[index]);
		SiftDown(arr, size, largest);
	}
}

void BuildHeap(std::vector<Customer>& arr, int n) {
	for (int i = n / 2 - 1; i >= 0; --i) {
		SiftDown(arr, n, i);
	}
}

void HeapSort(std::vector<Customer>& arr, int n) {

	int size = n;
	BuildHeap(arr, n);

	while (size > 1) {
		std::swap(arr[0], arr[size - 1]);
		--size;
		SiftDown(arr, size, 0);
	}

}

int getAdvertNum(std::vector<Customer>& arr, int n) {
	int number = 0;
	int ult_adv = -1;				//заведем 2 доп переменные, в которых будем хранить время показа последней и предпоследней рекламы
	int preult_adv = -1;

	for (int i = 0; i < n; ++i) {
		if (preult_adv >= arr[i].arrival && ult_adv <= arr[i].departure) { //если реклама уже была показана 2 раза, то больше и не надо
			continue;
		}

		if (ult_adv < arr[i].arrival) { //показываем рекламу в последние 2 момента, если не показывали последнюю рекламу(автоматически предпоследнюю тоже не показывали)
			preult_adv = arr[i].departure - 1;
			ult_adv = arr[i].departure;
			number += 2;
		} else {
			if (ult_adv == arr[i].departure) { // в случае, когда последний показ рекламы совпадает со временем ухода, мы должны также добавить один показ перед последним 
				preult_adv = ult_adv - 1;
				++number;
			} else {
				preult_adv = ult_adv; //иначе просто обновляем препоследний на последний, а последний показ в момент ухода
				ult_adv = arr[i].departure;
				++number;
			}
		}
	}

	return number;
}

int main() {

	int n = 0;
	std::cin >> n;

	std::vector<Customer> arr(n);

	for (int i = 0; i < n; ++i) {
		std::cin >> arr[i].arrival >> arr[i].departure;
	}

	HeapSort(arr, n);

	std::cout << getAdvertNum(arr, n) << std::endl;

	return 0;
}