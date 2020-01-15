#include "utils.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/* Generate graph from DIMACS file */
bool read_graph(string input_path, Graph& graph) {
	ifstream input(input_path);
	if (!input.is_open()) {
		return false;
	}

	string line;
	while (getline(input, line)) {
		istringstream iss(line);

		char key;
		string name;
		string token;
		int num_nodes = 0;
		int num_edges = 0;
		int u, v;

		iss >> key;
		switch (key) {
			case 'c': /* comment line */
				iss >> name;
				while (iss >> token) {
					name += ("_" + token);
				}
				graph.set_name(name);
				break;
			case 'p': /* problem line */
				iss >> token >> num_nodes >> num_edges;
				graph.init(num_nodes);
				break;
			case 'e': /* edge */
				iss >> u >> v;
				graph.add_edge(u-1, v-1);
				break;
		}
	}

	input.close();

	return true;
}

bool swap(Graph& graph) {
	srand(time(nullptr));

	Partition* pi = graph.pi;
	vector<int>& nodes = pi->nodes;
	vector<vector<int>>& adj_list = pi->adj_list;

	vector<Cell*> cells;
	for (Cell& cell: pi->cells) {
		cells.emplace_back(&cell);
	}

	random_shuffle(cells.begin(), cells.end());

	pi->refine();
	for (const Cell* cell: cells) {
		vector<pair<int, int>> edges;
		for (int i = cell->begin; i < cell->end; ++i) {
			for (int j = i+1; j < cell->end; ++j) {
				int u = nodes[i];
				int v = nodes[j];
				if (find(adj_list[u].begin(), adj_list[u].end(), v) != adj_list[u].end()) {
					edges.emplace_back(u, v);
				}
			}
		}

		if (edges.empty()) {
			continue;
		}

		for (int _ = 0; _ < 100; ++_) {
			int i = rand() % edges.size();
			int j = rand() % edges.size();

			int a = edges[i].first;
			int b = edges[i].second;
			int x = edges[j].first;
			int y = edges[j].second;

			if (find(adj_list[a].begin(), adj_list[a].end(), y) == adj_list[a].end() && find(adj_list[x].begin(), adj_list[x].end(), b) == adj_list[x].end()) {
				replace(adj_list[a].begin(), adj_list[a].end(), b, y);
				replace(adj_list[b].begin(), adj_list[b].end(), a, x);
				replace(adj_list[x].begin(), adj_list[x].end(), y, b);
				replace(adj_list[y].begin(), adj_list[y].end(), x, a);

				sort(adj_list[a].begin(), adj_list[a].end());
				sort(adj_list[b].begin(), adj_list[b].end());
				sort(adj_list[x].begin(), adj_list[x].end());
				sort(adj_list[y].begin(), adj_list[y].end());

				graph.pi = new Partition(graph.num_nodes, graph.adj_list);

				cout << "(" << a+1 << "," << b+1 << "), (" << x+1 << "," << y+1 << ") -> (" << a+1 << "," << y+1 << "), (" << b+1 << "," << x+1 << ")" << endl;
				return true;
			}
		}
	}

	graph.pi = new Partition(graph.num_nodes, graph.adj_list);

	return false;
}

Timer::Timer(void) {
	prev = curr = chrono::system_clock::now();
}

long Timer::click(void) {
	prev = curr;
	curr = chrono::system_clock::now();

	return chrono::duration_cast<microseconds>(curr-prev).count();
}

