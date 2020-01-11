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

Node::Node(int _id): id(_id), tmp(0) {}

void Node::print(void) const {
	cout << id;
	return;
}

Cell::Cell(void): counted(false), begin(-1), end(-1) {}

Cell::Cell(int _begin, int _end): counted(false), begin(_begin), end(_end) {}

void Cell::print(void) const {
	cout << "[" << begin << "," << end << ")";
	return;
}

Partition::Partition(void): init(false), equitable(false) {}

void Partition::print(void) const {
	cout << "| ";
	for (const Cell& cell: cells) {
		for (int i = cell.begin; i < cell.end; ++i) {
			nodes[i]->print();
			cout << ' ';
		}
		cout << "| ";
	}

	return;
}

Graph::Graph(void): num_nodes(0), num_edges(0) {}

Graph::~Graph(void) {
	for (Node* node: nodes) {
		delete node;
	}
}

void Graph::init(int _num_nodes) {
	assert(0 < _num_nodes);
	num_nodes = _num_nodes;

	id_to_node.clear();

	return;
}

void Graph::set_name(string _name) {
	name = _name;
	return;
}

Node* Graph::get_node(int id) {
	assert(0 <= id && id < num_nodes);

	if (id_to_node.find(id) == id_to_node.end()) {
		Node* node = new Node(id);
		nodes.insert(node);
		id_to_node[id] = node;
	}

	return id_to_node[id];
}

void Graph::add_edge(int u, int v) {
	assert(0 <= u && u < num_nodes);
	assert(0 <= v && v < num_nodes);

	Node* node_u = get_node(u);
	Node* node_v = get_node(v);

	adj_list[node_u].insert(node_v);
	adj_list[node_v].insert(node_u);

	++num_edges;

	return;
}

void Graph::refine(void) {
	#ifdef BENCHMARK
	long init_time = 0;
	long count_time = 0;
	long split_time = 0;

	Timer timer;
	timer.click();
	#endif

	if (!pi.init) {
		for (Node* node: nodes) {
			pi.nodes.emplace_back(node);
		}

		pi.cells.clear();
		pi.cells.push_back(Cell(0, pi.nodes.size()));

		pi.init = true;
	}

	for (Node* node: nodes) {
		node->tmp = 0;
	}

	queue<Cell*> uncounted_cells;
	for (Cell& cell: pi.cells) {
		cell.counted = false;
		uncounted_cells.push(&cell);
	}

	#ifdef BENCHMARK
	init_time += timer.click();
	#endif

	do {
		#ifdef BENCHMARK
		timer.click();
		#endif
		Cell* target = uncounted_cells.front(); uncounted_cells.pop();

		for (int i = target->begin; i < target->end; ++i) {
			Node* u = pi.nodes[i];
			for (Node* v: pi.nodes) {
				if (adj_list[v].find(u) != adj_list[v].end()) {
					++(v->tmp);
				}
			}
		}

		target->counted = true;
		#ifdef BENCHMARK
		count_time += timer.click();
		#endif

		list<Cell>::iterator cell;
		for (cell = pi.cells.begin(); cell != pi.cells.end(); ++cell) {
			vector<Node*>::iterator begin = pi.nodes.begin() + cell->begin;
			vector<Node*>::iterator end = pi.nodes.begin() + cell->end;
			const function<bool(const Node*, const Node*)> cmp = [](const Node* a, const Node* b) -> bool {
				return a->tmp < b->tmp;
			};

			sort(begin, end, cmp);

			if (pi.nodes[cell->begin]->tmp != pi.nodes[cell->end-1]->tmp) {
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

