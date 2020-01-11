#include "graph.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>
#include <queue>

#include "utils.h"

using namespace std;

Node::Node(void) {}

Cell::Cell(void): counted(false), begin(-1), end(-1) {}

Cell::Cell(int _begin, int _end): counted(false), begin(_begin), end(_end) {}

void Cell::print(void) const {
	cout << "[" << begin << "," << end << ")";
	return;
}

Partition::Partition(void): init(false), equitable(false) {}

void Partition::print(void) const {
	cout << "| ";
	for (const Cell cell: cells) {
		for (int i = cell.begin; i < cell.end; ++i) {
			cout << nodes[i] << ' ';
		}
		cout << "| ";
	}

	return;
}

Graph::Graph(void): num_nodes(0), num_edges(0) {}

Graph::~Graph(void) {
	for (Node* node: id_to_node) {
		delete node;
	}
}

void Graph::init(int _num_nodes) {
	assert(0 < _num_nodes);
	num_nodes = _num_nodes;

	id_to_node.clear();
	id_to_node.resize(num_nodes, nullptr);

	adj_list.clear();
	adj_list.resize(num_nodes, vector<int>());

	return;
}

void Graph::set_name(string _name) {
	name = _name;
	return;
}

void Graph::add_node(int id) {
	assert(0 <= id && id < num_nodes);

	if (id_to_node[id] == nullptr) {
		id_to_node[id] = new Node();
	}

	return;
}

void Graph::add_edge(int u, int v) {
	assert(0 <= u && u < num_nodes);
	assert(0 <= v && v < num_nodes);

	add_node(u);
	add_node(v);

	adj_list[u].push_back(v);
	adj_list[v].push_back(u);

	++num_edges;

	return;
}

void Graph::refine(void) {
	if (pi.equitable) {
		return;
	}

	#ifdef BENCHMARK
	long init_time = 0;
	long count_time = 0;
	long split_time = 0;

	Timer timer;
	#endif

	if (!pi.init) {
		pi.nodes.clear();
		pi.nodes.reserve(num_nodes);
		for (int i = 0; i < num_nodes; ++i) {
			pi.nodes.emplace_back(i);
		}

		pi.cells.clear();
		pi.cells.push_back(Cell(0, num_nodes));

		pi.init = true;
	}

	queue<Cell*> uncounted_cells;
	for (Cell& cell: pi.cells) {
		cell.counted = false;
		uncounted_cells.push(&cell);
	}

	vector<long long> degree(num_nodes, 0);

	#ifdef BENCHMARK
	init_time += timer.click();
	#endif

	do {
		Cell* target = uncounted_cells.front(); uncounted_cells.pop();

		for (int i = target->begin; i < target->end; ++i) {
			int u = pi.nodes[i];
			for (int v: adj_list[u]) {
				++degree[v];
			}
		}

		target->counted = true;

		#ifdef BENCHMARK
		count_time += timer.click();
		#endif

		list<Cell>::iterator cell;
		for (cell = pi.cells.begin(); cell != pi.cells.end(); ++cell) {
			vector<int>::iterator begin = pi.nodes.begin() + cell->begin;
			vector<int>::iterator end = pi.nodes.begin() + cell->end;
			const function<bool(const int&, const int&)> cmp = [&degree](const int& a, const int& b) -> bool {
				return degree[a] < degree[b];
			};

			sort(begin, end, cmp);

			if (degree[pi.nodes[cell->begin]] != degree[pi.nodes[cell->end-1]]) {
				int idx = cell->begin;
				while (idx < cell->end) {
					Cell* new_cell = new Cell();
					new_cell->begin = idx;
					new_cell->end = upper_bound(pi.nodes.begin()+idx, end, *(pi.nodes.begin()+idx), cmp) - pi.nodes.begin();

					pi.cells.insert(cell, *new_cell);
					uncounted_cells.push(new_cell);

					idx = new_cell->end;
				}

				cell = pi.cells.erase(cell);
				if (cell == pi.cells.end()) {
					break;
				}
			}
		}

		#ifdef BENCHMARK
		split_time += timer.click();
		#endif

	} while (!uncounted_cells.empty());

	pi.equitable = true;

	#ifdef BENCHMARK
	cout << setw(20) << "    init time" << 1e-3 * init_time << " ms" << endl;
	cout << setw(20) << "    count time" << 1e-3 * count_time << " ms" << endl;
	cout << setw(20) << "    split time" << 1e-3 * split_time << " ms" << endl;
	#endif

	return;
}

void Graph::print(void) const {
	cout << "Graph `" << name << "`" << endl;
	cout << "  # Nodes: " << num_nodes << endl;
	cout << "  # Edges: " << num_edges << endl;
	// cout << "  pi: "; pi.print(); cout << endl;

	return;
}

