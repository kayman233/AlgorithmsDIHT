/*
	Задача 3. LCA
	Задано дерево с корнем, содержащее 𝑛 (1 ≤ 𝑛 ≤ 100 000) вершин, пронумерованных от 0 до 𝑛−1.
	Требуется ответить на 𝑚 (1 ≤ 𝑚 ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин. 
	Запросы генерируются следующим образом. Заданы числа 𝑎1, 𝑎2 и числа 𝑥, 𝑦 и 𝑧. 
	Числа 𝑎3, . . . , 𝑎2𝑚 генерируются следующим образом: 𝑎𝑖 = (𝑥·𝑎𝑖−2+𝑦·𝑎𝑖−1+𝑧) mod 𝑛.
	Первый запрос имеет вид ⟨𝑎1, 𝑎2⟩. Если ответ на 𝑖−1-й запрос равен 𝑣, то 𝑖-й запрос имеет вид ⟨(𝑎2𝑖−1 + 𝑣) mod 𝑛, 𝑎2𝑖⟩.
	Для решения задачи можно использовать метод двоичного подъёма.
*/


#include <iostream>
#include <vector>

class TreeLca {
public:
	TreeLca(std::vector<int>& vertices);
	int LogDepth();

	void AddEdgesAndDp(std::vector<int>& vertices);
	void DFS(int vertex);
	void CountDP();

	int FindLCA(int first, int second);
private:
	std::vector<std::vector<int>> dp;

	std::vector<int> d; // глубины
	std::vector<std::vector<int>> edges;

	int tree_size;
	int tree_depth;
	int log_depth;
};

TreeLca::TreeLca(std::vector<int>& vertices) {
	tree_size = vertices.size() + 1;

	edges.resize(tree_size);
	dp.resize(tree_size);
	for (int i = 0; i < tree_size; ++i) {
		dp[i].resize(1);
	}
	d.resize(tree_size);

	AddEdgesAndDp(vertices);
	DFS(0);
	log_depth = LogDepth();
	CountDP();
}

void TreeLca::AddEdgesAndDp(std::vector<int>& vertices) {
	dp[0][0] = 0;

	for (int i = 1; i < tree_size; ++i) {
		dp[i][0] = vertices[i - 1];
		edges[dp[i][0]].push_back(i);
	}
}

void TreeLca::DFS(int vertex) {
	if (vertex == 0) {
		d[vertex] = 0;
	} else {
		d[vertex] = d[dp[vertex][0]] + 1;

		if (tree_depth < d[vertex]) {
			tree_depth = d[vertex];
		}
	}

	for (auto it = edges[vertex].begin(); it != edges[vertex].end(); ++it) {
		DFS(*it);
	}
}

int TreeLca::LogDepth() {
	int log_value = 0;

	for (int i = 0; i < tree_size; ++i) {

		if (i >= 1 << (log_value + 1)) {
			++log_value;
		}
	}

	return log_value;
}

void TreeLca::CountDP() {
	for (auto it = dp.begin(); it != dp.end(); ++it) {
		(*it).resize(log_depth + 2);
	}

	for (int i = 1; i < dp[0].size(); ++i) {
		for (int v = 0; v < d.size(); ++v) {
			dp[v][i] = dp[ dp[v][i - 1] ][i - 1];
		}
	}
}

// с помощью двоичного подъема
int TreeLca::FindLCA(int first, int second) {

	if (d[first] < d[second]) {
		std::swap(first, second);
	}

	int diff = d[first] - d[second];
	int k = 0;

	// находим предка второго, который лежит на одном урвоне с первым
	while (d[first] > d[second]) {
		if (diff % 2 == 1) {
			first = dp[first][k];
		}
		++k;
		diff /= 2;
	}

	// если первый оказался предком
	if (first == second) {
		return first;
	}

	// иначе ищем ближайшего общего предка для них 
	for (int i = log_depth + 1; i >= 0; --i) {
		if (dp[first][i] != dp[second][i]) {
			first = dp[first][i];
			second = dp[second][i];
		}
	}

	return dp[first][0];
}

int main() {
	int n = 0;
	long long m = 0;

	std::cin >> n >> m;

	std::vector<int> vertices;

	for (int i = 0; i < n - 1; ++i) {
		int vert = 0;
		std::cin >> vert;
		vertices.push_back(vert);
	}

	TreeLca tree(vertices);

	int a1 = 0;
	int a2 = 0;

	std::cin >> a1 >> a2;

	long long x = 0;
	long long y = 0;
	long long z = 0;

	std::cin >> x >> y >> z;

	int answer_sum = 0;
	int last_answer = 0;

	for (int i = 0; i < m; ++i) {
		last_answer = tree.FindLCA((a1 + last_answer) % n, a2);
		
		a1 = (a1 * x + a2 * y + z) % n;
		a2 = (a2 * x + a1 * y + z) % n;

		answer_sum += last_answer;
	}

	std::cout << answer_sum << std::endl;

	return 0;
}