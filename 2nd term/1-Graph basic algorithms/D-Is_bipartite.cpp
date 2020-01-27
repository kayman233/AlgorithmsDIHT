/*
	D. Двудольный граф
	Дан невзвешенный неориентированный граф. Определить, является ли он двудольным. Требуемая сложность O(V+E).
*/

#include <iostream>
#include <vector>
#include <list>
#include <queue>

class ListGraph {
public:
	ListGraph(int count);
	~ListGraph();

	void AddEdge(int from, int to);

	int VerticesCount() const;

	void GetVertices(int vertex, std::vector<int>& vertices) const;
private:
	unsigned int vertex_number;
	std::vector<std::list<int>> in_edges, out_edges;
};

ListGraph::ListGraph(int count) {
	vertex_number = count;

	in_edges.resize(count);
	out_edges.resize(count);
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) {
	if (from >= vertex_number || to >= vertex_number) {
		return;
	}

	for (auto it = out_edges[from].begin(); it != out_edges[from].end(); ++it) {
		if (*it == to) return;
	}

	in_edges[to].push_back(from);
	out_edges[from].push_back(to);
}

int ListGraph::VerticesCount() const {
	return vertex_number;
}

void ListGraph::GetVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (auto it = out_edges[vertex].begin(); it != out_edges[vertex].end(); ++it) {
		vertices.push_back(*it);
	}

	for (auto it = in_edges[vertex].begin(); it != in_edges[vertex].end(); ++it) {
		vertices.push_back(*it);
	}

}

bool BFS(ListGraph& graph, int vertex, std::vector<char>& used, std::vector<int>& parents) {

	std::queue<int> queue;

	queue.push(vertex);

	used[vertex] = 1;

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		std::vector<int> tmp;

		graph.GetVertices(parent, tmp);

		for (int i = 0; i < tmp.size(); ++i) {

			if (used[tmp[i]] == 0) {
				queue.push(tmp[i]);
				used[tmp[i]] = 3 - used[parent]; //обозначим, какой доле принадлежит
				parents[tmp[i]] = parent;
			} else {
				if (used[tmp[i]] == used[parent] && tmp[i] != parent) { //если вершина должна находиться в обеих долях, то не двудолен
					return 0;
				}
			}

		}
	}

	return 1;
}

bool isBipartite(ListGraph& graph) {
	bool bi = 1;

	std::vector<char> used(graph.VerticesCount(), 0);
	std::vector<int> parents(graph.VerticesCount(), 0);

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		if (used[i] == 0) {
			parents[i] = i;
			used[i] = 1;
			bi = BFS(graph, i, used, parents); //проходим в шлубину по каждой непосещенной вершине

			if (bi == 0) {
				return bi;
			}
		}
	}

	return bi;
}

int main() {

	int n = 0;
	int m = 0;

	std::cin >> n;
	std::cin >> m;

	ListGraph graph(n);

	for (int i = 0; i < m; ++i) {
		int from;
		int to;
		std::cin >> from >> to;
		graph.AddEdge(from, to);
	}

	if (isBipartite(graph) && graph.VerticesCount() > 1) {
		std::cout << "YES" << std::endl;
	} else {
		std::cout << "NO" << std::endl;
	}

	return 0;
}