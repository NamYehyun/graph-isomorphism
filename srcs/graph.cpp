#include "graph.h"

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

Node::Node(int _id): id(_id), colour(0) {}

Node::~Node(void) {}

Graph::Graph(void): num_nodes(0), num_edges(0) {
    nodes.clear();
    id_to_node.clear();
}

Graph::Graph(int _num_nodes): num_nodes(_num_nodes), num_edges(0) {
    assert(0 < num_nodes);

    nodes.clear();
    nodes.reserve(num_nodes);

    id_to_node.clear();
}

Graph::~Graph(void) {
    for (Node* node: nodes) {
        delete node;
    }
}

void Graph::init(int _num_nodes) {
    assert(0 < _num_nodes);
    num_nodes = _num_nodes;
    
    nodes.clear();
    nodes.reserve(num_nodes);

    id_to_node.clear();

    return;
}

Node* Graph::get_node(int id) {
    assert(0 <= id && id < num_nodes);

    if (id_to_node.find(id) == id_to_node.end()) {
        Node* node = new Node(id);
        nodes.emplace_back(node);
        id_to_node[id] = node;
    }

    return id_to_node[id];
}

void Graph::add_edge(int u, int v) {
    assert(0 <= u && u < num_nodes);
    assert(0 <= v && v < num_nodes);

    Node* node_u = get_node(u);
    Node* node_v = get_node(v);

    node_u->neighbors.insert(node_v);
    node_v->neighbors.insert(node_u);

    ++num_edges;

    return;
}

void Graph::print(void) {
    cout << "Nodes: " << num_nodes << endl;
    cout << "Edges: " << num_edges << endl;

    return;
}

