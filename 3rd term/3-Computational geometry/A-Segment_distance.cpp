/*
	A. Расстояние между отрезками
	Даны два отрезка в пространстве 
	(x1, y1, z1) - (x2, y2, z2) и (x3, y3, z3) - (x4, y4, z4). 
	Найдите расстояние между отрезками.
*/

#include <iostream>
#include <cmath>
#include <stdio.h>

const double eps = 1e-9;

struct Vec3 {
	double x;
	double y;
	double z;

	Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
	Vec3(const Vec3& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}
	void getVecBetween(Vec3& first, Vec3& second, double value);

	Vec3 operator-(const Vec3& other);

	double getLength();
};

double Vec3::getLength() {
	return sqrt( x * x + y * y + z * z);
}

Vec3 Vec3::operator-(const Vec3& other) {

	Vec3 answer = *this;
	answer.x -= other.x;
	answer.y -= other.y;
	answer.z -= other.z;
	return answer;
}

void Vec3::getVecBetween(Vec3& first, Vec3& second, double value) {
	*this = second - first;

	x *= value;
	y *= value;
	z *= value;

	x += first.x;
	y += first.y;
	z += first.z;
}

double GetDistance(Vec3& a1, Vec3& a2, Vec3& b1, Vec3& b2) {
	bool check_first = 1;

	double answer = 0;
	double answer_previous = 1;
	
	Vec3 first_point(a1);
	Vec3 second_point(b1);

	while (answer_previous - answer < -eps || answer_previous - answer > eps) { //	ternary search
		answer_previous = answer;

		if (check_first) {
			double left = 0;
			double right = 1;

			double a = 0;
			double b = 0;

			Vec3 left_point(a1);
			Vec3 right_point(a2);

			while (right - left > eps) {
				a = (left * 2 + right) / 3;
				b = (left + right * 2) / 3;

				left_point.getVecBetween(a1, a2, a);
				right_point.getVecBetween(a1, a2, b);

				if ((left_point - second_point).getLength() < (right_point - second_point).getLength()) {
					right = b;
				} else {
					left = a;
				}
			}

			first_point.getVecBetween(a1, a2, (right + left) / 2);
			answer = (first_point - second_point).getLength();

			check_first = 0;

		} else {

			double left = 0;
			double right = 1;

			double a = 0;
			double b = 0;

			Vec3 left_point(b1);
			Vec3 right_point(b2);

			while (right - left > eps) {
				a = (left * 2 + right) / 3;
				b = (left + right * 2) / 3;

				left_point.getVecBetween(b1, b2, a);
				right_point.getVecBetween(b1, b2, b);

				if ((left_point - first_point).getLength() < (right_point - first_point).getLength()) {
					right = b;
				} else {
					left = a;
				}
			}

			second_point.getVecBetween(b1, b2, (right + left) / 2);
			answer = (second_point - first_point).getLength();

			check_first = 1;
		}

	}

	return answer;
}

int main() {

	double x;
	double y;
	double z;

	std::cin >> x >> y >> z;
	Vec3 a1(x, y, z);

	std::cin >> x >> y >> z;
	Vec3 a2(x, y, z);

	std::cin >> x >> y >> z;
	Vec3 a3(x, y, z);

	std::cin >> x >> y >> z;
	Vec3 a4(x, y, z);
	
	double answer = GetDistance(a1, a2, a3, a4);

	printf("%0.8lf", answer);
	
	return 0;
}