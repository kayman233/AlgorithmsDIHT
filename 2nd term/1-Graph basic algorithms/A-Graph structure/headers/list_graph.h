#pragma once

#include "igraph.h"
#include <list>

class ListGraph : public IGraph {
public:
	ListGraph(int count);
	ListGraph(const IGraph* graph);
	virtual ~ListGraph();

	virtual void AddEdge(int from, int to);

	virtual int VerticesCount() const;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
	unsigned int vertex_number;
	std::vector<std::list<int>> in_edges, out_edges;
};