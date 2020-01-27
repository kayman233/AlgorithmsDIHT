#pragma once

#include "igraph.h"
#include <unordered_set>

class SetGraph : public IGraph {
public:
	SetGraph(int count);
	SetGraph(const IGraph* graph);
	virtual ~SetGraph();

	virtual void AddEdge(int from, int to);

	virtual int VerticesCount() const;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
	unsigned int vertex_number;
	std::vector<std::unordered_set<int>> set;
};