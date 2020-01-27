/*
Задача 2а). Приближенное решение метрической неориентированной задачи коммивояжера.
Задачи в контесте нет.
Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости)
с помощью минимального остовного дерева, построенного в первой задаче.
*/

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <set>
#include <stack>

#define INF 10000000

class ListGraph {
public:
	ListGraph(int count);
	~ListGraph();

	void AddEdge(int from, int to, int d);
	int VerticesCount() const;
	void GetVertices(int vertex, std::vector<int>& vertices) const;
	int GetDist(int from, int to);

private:
	unsigned int VertNum;
	std::vector<std::list<std::pair<int, int>>> Edges;
};

ListGraph::ListGraph(int count) {
	VertNum = count;

	Edges.resize(count);
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to, int d) {
	if (from >= VertNum || to >= VertNum) {
		return;
	}

	bool add_reversed = 0;

	for (auto it = Edges[from].begin(); it != Edges[from].end(); ++it) {
		if ((*it).first == to) {
			if ((*it).second > d) {
				Edges[from].erase(it);
				Edges[from].push_back(std::make_pair(to, d));
			}
			add_reversed = 1;
			break;
		}
	}

	if (add_reversed) {
		for (auto it = Edges[to].begin(); it != Edges[to].end(); ++it) {
			if ((*it).first == from) {
				if ((*it).second > d) {
					Edges[to].erase(it);
					Edges[to].push_back(std::make_pair(from, d));
				}
				return;
			}
		}
	}

	Edges[from].push_back(std::make_pair(to, d));
	Edges[to].push_back(std::make_pair(from, d));
}

int ListGraph::VerticesCount() const {
	return VertNum;
}

void ListGraph::GetVertices(int vertex, std::vector<int>& vertices) const {
	vertices.clear();
	for (auto it = Edges[vertex].begin(); it != Edges[vertex].end(); ++it) {
		vertices.push_back((*it).first);
	}
}

int ListGraph::GetDist(int from, int to) {
	for (auto it = Edges[from].begin(); it != Edges[from].end(); ++it) {
		if ((*it).first == to) {
			return (*it).second;
		}
	}

	if (from == to) {
		return 0;
	}

	return INF;
}

int SpanningTreeWeight(ListGraph& graph, std::vector<std::pair<int, int>>& edges) {
	int weight = 0;
	std::set<std::pair<int, int>> dist_set;

	std::vector<int> distances(graph.VerticesCount(), INF);
	std::vector<int> parents(graph.VerticesCount(), -1);

	distances[0] = 0;
	dist_set.insert(std::make_pair(0, 0));

	for (int i = 1; i < graph.VerticesCount(); ++i) {
		dist_set.insert(std::make_pair(INF, i));
	}
	parents[0] = 0;

	while (dist_set.size() > 0) {
		std::pair<int, int> min = *(dist_set.begin());

		dist_set.erase(dist_set.begin());

		int vertex = min.second;

		std::vector<int> tmp;
		graph.GetVertices(vertex, tmp);

		for (int i = 0; i < tmp.size(); ++i) {

			int v = tmp[i];
			int w = graph.GetDist(vertex, v);

			if (distances[v] > w) {
				if (dist_set.find(std::make_pair(distances[v], v)) != dist_set.end()) {
					dist_set.erase(std::make_pair(distances[v], v));
					dist_set.insert(std::make_pair(w, v));

					distances[v] = w;
					parents[v] = vertex;
				}
			}
		}
	}

	for (int i = 1; i < graph.VerticesCount(); ++i) {
		edges.push_back(std::make_pair(i, parents[i]));
	}

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		weight += graph.GetDist(i, parents[i]);
	}
	return weight;
}

void DFS(ListGraph& graph, std::vector<std::pair<int, int>>& answer) {
	answer.resize(0);
	std::vector<bool> visited(graph.VerticesCount(), false);

	std::stack<int> stack;
	stack.push(0);
	visited[0] = true;

	int previous = 0;
	int count = 0;

	while (!stack.empty()) {
		int v = stack.top();
		stack.pop();

		if (!visited[v]) {
			answer.push_back(std::make_pair(previous, v));
			previous = v;
			++count;
			if (count == graph.VerticesCount() - 1) {
				answer.push_back(std::make_pair(v, 0));
			}

			visited[v] = 1;
		}

		std::vector<int> next;

		graph.GetVertices(v, next);

		for (int i = 0; i < next.size(); ++i) {
			if (!visited[next[i]]) {
				stack.push(next[i]);
			}
		}

	}
}

void get_answer(ListGraph& graph, std::vector<std::pair<int, int>>& answer) {

	std::vector<std::pair<int, int>> edges;
	SpanningTreeWeight(graph, edges); //строим мин остов - дерево
	ListGraph* tree = new ListGraph(graph.VerticesCount());

	for (int i = 0; i < edges.size(); ++i) {
		std::pair<int, int> edge = edges[i];
		tree->AddEdge(edge.second, edge.first,
			graph.GetDist(edge.second, edge.first));
	}

	DFS(*tree, answer); // проходимся dfs-ом по остову, записывая путь - ответ
}

int main() {

	int n = 0;
	int m = 0;

	std::cin >> n >> m;

	ListGraph* graph = new ListGraph(n);

	for (int i = 0; i < m; ++i) {
		int from;
		int to;
		int w;
		std::cin >> from >> to >> w;
		graph->AddEdge(from - 1, to - 1, w);
	}

	std::vector<std::pair<int, int>> answer;
	get_answer(*graph, answer);

	for (int i = 0; i < answer.size(); ++i) {
		std::cout << answer[i].first << " " << answer[i].second << " " << graph->GetDist(answer[i].first, answer[i].second) << std::endl;
	}

	return 0;
}