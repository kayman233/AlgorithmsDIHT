/*
Задача 4. Чай. (5 балла)

Задача в контесте - C.
В одном из отделов крупной организации работает n человек. Как практически все сотрудники этой организации,
они любят пить чай в перерывах между работой. При этом они достаточно дисциплинированы и делают в день ровно один перерыв,
во время которого пьют чай. Для того, чтобы этот перерыв был максимально приятным,
каждый из сотрудников этого отдела обязательно пьет чай одного из своих любимых сортов.
В разные дни сотрудник может пить чай разных сортов. Для удобства пронумеруем сорта чая числами от 1 до m.
Недавно сотрудники отдела купили себе большой набор чайных пакетиков, который содержит a1 пакетиков чая сорта номер 1,
a2 пакетиков чая сорта номер 2, ..., am пакетиков чая сорта номер m. Теперь они хотят знать,
на какое максимальное число дней им может хватить купленного набора так,
чтобы в каждый из дней каждому из сотрудников доставался пакетик чая одного из его любимых сортов.
Каждый сотрудник отдела пьет в день ровно одну чашку чая, которую заваривает из одного пакетика. При этом пакетики чая не завариваются повторно.
Входные данные
Первая строка содержит два целых числа n и m (1 ≤ n, m ≤ 50). Вторая строка содержит m целых чисел a1, ..., am (1 ≤ ai ≤ 106 для всех i от 1 до m).
Далее следуют n строк — i-я из этих строк описывает любимые сорта i-го сотрудника отдела
и имеет следующий формат: сначала следует положительное число ki — количество любимых сортов чая этого сотрудника,
а затем идут ki различных чисел от 1 до m — номера этих сортов.

*/


#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

#define INF 10000000

class ListGraph {
public:
	ListGraph(int count);
	ListGraph(ListGraph* graph);
	~ListGraph();

	void AddEdge(int from, int to, int d);
	int VerticesCount() const;
	void GetVertices(int vertex, std::vector<int>& vertices) const;
	int GetDist(int from, int to);
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


	for (auto it = Edges[from].begin(); it != Edges[from].end(); ++it) {
		if ((*it).first == to) {
			if ((*it).second > d) {
				Edges[from].erase(it);
				Edges[from].push_back(std::make_pair(to, d));
			}
			break;
		}
	}

	Edges[from].push_back(std::make_pair(to, d));
}

ListGraph::ListGraph(ListGraph* graph) {
	VertNum = (*graph).VerticesCount();
	Edges.resize(VertNum);

	for (int i = 0; i < VertNum; ++i) {
		std::vector<int> tmp;
		graph->GetVertices(i, tmp);
		for (int j = 0; j < tmp.size(); ++j) {
			Edges[i].push_back(std::make_pair(tmp[j], graph->GetDist(i, tmp[j])));
		}
	}
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

int bfs(ListGraph& graph, std::vector<int>& augmenting_path) {
	int s = 0;
	int t = graph.VerticesCount() - 1;

	int new_flow = 0;

	augmenting_path.assign(graph.VerticesCount(), -1);

	std::queue<std::pair<int, int>> queue;

	queue.push(std::make_pair(s, INF));

	while (queue.size() > 0) {
		int vertex = queue.front().first;
		int flow = queue.front().second;

		queue.pop();

		std::vector<int> next_vert;

		graph.GetVertices(vertex, next_vert);

		for (int i = 0; i < next_vert.size(); ++i) {
			int capacity = graph.GetDist(vertex, next_vert[i]);
			if (augmenting_path[next_vert[i]] == -1 && capacity > 0) {
				augmenting_path[next_vert[i]] = vertex;

				new_flow = std::min(flow, capacity);

				if (next_vert[i] == t) {
					return new_flow;
				}

				queue.push(std::make_pair(next_vert[i], new_flow));
			}
		}

	}

	return 0;
}

void make_resudial_network(ListGraph& graph, std::vector<int>& augmenting_path, int min_edge) {
	int s = 0;
	int t = graph.VerticesCount() - 1;

	int vertex = t;

	while (vertex != s) {
		int previous = augmenting_path[vertex];

		int capacity = 0;
		for (auto it = graph.Edges[previous].begin(); it != graph.Edges[previous].end(); ++it) {
			if ((*it).first == vertex) {
				capacity = (*it).second;
				graph.Edges[previous].erase(it);
				break;
			}
		}
		graph.Edges[previous].push_back(std::make_pair(vertex, capacity - min_edge));

		capacity = 0;
		for (auto it = graph.Edges[vertex].begin(); it != graph.Edges[vertex].end(); ++it) {
			if ((*it).first == previous) {
				capacity = (*it).second;
				graph.Edges[vertex].erase(it);
				break;
			}
		}
		graph.Edges[vertex].push_back(std::make_pair(previous, capacity + min_edge));

		vertex = previous;
	}
}

int MaxFlow(ListGraph graph) {
	int max_flow = 0;
	std::vector<int> augmenting_path;

	int min_edge = bfs(graph, augmenting_path);

	while (min_edge > 0) {

		max_flow += min_edge;

		make_resudial_network(graph, augmenting_path, min_edge);

		min_edge = bfs(graph, augmenting_path);
	}

	return max_flow;
}

void rebuild_graph(ListGraph& graph, int n, int value) {
	for (int i = 0; i <= n; ++i) {
		std::list<std::pair<int, int>> additional_list;
		for (auto it = graph.Edges[i].begin(); it != graph.Edges[i].end(); ++it) {
			int vertex = (*it).first;
			additional_list.push_back(std::make_pair(vertex, value));
		}
		graph.Edges[i].resize(0);
		for (auto it = additional_list.begin(); it != additional_list.end(); ++it) {
			graph.Edges[i].push_back((*it));
		}
	}

}

int find_answer(ListGraph& graph, int n, int l, int r, int mid) {
	while (l < r - 1) { //с помощью бинпоиска

		int flow = MaxFlow(graph);
		if (flow < n * mid) {
			r = mid;
		}
		else {
			l = mid;
		}
		mid = (l + r) / 2;

		rebuild_graph(graph, n, mid); // ищем нужный вес, перестраивая граф
	}

	return l;
}

int main() {

	int n = 0;
	int m = 0;

	std::cin >> n >> m;

	int total_num = n + m + 2;
	int total_packs = 0;
	ListGraph* graph = new ListGraph(total_num);

	for (int i = 0; i < m; ++i) {
		int num_pack = 0;
		std::cin >> num_pack;
		graph->AddEdge(total_num - (m - i) - 1, total_num - 1, num_pack);
		total_packs += num_pack;
	}

	int l = 0;
	int r = total_packs + 1; // дней точно не больше кол-ва пакетиков чая
	int mid = (l + r) / 2;

	for (int i = 0; i < n; ++i) {
		int num_of_favorite = 0;
		std::cin >> num_of_favorite;
		for (int j = 0; j < num_of_favorite; ++j) {
			int tea = 0;
			std::cin >> tea;
			graph->AddEdge(i + 1, tea + n, mid);
		}
		graph->AddEdge(0, i + 1, mid);
	}

	std::cout << find_answer(*graph,n, l, r, mid) << std::endl;
	return 0;
}