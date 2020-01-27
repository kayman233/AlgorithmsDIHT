/*
3.1 Высокая пирамида
	Дано N кубиков. Требуется определить каким количеством способов можно выстроить из этих кубиков пирамиду.
*/

#include <iostream>
#include <vector>

long long number_of_pyramids(int n) {

	std::vector<std::vector<long long> > table(n + 1, std::vector<long long>(n + 1)); //создание таблицы
	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j <= n; ++j) {
			table[i][j] = 0;
		}
	}

	table[0][0] = 1;

	for (int i = 0; i <= n; ++i) { //задача аналогична нахождению кол-ва разбиений часла на слагаемые
		for (int j = 1; j <= n; ++j) {
			if (j <= i) {
				table[i][j] = table[i][j - 1] + table[i - j][j];  //разбиения, в которых нет j + в которых есть(совпадает с количеством разбиений числа i−j на слагаемые)
			} else {
				table[i][j] = table[i][i];
			}
		}
	}

	return table[n][n];
}

int main() {

	int n = 0;
	std::cin >> n;

	if (n == 0) {
		std::cout << 0 << std::endl;
	} else {
		std::cout << number_of_pyramids(n) << std::endl;
	}
	return 0;
}