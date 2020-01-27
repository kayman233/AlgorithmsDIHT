#include <iostream>
#include <vector>
#include <string>

class BigInteger {
public:
	// Constructors, dectructors, =
	BigInteger();
	BigInteger(int value);
	BigInteger(const BigInteger& other);
	BigInteger(std::string str);

	~BigInteger() = default;

	BigInteger& operator=(const BigInteger& other);

	// Arithmetic

	friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs);

	BigInteger& operator*=(const BigInteger& other);
	BigInteger& operator+=(const BigInteger& other);
	BigInteger& operator-=(const BigInteger& other);
	BigInteger& operator/=(const BigInteger& other);
	BigInteger& operator%=(const BigInteger& other);

	// Unary
	BigInteger operator+();
	BigInteger operator-();
	BigInteger& operator++();
	BigInteger operator++(int);
	BigInteger& operator--();
	BigInteger operator--(int);

	// Comparison
	friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);

	// Casts
	explicit operator bool();

	// toSring
	std::string toString() const;

	// Streams
	friend std::istream& operator >>(std::istream& istream, BigInteger& value);
	friend std::ostream& operator <<(std::ostream& ostream, const BigInteger& value);
private:
	std::vector<unsigned char> buffer;
	const unsigned char base = 100;
	bool is_signed = 0;

	void InvertSign();
	void CheckNull();

	friend BigInteger KaratsubaMul(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger Mul(const BigInteger& big, const BigInteger& small);
	friend void SplitAt(const BigInteger& value, BigInteger& left, BigInteger& right, int pos);
};

BigInteger::BigInteger() {
	buffer.push_back(0);
}

BigInteger::BigInteger(int value) {
	if (value < 0) {
		is_signed = 1;
	}
	value = abs(value);
	if (value == 0) {
		buffer.push_back(0);
	}
	while (value != 0) {
		buffer.push_back(value % 100);
		value /= 100;
	}

	CheckNull();
}

BigInteger::BigInteger(const BigInteger& other) {
	is_signed = other.is_signed;
	buffer = other.buffer;
}

BigInteger::BigInteger(std::string str) {
	int i = str.size();
	if (str[0] == '-') {
		is_signed = 1;
		str.erase(str.begin(), str.begin() + 1);
		--i;
		if (i == 0) {
			buffer.push_back(0);
		}
	}
	while (i > 1) {
		int val = 0;
		val = str[i - 2] - '0';
		val *= 10;
		val += str[i - 1] - '0';
		buffer.push_back(val);
		i -= 2;
	}
	if (i == 1) {
		buffer.push_back(str[0] - '0');
	}

	CheckNull();
}

BigInteger& BigInteger::operator=(const BigInteger& other) {
	if (this == &other) {
		return *this;
	}
	is_signed = other.is_signed;
	buffer = other.buffer;

	return *this;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
	BigInteger sum(lhs);
	sum += rhs;

	return sum;
}

BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
	BigInteger diff(lhs);
	diff -= rhs;

	return diff;
}

BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
	BigInteger result(0);
	result = KaratsubaMul(lhs, rhs);

	if (lhs.is_signed != rhs.is_signed) {
		result.is_signed = 1;
	}

	while (result.buffer.size() > 1 && result.buffer.back() == 0) {
		result.buffer.pop_back();
	}

	return result;
}

BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
	BigInteger del(0);
	del.buffer.resize(lhs.buffer.size());
	BigInteger current(0);

	for (int i = lhs.buffer.size() - 1; i >= 0; --i) {
		current.buffer.insert(current.buffer.begin(), 1, 0);
		current.CheckNull();
		current.buffer[0] = lhs.buffer[i];

		int digit = 0;
		int left = 0;
		int right = lhs.base;

		while (left <= right) {
			int mid = (left + right) / 2;
			BigInteger mul = rhs * mid;
			mul.is_signed = 0;
			if (mul <= current) {
				digit = mid;
				left = mid + 1;
			} else {
				right = mid - 1;
			}
		}

		del.buffer[i] = digit;
		if (rhs.is_signed) {
			current += rhs * digit;
		} else {
			current -= rhs * digit;
		}
	}
	if (lhs.is_signed != rhs.is_signed) {
		del.is_signed = 1;
	}

	del.CheckNull();

	return del;
}

BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) {
	BigInteger current(0);

	for (int i = lhs.buffer.size() - 1; i >= 0; --i) {
		current.buffer.insert(current.buffer.begin(), 1, 0);
		current.CheckNull();
		current.buffer[0] = lhs.buffer[i];

		int digit = 0;
		int left = 0;
		int right = lhs.base;

		while (left <= right) {
			int mid = (left + right) / 2;
			BigInteger mul = rhs * mid;
			mul.is_signed = 0;
			if (mul <= current) {
				digit = mid;
				left = mid + 1;
			} else {
				right = mid - 1;
			}
		}

		if (rhs.is_signed) {
			current += rhs * digit;
		} else {
			current -= rhs * digit;
		}
	}
	current.is_signed = lhs.is_signed;
	current.CheckNull();

	return current;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
	if ((is_signed && other.is_signed == 0) // 1000 += -1000 A = 1000 - 1000 <=> -1000 -= -1000 
		|| (is_signed == 0 && other.is_signed)) {
		InvertSign();
		(*this) -= other;
		InvertSign();
		CheckNull();
		return *this;
	}
	int size;
	if (buffer.size() < other.buffer.size()) {
		size = other.buffer.size();
	} else {
		size = buffer.size();
	}
	if (this == &other) {
		BigInteger plus(other);
		(*this) += plus;
		return *this;
	}

	int carry = 0;
	for (int i = 0; i < size || carry == 1; ++i) {
		if (i == (int)(buffer.size())) {
			buffer.push_back(0);
		}
		buffer[i] += carry;
		if (i < (int)(other.buffer.size())) {
			buffer[i] += other.buffer[i];
		}
		if (buffer[i] >= base) {
			buffer[i] -= base;
			carry = 1;
		} else {
			carry = 0;
		}
	}

	CheckNull();

	return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
	BigInteger minus(other);
	if (is_signed == other.is_signed) { // 1000 - 500 = 500 | -1000 - -500 = -500
									// 500 - 1000 = -500 | -500 - -1000 = 500
		if (is_signed == 0 && (*this) <= other) {
			minus = *this;
			*this = other;
			InvertSign();
		} else if (is_signed == 1 && (*this) >= other) {
			minus = *this;
			*this = other;
			InvertSign();
		}

		int other_size = minus.buffer.size();
		int carry = 0;
		for (int i = 0; i < other_size || carry == 1; ++i) {
			buffer[i] -= carry;
			if (i < other_size) {
				buffer[i] -= minus.buffer[i];
			}
			if (buffer[i] > base) {
				buffer[i] += base;
				carry = 1;
			} else {
				carry = 0;
			}
		}
	} else if ((is_signed && other.is_signed == 0) //-1000 -= 500 <=> -1000 - 500 
		|| (is_signed == 0 && other.is_signed)) {
		minus.InvertSign();
		(*this) += minus;

		return *this;
	}

	CheckNull();

	return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
	BigInteger mul = *this;
	*this = mul * other;

	return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
	BigInteger del = *this;
	*this = del / other;

	return *this;
}


BigInteger& BigInteger::operator%=(const BigInteger& other) {
	BigInteger rest = *this;
	*this = rest % other;

	return *this;
}

BigInteger BigInteger::operator+() {
	BigInteger plus = *this;

	return plus;
}

BigInteger BigInteger::operator-() {
	BigInteger minus = *this;
	minus.InvertSign();
	minus.CheckNull();
	return minus;
}

BigInteger& BigInteger::operator++() {
	*this += 1;
	return *this;
}

BigInteger BigInteger::operator++(int) {
	BigInteger plus = *this;
	*this += 1;
	return plus;
}

BigInteger& BigInteger::operator--() {
	*this -= 1;
	return *this;
}

BigInteger BigInteger::operator--(int) {
	BigInteger plus = *this;
	*this -= 1;
	return plus;
}

bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
	if (lhs.is_signed && rhs.is_signed == 0) {
		return 1;
	} else if (lhs.is_signed == 0 && rhs.is_signed) {
		return 0;
	}

	bool sign = lhs.is_signed;
	if (lhs.buffer.size() != rhs.buffer.size()) {
		return sign ^ (lhs.buffer.size() < rhs.buffer.size());
	}
	for (int i = lhs.buffer.size() - 1; i >= 0; --i) {
		if (lhs.buffer[i] < rhs.buffer[i]) {
			return sign ^ true;
		}

		if (lhs.buffer[i] > rhs.buffer[i]) {
			return sign ^ false;
		}
	}

	return false;
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
	return !(rhs < lhs);
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs) {
	return rhs < lhs;
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
	return !(lhs < rhs);
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
	return !(lhs < rhs) && !(rhs < lhs);
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
	return (lhs < rhs) || (rhs < lhs);
}

std::string BigInteger::toString() const {
	std::string str;
	if (is_signed) {
		str.append("-");
	}

	for (int i = buffer.size() - 1; i >= 0; --i) {
		if (i < int(buffer.size()) - 1 && buffer[i] < 10) {
			str.append("0");
		}
		str += std::to_string(buffer[i]);
	}

	return str;
}

BigInteger::operator bool() {
	if (buffer.size() > 1) {
		return 1;
	} else if (buffer.size() == 1) {
		if (buffer[0] == 0) {
			return 0;
		} else {
			return 1;
		}
	}

	return 0;
}

std::istream& operator >>(std::istream& istream, BigInteger& value) {
	std::string str;
	istream >> str;

	BigInteger new_val(str);
	value = new_val;

	return istream;
}

std::ostream& operator <<(std::ostream& ostream, const BigInteger& value) {
	ostream << value.toString();
	return ostream;
}

BigInteger Mul(const BigInteger& big, const BigInteger& small) {
	BigInteger result(0);
	if (big.is_signed != small.is_signed) {
		result.is_signed = 1;
	}
	result.buffer.resize(big.buffer.size() + 1, 0);
	for (int i = 0; i < (int)big.buffer.size(); ++i) {
		int carry = 0;
		for (int j = 0; j < 1 || carry; ++j) {
			int temp = result.buffer[i + j] + big.buffer[i] * (j == 0 ? small.buffer[j] : 0) + carry;
			result.buffer[i + j] = temp % big.base;
			carry = temp / big.base;
		}
	}

	while (result.buffer.size() > 1 && result.buffer.back() == 0) {
		result.buffer.pop_back();
	}

	return result;
}

BigInteger KaratsubaMul(const BigInteger& lhs, const BigInteger& rhs) {
	if (lhs.buffer.size() == 1) {
		return Mul(rhs, lhs);
	}
	if (rhs.buffer.size() == 1) {
		return Mul(lhs, rhs);
	}

	int size;
	if (lhs.buffer.size() < rhs.buffer.size()) {
		size = lhs.buffer.size();
	} else {
		size = rhs.buffer.size();
	}

	int mid = (size / 2);
	if (mid == 0) {
		mid = 1;
	}

	BigInteger lhs_left_part(0);
	BigInteger lhs_right_part(0);
	BigInteger rhs_left_part(0);
	BigInteger rhs_right_part(0);

	SplitAt(lhs, lhs_left_part, lhs_right_part, mid);
	SplitAt(rhs, rhs_left_part, rhs_right_part, mid);

	BigInteger first(KaratsubaMul(lhs_left_part, rhs_left_part));
	BigInteger second(KaratsubaMul(lhs_right_part, rhs_right_part));

	BigInteger diff_1(lhs_left_part - lhs_right_part);
	bool sign_1 = diff_1.is_signed;
	BigInteger diff_2(rhs_left_part - rhs_right_part);
	bool sign_2 = diff_2.is_signed;
	diff_1.is_signed = 0;
	diff_2.is_signed = 0;
	BigInteger third(KaratsubaMul(diff_1, diff_2));

	BigInteger res(0);
	res = first;

	BigInteger res_1(first + second);
	if (sign_1 == sign_2) {
		res_1 -= third;
	} else {
		res_1 += third;
	}
	res_1.buffer.insert(res_1.buffer.begin(), mid, 0);
	res_1.CheckNull();
	res += res_1;

	BigInteger res_2(second);
	res_2.buffer.insert(res_2.buffer.begin(), 2 * mid, 0);
	res_2.CheckNull();
	res += res_2;

	res.CheckNull();
	return res;
}

void SplitAt(const BigInteger& value, BigInteger& left, BigInteger& right, int pos) {
	left = right = value;
	if (right.buffer.begin() + pos < right.buffer.end()) {
		right.buffer.erase(right.buffer.begin(), right.buffer.begin() + pos);
		left.buffer.erase(left.buffer.begin() + pos, left.buffer.end());
	} else {
		right.buffer.erase(right.buffer.begin(), right.buffer.end());
	}
	if (right.buffer.size() == 0) {
		right.buffer.push_back(0);
	}

	left.CheckNull();
	right.CheckNull();
}

void BigInteger::InvertSign() {
	is_signed = is_signed ? 0 : 1;
}

void BigInteger::CheckNull() {
	while (buffer.size() > 1 && buffer.back() == 0) {
		buffer.pop_back();
	}

	if (buffer.size() == 1 && buffer[0] == 0 && is_signed) {
		InvertSign();
	}
}