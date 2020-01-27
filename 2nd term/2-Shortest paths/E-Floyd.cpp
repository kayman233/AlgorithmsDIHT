/*
	Задача 5. Алгоритм Флойда

	Полный ориентированный взвешенный граф задан матрицей смежности.
	Постройте матрицу кратчайших путей между его вершинами.
	Гарантируется, что в графе нет циклов отрицательного веса.
*/

#include <iostream>
#include <vector>
#include <algorithm>

#define inf 101
class MatrixGraph {
public:
	MatrixGraph(int count);
	~MatrixGraph();

	void AddEdge(int from, int to, int d);

	unsigned int vertex_number;
	std::vector<std::vector<int>> matrix;
};

MatrixGraph::MatrixGraph(int count) {
	vertex_number = count;
	matrix.assign(count, std::vector<int>(count, 0));
}

MatrixGraph::~MatrixGraph() {}

void MatrixGraph::AddEdge(int from, int to, int d) {
	if (from >= vertex_number || to >= vertex_number) {
		return;
	}

	matrix[from][to] = d;
}

int Floyd(MatrixGraph& graph) {
	for (int k = 0; k < graph.vertex_number; ++k)
		for (int i = 0; i < graph.vertex_number; ++i)
			for (int j = 0; j < graph.vertex_number; ++j)
				if (graph.matrix[i][k] < inf && graph.matrix[k][j] < inf)
					graph.matrix[i][j] = 
						std::min(graph.matrix[i][j], graph.matrix[i][k] + graph.matrix[k][j]);
}

int main() {

	int n = 0;
	std::cin >> n;

	MatrixGraph graph(n);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			int d = 0;
			std::cin >> d;
			graph.AddEdge(i, j, d);
		}
	}

	Floyd(graph);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << graph.matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}