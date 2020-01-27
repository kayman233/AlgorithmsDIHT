/*
	Задача 3. Максимальный поток в ориентированном графе.
	Задан ориентированный граф, каждое ребро которого обладает целочисленной пропускной способностью.
	Найдите максимальный поток из вершины с номером 1 в вершину с номером 𝑛.

	Вариант 1. С помощью алгоритма Эдмондса-Карпа.
*/


#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

#define INF 10000000

class ListGraph {
public:
	ListGraph(int count);
	~ListGraph();

	void AddEdge(int from, int to, int d);
	int VerticesCount() const;
	void GetVertices(int vertex, std::vector<int>& vertices) const;
	int GetDist(int from, int to);
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

int BFS(ListGraph& graph, std::vector<int>& augmenting_path) {
	int s = 0;
	int t = graph.VerticesCount() - 1;

	int new_flow = 0;

	augmenting_path.assign(graph.VerticesCount(), -1);

	std::queue<std::pair<int, int>> queue;
	queue.push(std::make_pair(s, INF));

	while (queue.size() > 0) {
		int vertex = queue.front().first;
		int flow = queue.front().second;

		queue.pop();

		std::vector<int> next_vert;

		graph.GetVertices(vertex, next_vert);

		for (int i = 0; i < next_vert.size(); ++i) {
			int capacity = graph.GetDist(vertex, next_vert[i]);
			if (augmenting_path[next_vert[i]] == -1 && capacity > 0) {
				augmenting_path[next_vert[i]] = vertex;

				new_flow = std::min(flow, capacity);

				if (next_vert[i] == t) {
					return new_flow;
				}

				queue.push(std::make_pair(next_vert[i], new_flow));
			}
		}

	}

	return 0;
}

void MakeResidualNetwork(ListGraph& graph, std::vector<int>& augmenting_path, int min_edge) {
	int s = 0;
	int t = graph.VerticesCount() - 1;

	int vertex = t;

	while (vertex != s) {
		int previous = augmenting_path[vertex];

		int capacity = 0;
		for (auto it = graph.edges[previous].begin(); it != graph.edges[previous].end(); ++it) {
			if ((*it).first == vertex) {
				capacity = (*it).second;
				graph.edges[previous].erase(it);
				break;
			}
		}
		graph.edges[previous].push_back(std::make_pair(vertex, capacity - min_edge));

		capacity = 0;
		for (auto it = graph.edges[vertex].begin(); it != graph.edges[vertex].end(); ++it) {
			if ((*it).first == previous) {
				capacity = (*it).second;
				graph.edges[vertex].erase(it);
				break;
			}
		}
		graph.edges[vertex].push_back(std::make_pair(previous, capacity + min_edge));

		vertex = previous;
	}
}

int MaxFlow(ListGraph& graph) {
	int max_flow = 0;
	std::vector<int> augmenting_path;

	int min_edge = BFS(graph, augmenting_path);

	while (min_edge > 0) {
		max_flow += min_edge;
		MakeResidualNetwork(graph, augmenting_path, min_edge);
		min_edge = BFS(graph, augmenting_path);
	}

	return max_flow;
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

	std::cout << MaxFlow(graph) << std::endl;

	return 0;
}