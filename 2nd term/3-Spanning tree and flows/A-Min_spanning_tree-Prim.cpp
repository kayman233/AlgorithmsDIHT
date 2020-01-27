/*
	Задача 1. «Минимальное остовное дерево»

	Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
	Вариант 1. С помощью алгоритма Прима.
*/

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <set>

#define INF 10000000

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

	if (from == to) {
		return 0;
	}

	return INF;
}

//Prim's algorithm
int SpanningTreeWeight(ListGraph& graph) {
	int weight = 0;
	std::set<std::pair<int, int>> dist_set;

	std::vector<int> distances(graph.VerticesCount(), INF);
	std::vector<int> parents(graph.VerticesCount(), -1);

	distances[0] = 0;
	dist_set.insert(std::make_pair(0, 0));

	for (int i = 1; i < graph.VerticesCount(); ++i) {
		dist_set.insert(std::make_pair(INF, i));
	}
	parents[0] = 0;
	
	while (dist_set.size() > 0) {
		std::pair<int, int> min = *(dist_set.begin());
		dist_set.erase(dist_set.begin());

		int vertex = min.second;
		
		std::vector<int> tmp;
		graph.GetVertices(vertex, tmp);

		for (int i = 0; i < tmp.size(); ++i) {
			int v = tmp[i];
			int w = graph.GetDist(vertex, v);

			if (distances[v] > w) {
				if (dist_set.find(std::make_pair(distances[v], v)) != dist_set.end()) {
					dist_set.erase(std::make_pair(distances[v], v));
					dist_set.insert(std::make_pair(w, v));

					distances[v] = w;
					parents[v] = vertex;
				}
			}
		}
	}

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		weight += graph.GetDist(i, parents[i]);
	}

	return weight;
}

int main() {

	int n = 0;
	int m = 0;

	std::cin >> n >> m;

	ListGraph graph(n);

	for (int i = 0; i < m; ++i) {
		int from;
		int to;
		int w;
		std::cin >> from >> to >> w;
		graph.AddEdge(from - 1, to - 1, w);
	}

	std::cout << SpanningTreeWeight(graph) << std::endl;

	return 0;
}