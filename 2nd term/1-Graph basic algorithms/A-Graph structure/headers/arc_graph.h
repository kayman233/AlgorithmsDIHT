#pragma once

#include "igraph.h"
#include <map>

class ArcGraph : public IGraph {
public:
	ArcGraph(int count);
	ArcGraph(const IGraph* graph);
	virtual ~ArcGraph();

	virtual void AddEdge(int from, int to);

	virtual int VerticesCount() const;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
	unsigned int vertex_number;
	std::vector<std::pair<int, int>> pairs;
};