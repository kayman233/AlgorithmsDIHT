/*
	B. Выпуклая оболочка 3D
	Даны n точек в пространстве. Никакие 4 точки не лежат в одной плоскости. Найдите выпуклую оболочку этих точек за O(n*log(n)).
*/


// !!! Solution with O(n^2) - gift wrapping !!!

#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>

struct Vec3 {
	int x;
	int y;
	int z;

	int id;

	Vec3(int x, int y, int z, int id) : x(x), y(y), z(z), id(id) {}

	Vec3(const Vec3& other) { 
		x = other.x;
		y = other.y,
		z = other.z;
		id = other.id;
	}

	Vec3& operator=(const Vec3& other) {
		x = other.x;
		y = other.y,
		z = other.z;
		id = other.id;

		return *this;
	}

	friend bool operator<(const Vec3& first, const Vec3& second) {

		if (first.z < second.z) {
			return true;
		} else if (first.z == second.z) {
			if (first.y < second.y) {
				return true;
			} else if (first.y == second.y) {
				return first.x < second.x;
			}
		}

		return false;
	}

	friend bool operator==(const Vec3& first, const Vec3& second) {

		if (!(first < second)) {
			if (!(second < first)) {
				return true;
			}
		}

		return false;
	}

	Vec3 operator-(const Vec3& other) {

		Vec3 answer = *this;
		answer.x -= other.x;
		answer.y -= other.y;
		answer.z -= other.z;
		return answer;
	}

	Vec3 operator*(const Vec3& other) {

		Vec3 answer(0, 0, 0, -1);

		answer.x = y * other.z - z * other.y;
		answer.y = z * other.x - x * other.z;
		answer.z = x * other.y - y * other.x;

		return answer;
	}

	double length() {
		return sqrt(x * x + y * y + z * z);
	}
};

struct Face {
	int a;
	int b;
	int c;

	Face(int a, int b, int c) : a(a), b(b), c(c) {}
};

struct lex_compare {
	bool operator() (const Face& lhs, const Face& rhs) const {
		if (lhs.a < rhs.a) {
			return true;
		} else if (lhs.a == rhs.a) {
			if (lhs.b < rhs.b) {
				return true;
			} else if (lhs.b == rhs.b) {
				return lhs.c < rhs.c;
			}
		}

		return false;
	}
};

struct Edge {
	Vec3 p;
	Vec3 q;

	Edge(Vec3 p, Vec3 q) : p(p), q(q) {}
};

Vec3 getMin(std::vector<Vec3>& points) {
	Vec3 min = points[0];

	for (int i = 1; i < points.size(); ++i) {
		if (points[i] < min) {
			min = points[i];
		}
	}

	return min;
}

double DotProduct(Vec3& first, Vec3& second) {
	double answer;

	answer = first.x * second.x;
	answer += first.y * second.y;
	answer += first.z * second.z;

	return answer;
}

double AreaSquared(Vec3& a, Vec3& b, Vec3& c) {
	double answer;
	Vec3 side1(b - a);
	Vec3 side2(c - a);

	Vec3 cross_product = side1 * side2;

	answer = cross_product.length() / 2;

	return answer;
}

double SignedVolume(Vec3& a, Vec3& b, Vec3& c, Vec3& d) {
	double answer;

	Vec3 side1(b - a);
	Vec3 side2(c - a);
	Vec3 side3(d - a);

	Vec3 cross_product = side1 * side2;

	answer = DotProduct(cross_product, side3) / 6;

	return answer;
}

Vec3 PivotEdge(Edge& e, std::vector<Vec3>& points) {
	Vec3 p(0, 0, 0, -1);

	if (e.p.id != 0 && e.q.id != 0) { // to get the third point
		p = points[0];
	} else if (e.p.id != 1 && e.q.id != 1) {
		p = points[1];
	} else {
		p = points[2];
	}

	double area = AreaSquared(e.p, e.q, p);

	for (int i = 1; i < points.size(); ++i) {
		double volume = SignedVolume(e.p, e.q, p, points[i]);
		if (volume < 0) { // if can choose another point
			p = points[i];
		}
	}

	return p;
}

Edge FindEdge(std::vector<Vec3>& points) {
	Vec3 p = getMin(points); // most bottom, most left, most back
	Vec3 q = p;

	for (int i = 0; i < points.size(); ++i) {
		if (q.z == points[i].z && q.y == points[i].y && q.x < points[i].x) {
			q = points[i];
		}
	}

	if (q == p) { // to rotate
		q.x += 1;
	}
	Edge new_edge(p, q);
	q = PivotEdge(new_edge, points);

	Edge ans(p, q);
	return ans;
}

struct cmp {
	bool operator() (const Edge& lhs, const Edge& rhs) const {
		if (lhs.p < rhs.p) {
			return 1;
		} else if (lhs.p == rhs.p) {
			return lhs.q < rhs.q;
		}

		return 0;
	}
};

bool isUsed(Edge& edge, std::set<Edge, cmp>& used_edges) {

	if (used_edges.find(edge) == used_edges.end()) {
		return 0;
	}

	return 1;
}

Vec3 GetNormal(Vec3& a, Vec3& b, Vec3& c) {
	Vec3 side1 = b - a;
	Vec3 side2 = c - a;

	Vec3 ans = side1 * side2;

	return ans;
}

void AddMin(Vec3& a, Vec3& b, Vec3& c, Vec3& normal, std::set<Face, lex_compare>& faces) {
	Vec3 normal2 = GetNormal(a, b, c); // chech if normal is co-directed
	if (normal2 == normal) {
		Face new_face(a.id, b.id, c.id);
		faces.insert(new_face);
	} else {
		Face new_face(a.id, c.id, b.id);
		faces.insert(new_face);
	}
}

void AddFace(Edge& edge, Vec3& r, Vec3& normal, std::set<Face, lex_compare>& faces) {

	int min = std::min(edge.p.id, edge.q.id);
	min = std::min(min, r.id); // find min
	if (edge.p.id == min) {
		AddMin(edge.p, edge.q, r, normal, faces);
	} else if (edge.q.id == min) {
		AddMin(edge.q, edge.p, r, normal, faces);
	} else {
		AddMin(r, edge.p, edge.q, normal, faces);
	}
}

void BuildConvexHull(std::vector<Vec3>& points, std::set<Face, lex_compare>& faces) { // gift wrapping

	Edge edge = FindEdge(points);
	Vec3 r = PivotEdge(edge, points);

	Vec3 normal = GetNormal(edge.q, edge.p, r);

	std::stack<Edge> stack;
	Edge first(edge.q, edge.p);
	Edge second(r, edge.q);
	Edge third(edge.p, r);
	stack.push(first);
	stack.push(second);
	stack.push(third);

	AddFace(edge, r, normal, faces);

	std::set<Edge, cmp> used_edges;

	while (stack.size() > 0) {

		Edge edge_ = stack.top();
		stack.pop();

		if (isUsed(edge_, used_edges) == 0) {

			Vec3 q = PivotEdge(edge_, points);
			normal = GetNormal(edge_.q, edge_.p, q);
			AddFace(edge_, q, normal, faces);

			Edge first_(edge_.q, edge_.p);
			Edge second_(r, edge_.q);
			Edge third_(edge_.p, r);

			stack.push(first);
			stack.push(second);
			stack.push(third);

			used_edges.insert(edge_);
		}
	}
}

void PrintConvexHull(std::vector<Vec3>& points) {
	std::set<Face, lex_compare> faces;
	BuildConvexHull(points, faces);

	std::cout << faces.size() << std::endl;
	for (auto it = faces.begin(); it != faces.end(); ++it) {
		std::cout << 3 << " " << (*it).a << " " << (*it).b << " " << (*it).c << std::endl;
	}
}

int main() {

	int m;
	std::cin >> m;

	for (int k = 0; k < m; ++k) {
		int n;
		std::cin >> n;

		std::vector<Vec3> points;
		for (int i = 0; i < n; ++i) {
			int x;
			int y;
			int z;
			std::cin >> x >> y >> z;
			Vec3 new_vector(x, y, z, i);
			points.push_back(new_vector);
			points[i].id = i;
		}

		PrintConvexHull(points);
	}

	return 0;
}