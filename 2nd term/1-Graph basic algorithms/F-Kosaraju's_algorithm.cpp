/*
	Дан ориентированный граф.
	Определите, какое минимальное число ребер необходимо добавить, чтобы граф стал сильносвязным.
	В графе возможны петли.
*/

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

class ListGraph {
public:
	ListGraph(int count);
	virtual ~ListGraph();

	virtual void AddEdge(int from, int to);

	virtual int VerticesCount() const;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;

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

void ReverseEdges(ListGraph& graph, ListGraph& reversed_graph) {

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> tmp;
		graph.GetNextVertices(i, tmp);

		for (int j = 0; j < tmp.size(); ++j) {
			reversed_graph.AddEdge(tmp[j], i);
		}

		tmp.clear();
		graph.GetPrevVertices(i, tmp);

		for (int j = 0; j < tmp.size(); ++j) {
			reversed_graph.AddEdge(i, tmp[j]);
		}
	}

}

void TopologicalSort(ListGraph& graph, int vertex, std::vector<char>& used, std::vector<int>& time_out) {
	used[vertex] = 1;

	std::vector<int> tmp;
	graph.GetNextVertices(vertex, tmp);

	for (int i = 0; i < tmp.size(); ++i) {
		if (!used[tmp[i]]) {
			TopologicalSort(graph, tmp[i], used, time_out);
		}
	}

	time_out.push_back(vertex);
}

void DFS(ListGraph& graph, int vertex, std::vector<char>& used, int components_num, std::vector<int>& which_comp) {
	used[vertex] = 1;
	which_comp[vertex] = components_num;

	std::vector<int> tmp;
	graph.GetNextVertices(vertex, tmp);

	for (int i = 0; i < tmp.size(); ++i) {
		if (!used[tmp[i]]) {
			DFS(graph, tmp[i], used, components_num, which_comp);
		}
	}
}

int VertexNumToConnective(ListGraph& graph) {
	int num = 0;
	std::vector<char> used(graph.VerticesCount(), 0);
	std::vector<int> time_out;

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		if (!used[i]) {
			TopologicalSort(graph, i, used, time_out);
		}
	}

	std::reverse(time_out.begin(), time_out.end());

	ListGraph* reversed_graph = new ListGraph(graph.VerticesCount()); //алгоритм Касарайю

	ReverseEdges(graph, *reversed_graph);

	used.assign(graph.VerticesCount(), 0);

	std::vector<int> which_comp((*reversed_graph).VerticesCount(), 0);
	int components_num = 0;

	for (int i = 0; i < time_out.size(); ++i) {
		if (!used[time_out[i]]) {
			DFS((*reversed_graph), time_out[i], used, components_num, which_comp);
			++components_num;
		}
	}

	ListGraph* condensated_graph = new ListGraph(components_num); //получаем сконденсированный граф

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> tmp;
		graph.GetNextVertices(i, tmp);

		for (int j = 0; j < tmp.size(); ++j) {
			if (which_comp[i] != which_comp[tmp[j]]) {
				condensated_graph->AddEdge(which_comp[i], which_comp[tmp[j]]);
			}
		}
	}

	int sink_num = 0; //считаем кол-во истоков и стоков
	int source_num = 0;

	if (condensated_graph->VerticesCount() == 1) {
		return 0;
	}

	for (int i = 0; i < condensated_graph->VerticesCount(); ++i) {
		std::vector<int> tmp;
		condensated_graph->GetNextVertices(i, tmp);

		if (tmp.size() == 0) {
			++sink_num;
		}

		tmp.clear();
		condensated_graph->GetPrevVertices(i, tmp);

		if (tmp.size() == 0) {
			++source_num;
		}

	}

	return std::max(sink_num, source_num);
}

int main() {

	int n = 0;
	int m = 0;

	std::cin >> n >> m;

	ListGraph graph = ListGraph(n);

	for (int i = 0; i < m; ++i) {
		int from;
		int to;
		std::cin >> from >> to;
		graph.AddEdge(from - 1, to - 1);
	}

	std::cout << VertexNumToConnective(graph) << std::endl;

	return 0;
}