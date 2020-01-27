/*
	C. Пересечение многоугольников
	Даны два выпуклых многоугольника на плоскости.
	В первом n точек, во втором m. Определите, пересекаются ли они за O(n + m).
	Указание. Используйте сумму Минковского.
*/


#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#define PI 3.1415926535

struct Vec3{
	double x;
	double y;

	Vec3(double x, double y) : x(x), y(y) {}

	Vec3(const Vec3& other) {
		x = other.x;
		y = other.y;
	}

	friend Vec3 operator+(const Vec3& a, const Vec3& b);

	friend Vec3 operator-(const Vec3& a, const Vec3& b);

	friend bool operator<(const Vec3& first, const Vec3& second);
};

Vec3 operator+(const Vec3& a, const Vec3& b) {
	Vec3 sum_vector(0, 0);
	sum_vector.x = a.x + b.x;
	sum_vector.y = a.y + b.y;

	return sum_vector;
}

Vec3 operator-(const Vec3& a, const Vec3& b) {
	Vec3 sum_vector(0, 0);
	sum_vector.x = a.x - b.x;
	sum_vector.y = a.y - b.y;

	return sum_vector;
}

bool operator<(const Vec3& first, const Vec3& second) {
	if (first.x < second.x) {
		return 1;
	}
	else if (first.x == second.x) {
		return first.y < second.y;
	}
	return 0;
}

void Shift(std::vector<Vec3>& polygon) {
	int min = 0;
	for (int i = 1; i < polygon.size(); ++i) {
		if (polygon[i] < polygon[min]) {
			min = i;
		}
	}
	std::rotate(polygon.begin(), polygon.begin() + min, polygon.end());
	std::reverse(polygon.begin() + 1, polygon.end());
}

double getPolarAngles(Vec3& a, Vec3& b) {
	Vec3 vec(b - a);

	double len = sqrt(vec.x * vec.x + vec.y * vec.y);

	double angle_cos = vec.x / len;
	double angle = acos(angle_cos) * 180.0 / PI;

	if (vec.y >= 0) {
		return angle;
	}

	return 360 - angle;
}

void getAngles(std::vector<Vec3>& points, std::vector<double>& angles) {
	Vec3 a = points[0];
	Vec3 b = points[1];
	Vec3 vec(b - a);

	int i = 0;
	int n = points.size();
	while (vec.y < 0 ) {
		angles.push_back(getPolarAngles(points[i % n], points[(i + 1) % n]) - 360);
		++i;
		vec.x = points[(i + 1) % n].x - points[i % n].x;
		vec.y = points[(i + 1) % n].y - points[i % n].y;
	}

	while (i < points.size()) {
		angles.push_back(getPolarAngles(points[i % n], points[(i + 1) % n]));
		++i;
	}
	angles.push_back(360 - angles[0]);
	angles.push_back(360 - angles[1]);
}

double getTriangleArea(Vec3& a, Vec3& b, Vec3& c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool isInside(Vec3& a, Vec3& b, Vec3& c) {
	return getTriangleArea(a, b, c) > 0; // check 3 points direction
}

void MinkowskiSum(std::vector<Vec3>& first, std::vector<Vec3>& second, std::vector<Vec3>& result_polygon) {
	int n = first.size();
	int m = second.size();

	int i = 0;
	int j = 0;

	std::vector<double> angles_first;
	std::vector<double> angles_second;

	getAngles(first, angles_first);
	getAngles(second, angles_second);

	while (i < n || j < m) {
		result_polygon.push_back(first[i % n] + second[j % m]);
		if (angles_first[i] < angles_second[j]) {
			++i;
		}
		else if (angles_first[i] > angles_second[j]) {
			++j;
		}
		else {
			++i;
			++j;
		}
	}
}

bool haveIntersection(std::vector<Vec3>& first, std::vector<Vec3>& second) {

	int n = first.size();
	int m = second.size();

	for (int i = 0; i < n; ++i) { // -first
		first[i].x *= -1;
		first[i].y *= -1;
	}

	std::vector<Vec3> result_polygon;

	Shift(first); // rotate from min
	Shift(second);

	MinkowskiSum(first, second, result_polygon);

	Vec3 zero(0, 0); // if zero is inside
	int size = result_polygon.size();
	int k = 0;
	while (k < size && isInside(result_polygon[k], result_polygon[(k + 1) % size], zero)) {
		++k;
	}
	if (k == size) {
		return 1;
	}
	
	return 0;
}

int main() {

	int n;
	int m;

	std::cin >> n;
	std::vector<Vec3> first_polygon;
	for (int i = 0; i < n; ++i) {
		double x;
		double y;
		std::cin >> x >> y;
		Vec3 new_point(x, y);
		first_polygon.push_back(new_point);
	}

	std::cin >> m;
	std::vector<Vec3> second_polygon;
	for (int i = 0; i < m; ++i) {
		double x;
		double y;
		std::cin >> x >> y;
		Vec3 new_point(x, y);
		second_polygon.push_back(new_point);
	}

	if (haveIntersection(first_polygon, second_polygon)) {
		std::cout << "YES";
	} else {
		std::cout << "NO";
	}

	return 0;
}