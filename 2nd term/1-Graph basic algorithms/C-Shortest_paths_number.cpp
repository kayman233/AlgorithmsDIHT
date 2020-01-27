/*
	C. Количество различных путей
	Дан невзвешенный неориентированный граф.
	В графе может быть несколько кратчайших путей между какими-то вершинами.
	Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
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

int GetLength(ListGraph& graph, int from, int to) {
	std::vector<bool> visited(graph.VerticesCount());
	visited.assign(graph.VerticesCount(), 0);
	visited[from] = 1;

	std::queue<int> queue;
	queue.push(from);

	std::vector<int> distances(graph.VerticesCount(), 0), parents(graph.VerticesCount(), 0);

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		std::vector<int> tmp;
		graph.GetVertices(parent, tmp);

		for (int i = 0; i < tmp.size(); ++i) {
			if (visited[tmp[i]] == 0) {
				queue.push(tmp[i]);
				distances[tmp[i]] = distances[parent] + 1;
				visited[tmp[i]] = 1;
				parents[tmp[i]] = parent;
			}

			if (tmp[i] == to) { //как только дошли до нужной вершины, возвращаем длину
				return distances[to];
			}
		}
	}
}

int PathsNumber(ListGraph& graph, int from, int to) {
	int min_len = -1;
	int num = 0;
	int length = GetLength(graph, from, to); //сначала находим кратчайшую длину с помощью bfs

	std::vector<bool> visited(graph.VerticesCount(), 0);

	std::queue<int> queue;
	queue.push(from);

	visited[from] = 1;

	std::vector<int> distances(graph.VerticesCount(), 0);
	std::vector<int> parents(graph.VerticesCount(), 0);
	std::vector<int> count(graph.VerticesCount(), 0); //в каждой вершине храним кол-во способов до нее добраться

	count[from] = 1;

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		std::vector<int> tmp;

		graph.GetVertices(parent, tmp);

		for (int i = 0; i < tmp.size(); ++i) {
			if (visited[tmp[i]] == 0) { //если вершина не посещена
				queue.push(tmp[i]);

				distances[tmp[i]] = distances[parent] + 1;
				visited[tmp[i]] = 1;
				parents[tmp[i]] = parent;

				if (distances[tmp[i]] <= length) { //для вершин, расстояние до которых уже больше нужной длины, не рассматриваем
					std::vector<int> neighbours;
					graph.GetVertices(tmp[i], neighbours);

					for (int j = 0; j < neighbours.size(); ++j) {
						if (distances[neighbours[j]] < distances[tmp[i]]) {
							count[tmp[i]] += count[neighbours[j]]; //обновляем кол-во спобосов добраться
						}
					}
				}
			}

			if (tmp[i] == to) {
				return count[to]; //если дошли до необходимой, то возвращаем ответ
			}
		}
	}

	return 0;
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

	int from = 0;
	int to = 0;

	std::cin >> from;
	std::cin >> to;

	std::cout << PathsNumber(graph, from, to) << std::endl;

	return 0;
}