/*
	Необходимо написать торгового советника для поиска арбитража.

	Арбитраж - это торговля по цепочке различных валют в надежде заработать на небольших различиях в коэффициентах.
	Например, есть следующие курсы валют:
	GBP/USD: 0.67
	RUB/GBP: 78.66
	USD/RUB: 0.02

	Имея 1$ и совершив цикл USD->GBP->RUB->USD, получим 1.054$. Таким образом заработав 5.4 
*/

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

#define INF -1

class ListGraph {
public:
	ListGraph(int count);
	~ListGraph();

	void AddEdge(int from, int to, double d);
	int VerticesCount() const;
	void GetVertices(int vertex, std::vector<int>& vertices) const;
	double GetDist(int from, int to);
private:
	unsigned int vertex_number;
	std::vector<std::list<std::pair<int, double>>> edges;
};

ListGraph::ListGraph(int count) {
	vertex_number = count;

	edges.resize(count);
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to, double d) {
	if (from >= vertex_number || to >= vertex_number) {
		return;
	}

	for (auto it = edges[from].begin(); it != edges[from].end(); ++it) {
		if ((*it).first == to) {
			if ((*it).second > d) {
				edges[from].erase(it);
				edges[from].push_back(std::make_pair(to, d));
			}
			break;
		}
	}

	edges[from].push_back(std::make_pair(to, d));
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

double ListGraph::GetDist(int from, int to) {
	for (auto it = edges[from].begin(); it != edges[from].end(); ++it) {
		if ((*it).first == to) {
			return (*it).second;
		}
	}

	return INF;
}

//Ford–Fulkerson algorithm
bool hasLoop(ListGraph& graph) {
	std::vector<double> d(graph.VerticesCount());

	for (int v = 0; v < graph.VerticesCount(); ++v) {
		d[v] = INF;
	}
	d[0] = 1;

	for (int i = 0; i < graph.VerticesCount() - 1; ++i) {
		for (int u = 0; u < graph.VerticesCount(); ++u) {
			for (int v = 0; v < graph.VerticesCount(); ++v) {
				double dist = graph.GetDist(u, v);
				if (dist != INF) {
					if (d[v] != INF) {
						if (d[u] != INF) {
							d[v] = std::max(d[v], d[u] * dist);
						}
					} else {
						if (d[u] != INF) {
							d[v] = d[u] * dist;
						}
					}
				}
			}
		}
	}

	for (int u = 0; u < graph.VerticesCount(); ++u) {
		for (int v = 0; v < graph.VerticesCount(); ++v) {
			double dist = graph.GetDist(u, v);

			if (dist != INF) {
				if (d[v] != INF) {
					if (d[u] != INF) {
						if (d[v] < d[u] * dist) {
							return 1;
						}
					}
				} else {
					if (d[u] != INF) {
						if (d[v] < d[u] * dist) {
							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}

int main() {
	int n = 0;

	std::cin >> n;

	ListGraph graph(n);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				double dist = 0;

				std::cin >> dist;
				if (dist != -1) {
					graph.AddEdge(i, j, dist);
				}
			} else {
				graph.AddEdge(i, i, 1);
			}
		}
	}
	
	if (hasLoop(graph)) {
		std::cout << "YES" << std::endl;
	} else {
		std::cout << "NO" << std::endl;
	}

	return 0;
}