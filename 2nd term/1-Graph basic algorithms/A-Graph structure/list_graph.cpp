#include "list_graph.h"

ListGraph::ListGraph(int count) {
	vertex_number = count;

	in_edges.resize(count);
	out_edges.resize(count);
}

ListGraph::ListGraph(const IGraph* graph) {
	vertex_number = (*graph).VerticesCount(); //кол-во вершин
	in_edges.resize(vertex_number);
	out_edges.resize(vertex_number);

	for (int i = 0; i < vertex_number; ++i) { //добавление ребер
		std::vector<int> tmp;
		GetNextVertices(i, tmp);
		for (int j = 0; j < tmp.size(); ++j) {
			in_edges[i].push_back(tmp[j]);
		}

		tmp.clear();

		GetPrevVertices(i, tmp);
		for (int j = 0; j < tmp.size(); ++j) {
			out_edges[i].push_back(tmp[j]);
		}
	}
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) { //добавление ребра
	if (from >= vertex_number || to >= vertex_number) { //если подана несуществующая вершина
		return;
	}

	for (auto it = out_edges[from].begin(); it != out_edges[from].end(); ++it) { //проверяем на наличие
		if (*it == to) return;
	}

	in_edges[to].push_back(from); //добавляем ребро
	out_edges[from].push_back(to);
}

int ListGraph::VerticesCount() const {
	return vertex_number;
}

void ListGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (auto it = out_edges[vertex].begin(); it != out_edges[vertex].end(); ++it) {
		vertices.push_back(*it);
	}
}

void ListGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (auto it = in_edges[vertex].begin(); it != in_edges[vertex].end(); ++it) {
		vertices.push_back(*it);
	}
}