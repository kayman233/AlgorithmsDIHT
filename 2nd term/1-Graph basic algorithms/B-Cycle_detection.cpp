/*
	B. Цикл минимальной длины
	Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
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

int FindLoop(ListGraph& graph, int vertex) {
	std::vector<bool> visited(graph.VerticesCount(), 0);

	std::queue<int> queue;
	queue.push(vertex);

	visited[vertex] = 1;

	std::vector<int> distances(graph.VerticesCount(), 0); //для хранения расстояния
	std::vector<int> parents(graph.VerticesCount(), 0); //для хранения родителей


	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		std::vector<int> tmp;
		graph.GetVertices(parent, tmp);

		for (int i = 0; i < tmp.size(); ++i) {
			if (visited[tmp[i]] == 0) {
				queue.push(tmp[i]);
				distances[tmp[i]] = distances[parent] + 1;	//добавляем расстояние от искомой вершины, обновляем данные о том
				visited[tmp[i]] = 1;						//что уже посетили, а также родителя
				parents[tmp[i]] = parent;
			} else {
				if (visited[tmp[i]] == 1 && tmp[i] != parents[parent]) {
					return distances[tmp[i]] + distances[parent] + 1; //как только встречаем уже посещенный, выходим, возвращаем результат
				}
			}
		}
	}

	return -1;
}

int MinLoop(ListGraph& graph) {

	int min_len = -1;

	for (int i = 0; i < graph.VerticesCount(); ++i) { //запускаем bfs для  каждой вершины
		int loop_length = FindLoop(graph, i);
		if ((loop_length != -1) and ((loop_length < min_len) or (min_len == -1))) { //условие, что данная длина цикла минимальная
			min_len = loop_length;
		}
	}

	return min_len;
}

int main() {

	int n = 0;
	int m = 0;

	std::cin >> n >> m;

	ListGraph graph(n);

	for (int i = 0; i < m; ++i) {
		int from;
		int to;
		std::cin >> from >> to;
		graph.AddEdge(from, to);
	}

	std::cout << MinLoop(graph) << std::endl; //находим длину кратчайшего цикла

	return 0;
}