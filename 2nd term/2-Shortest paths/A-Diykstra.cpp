/*
	Задача 1. «Города»
	Требуется отыскать самый выгодный маршрут между городами. Требуемое время работы O((N+M)logN),
	где N-количество городов, M-известных дорог между ними.
	Оптимизируйте ввод
*/

#include <iostream>
#include <vector>
#include <list>
#include <set>

#define INF 100000

class ListGraph {
public:
	ListGraph(int count);
	~ListGraph();

	void AddEdge(int from, int to, int d);
	int VerticesCount() const;
	void GetVertices(int vertex, std::vector<int>& vertices) const;
	int GetDist(int from, int to);
private:
	unsigned int vertex_number;
	std::vector<std::list<std::pair<int, int>>> edges;
};

ListGraph::ListGraph(int count) {
	vertex_number = count;

	edges.resize(count);
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to, int d) {
	if (from >= vertex_number || to >= vertex_number) {
		return;
	}

	bool add_reversed = 0;

	for (auto it = edges[from].begin(); it != edges[from].end(); ++it) {
		if ((*it).first == to) {
			if ((*it).second > d) {
				edges[from].erase(it);
				edges[from].push_back(std::make_pair(to, d));
			}
			add_reversed = 1;
			break;
		}
	}

	if (add_reversed) {
		for (auto it = edges[to].begin(); it != edges[to].end(); ++it) {
			if ((*it).first == from) {
				if ((*it).second > d) {
					edges[to].erase(it);
					edges[to].push_back(std::make_pair(from, d));
				}
				return;
			}
		}
	}
	

	edges[from].push_back(std::make_pair(to, d));
	edges[to].push_back(std::make_pair(from, d));
}

int ListGraph::VerticesCount() const {
	return vertex_number;
}

void ListGraph::GetVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (auto it = edges[vertex].begin(); it != edges[vertex].end(); ++it) {
		vertices.push_back((*it).first);
	}
}

int ListGraph::GetDist(int from, int to) {
	for (auto it = edges[from].begin(); it != edges[from].end(); ++it) {
		if ((*it).first == to) {
			return (*it).second;
		}
	}

	return INF;
}

int ShortestPath(ListGraph& graph, int from, int to) {
	std::set<std::pair<int, int>> dist_set; //сет пар - расстояние, вершина

	std::vector<int> distances(graph.VerticesCount(), INF);

	distances[from] = 0;
	dist_set.insert(std::make_pair(0, from));

	while (dist_set.size() > 0) {
		std::pair<int, int> min = *(dist_set.begin());

		dist_set.erase(dist_set.begin());

		int vertex = min.second;

		std::vector<int> tmp;
		graph.GetVertices(vertex, tmp);

		for (int i = 0; i < tmp.size(); ++i) {

			int v = tmp[i];
			int w = graph.GetDist(vertex, v);

			if ((distances[v] > distances[vertex] + w) && (v != vertex) && (w >= 0)) {

				if (distances[v] != INF) {
					dist_set.erase(dist_set.find(std::make_pair(distances[v], v)));
				}

				distances[v] = distances[vertex] + w;
				dist_set.insert(std::make_pair(distances[v], v));
			}
		}
	}

	return distances[to];
}

int main() {
	int n = 0;
	int m = 0;

	std::cin >> n;
	std::cin >> m;

	ListGraph graph(n);

	for (int i = 0; i < m; ++i) {
		int from = 0;
		int to = 0;
		int dist = 0;

		std::cin >> from;
		std::cin >> to;
		std::cin >> dist;
		graph.AddEdge(from, to, dist);
	}

	int from = 0;
	int to = 0;

	std::cin >> from;
	std::cin >> to;

	std::cout << ShortestPath(graph, from, to) << std::endl;

	return 0;
}