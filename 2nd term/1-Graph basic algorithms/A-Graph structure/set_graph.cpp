#include "set_graph.h"

SetGraph::SetGraph(int count) {
	vertex_number = count;
	set.assign(count, std::unordered_set<int>());
}

SetGraph::SetGraph(const IGraph* graph) {
	vertex_number = (*graph).VerticesCount();
	set.assign(vertex_number, std::unordered_set<int>());

	for (int i = 0; i < vertex_number; ++i) {

		std::vector<int> tmp;
		GetNextVertices(i, tmp);

		for (int j = 0; j < tmp.size(); ++j) {
			set[i].insert(tmp[j]);
		}

	}
}

SetGraph::~SetGraph() {}

void SetGraph::AddEdge(int from, int to) {
	if (from >= vertex_number || to >= vertex_number) { //если подана несуществующая вершина
		return;
	}

	//доп проверка на уже существвующее ребро не нужна, так как в любом случае, в unordered_set элементы не повторяются 

	set[from].insert(to); 
}

int SetGraph::VerticesCount() const {
	return vertex_number;
}

void SetGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (auto it = set[vertex].begin(); it != set[vertex].end(); ++it) {
		vertices.push_back(*it);
	}
}

void SetGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (int i = 0; i < vertex_number; ++i) {
		if (set[i].find(vertex) != set[i].end()) { //ищем vertex в каждом
			vertices.push_back(i);
		}
	}
}