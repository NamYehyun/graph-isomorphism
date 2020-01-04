#include "graph.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

Node::Node(int _id): id(_id), tmp(0) {}

void Node::print(void) {
	cout << id;
}

Cell::Cell(void): counted(false) {}

void Cell::print(void) {
	cout << "( ";
	for (Node* node: nodes) {
		cout << node->id << ' ';
	}
	cout << ")";
}

Partition::Partition(void): init(false), equitable(false) {}

void Partition::print(void) {
	for (Cell cell: cells) {
		cell.print();
		cout << ' ';
	}
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
	if (!pi.init) {
		Cell cell;
		for (Node* node: nodes) {
			cell.nodes.emplace_back(node);
		}

		pi.cells.clear();
		pi.cells.push_back(cell);

		pi.init = true;
	}

	bool stable = true;

	for (Node* node: nodes) {
		node->tmp = 0;
	}

	queue<Cell*> uncounted_cells;
	for (Cell& cell: pi.cells) {
		cell.counted = false;
		uncounted_cells.push(&cell);
	}

	do {
		Cell* target = uncounted_cells.front(); uncounted_cells.pop();

		for (Node* u: target->nodes) {
			for (Node* v: nodes) {
				if (adj_list[v].find(u) != adj_list[v].end()) {
					++(v->tmp);
				}
			}
		}

		target->counted = true;

		stable = true;
		list<Cell>::iterator cell;
		for (cell = pi.cells.begin(); cell != pi.cells.end(); ++cell) {
			vector<Node*>& cnodes = cell->nodes;
			sort(cnodes.begin(), cnodes.end(),
					[](const Node* a, const Node* b) -> bool {
					return a->tmp < b->tmp;
					}
				);

			if (cnodes.front()->tmp != cnodes.back()->tmp) {

				int idx = 0;
				while (idx < static_cast<int>(cnodes.size())) {
					Cell new_cell;
					new_cell.nodes.emplace_back(cnodes[idx]);
					while (idx < static_cast<int>(cnodes.size())-1 && cnodes[idx+1]->tmp == cnodes[idx]->tmp) {
						new_cell.nodes.emplace_back(cnodes[idx+1]);
						++idx;
					}

					pi.cells.insert(cell, new_cell);
					uncounted_cells.push(&new_cell);
					++idx;
				}

				cell = pi.cells.erase(cell);
				if (cell == pi.cells.end()) {
					break;
				}

				stable = false;
			}
		}
	} while (!stable);

	pi.equitable = true;
	return;
}

void Graph::print(void) {
	cout << "Graph `" << name << "`" << endl;
	cout << "  # Nodes: " << num_nodes << endl;
	cout << "  # Edges: " << num_edges << endl;
	cout << "  pi: "; pi.print(); cout << endl;

	return;
}

