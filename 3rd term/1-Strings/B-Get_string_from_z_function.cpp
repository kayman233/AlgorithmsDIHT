/*
	B2. Строка по Z-функции

	Найти лексикографически-минимальную строку, построенную по z-функции, в алфавите a-z.

*/


#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

const std::vector<char> alphabet = { 'x', 'y', 'z'};

void Check_Z_Block(int& pos, const std::vector<int>& z_array, std::string& min_string, std::vector<int>& used_symbols_pos) {

	int current_length = z_array[pos];
	int prefix_pos = 0;

	used_symbols_pos.clear();

	while (current_length > 0) {

		if (z_array[pos] > current_length) {
			current_length = z_array[pos];
			prefix_pos = 0;

			used_symbols_pos.push_back(z_array[pos]); // to save positions of used symbols
		}

		if (z_array[pos] == current_length) {
			used_symbols_pos.push_back(z_array[pos]);
		}

		min_string = min_string + min_string[prefix_pos]; // add to the result string
		++prefix_pos;
		--current_length;

		++pos;
	}

}

char FindSymbol(std::vector<int>& used_symbols_pos, const std::string& min_string) {

	int new_sym_ind = 1;

	std::unordered_set<char> used_symbols;
	for (int j = 0; j < used_symbols_pos.size(); ++j) {
		used_symbols.insert(min_string[used_symbols_pos[j]]);
	}

	while (used_symbols.find(alphabet[new_sym_ind]) != used_symbols.end()) {
		++new_sym_ind;
	}

	return alphabet[new_sym_ind];
}

std::string BuildString(const std::vector<int>& z_array) {
	
	if (z_array.size() == 0) {
		return "";
	}
	std::string min_string;

	min_string += alphabet[0];

	std::vector<int> used_symbols_pos;
	bool insert_after_block = 1;

	int i = 1;
	while (i < z_array.size()) {

		if (z_array[i] != 0) {
			Check_Z_Block(i, z_array, min_string, used_symbols_pos);
			insert_after_block = 1;
		} else {

			char new_sym = alphabet[1];

			if (insert_after_block) {
				new_sym = FindSymbol(used_symbols_pos, min_string);
				insert_after_block = 0;
			}

			min_string = min_string + new_sym;
			++i;
		}
	}

	return min_string;
}

int main() {
	
	int x;
	std::vector<int> result_array;

	while (std::cin >> x) {
		result_array.push_back(x);
	}

	std::cout << BuildString(result_array) << std::endl;

	return 0;
}