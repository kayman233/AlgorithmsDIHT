#include "matrix_graph.h"

MatrixGraph::MatrixGraph(int count) {
	vertex_number = count;
	matrix.assign(count, std::vector<bool>(count, 0));
}

MatrixGraph::MatrixGraph(const IGraph* graph) {
	vertex_number = (*graph).VerticesCount();
	matrix.assign(vertex_number, std::vector<bool>(vertex_number, 0)); //изначально пусто

	for (int i = 0; i < vertex_number; ++i) {

		std::vector<int> tmp;
		GetNextVertices(i, tmp);

		for (int j = 0; j < tmp.size(); ++j) {
			matrix[i][tmp[j]] = 1; //1 - если есть вершина из i tmp[j]
		}
	}
}

MatrixGraph::~MatrixGraph() {}

void MatrixGraph::AddEdge(int from, int to) {
	if (from >= vertex_number || to >= vertex_number) { //если подана несуществующая вершина
		return;
	}

	matrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
	return vertex_number;
}

void MatrixGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (int i = 0; i < vertex_number; ++i) {
		if (matrix[vertex][i] == 1) { //смотрим по строке
			vertices.push_back(i);
		}
	}
}

void MatrixGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (int i = 0; i < vertex_number; ++i) {
		if (matrix[i][vertex] == 1) { //по столбцу
			vertices.push_back(i);
		}
	}
}