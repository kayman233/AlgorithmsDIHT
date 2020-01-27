#include "arc_graph.h"

ArcGraph::ArcGraph(int count) {
	vertex_number = count;
}

ArcGraph::ArcGraph(const IGraph* graph) {
	vertex_number = (*graph).VerticesCount();
	pairs.assign(vertex_number, std::pair<int, int>());

	for (int i = 0; i < vertex_number; ++i) {

		std::vector<int> tmp;
		GetNextVertices(i, tmp);

		for (int j = 0; tmp.size(); ++j) {
			std::pair<int,int> new_pair = std::make_pair(i, j);
			pairs.push_back(new_pair);
		}

	}
}

ArcGraph::~ArcGraph() {}

void ArcGraph::AddEdge(int from, int to) {
	if (from >= vertex_number || to >= vertex_number) { //если подана несуществующая вершина
		return;
	}

	std::pair<int, int> new_pair = std::make_pair(from, to);

	for (int i = 0; i < pairs.size(); ++i) { //проверяем, встречалась ли уже пара
		if (pairs[i] == new_pair) {
			return; 
		}
	}

	pairs.push_back(new_pair); //если нет, то добавляем
}

int ArcGraph::VerticesCount() const {
	return vertex_number;
}

void ArcGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();

	for (int i = 0; i < pairs.size(); ++i) {
		if (pairs[i].first == vertex) { //по первому полю
			vertices.push_back(pairs[i].second);
		}
	}
}

void ArcGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();

	for (int i = 0; i < pairs.size(); ++i) {
		if (pairs[i].second == vertex) { //по второму
			vertices.push_back(pairs[i].first);
		}
	}
}