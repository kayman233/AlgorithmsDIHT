#pragma once

#include "igraph.h"

class MatrixGraph : public IGraph {
public:
	MatrixGraph(int count);
	MatrixGraph(const IGraph* graph);
	virtual ~MatrixGraph();

	virtual void AddEdge(int from, int to);

	virtual int VerticesCount() const;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
	unsigned int vertex_number;
	std::vector<std::vector<bool>> matrix;
};