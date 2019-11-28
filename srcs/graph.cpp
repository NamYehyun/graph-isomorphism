#include "graph.h"

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

Graph::Graph(void): num_nodes(0), num_edges(0) {}

Graph::Graph(int _num_nodes): num_nodes(_num_nodes), num_edges(0) {
    assert(0 < num_nodes);
    adj_matrix.resize(num_nodes, vector<bool>(num_nodes, false));
}

void Graph::init(int _num_nodes) {
    assert(0 < _num_nodes);
    num_nodes = _num_nodes;
    adj_matrix.resize(num_nodes, vector<bool>(num_nodes, false));

    return;
}

bool Graph::add_edge(int u, int v) {
    assert(0 <= u && u < num_nodes);
    assert(0 <= v && v < num_nodes);

    if (adj_matrix[u][v] || adj_matrix[v][u]) {
        return false;
    }

    adj_matrix[u][v] = adj_matrix[v][u] = true;
    ++num_edges;

    return true;
}

string Graph::certificate(void) {
    string ret = "";
    for (int v = 0; v < num_nodes; ++v) {
        for (int u = 0; u < v; ++u) {
            ret += (adj_matrix[u][v] ? "1" : "0");
        }
    }

    return ret;
}

void Graph::print(void) {
    cout << "Nodes: " << num_nodes << endl;
    cout << "Edges: " << num_edges << endl;

    cout << endl << "Adjacency Matrix: " << endl;
    cout << "  ";
    for (int v = 0; v < num_nodes; ++v) {
        cout << v % 10 << ' ';
    }
    cout << endl;
    for (int u = 0; u < num_nodes; ++u) {
        cout << u % 10 << ' ';
        for (int v = 0; v < num_nodes; ++v) {
            cout << adj_matrix[u][v] << ' ';
        }
        cout << endl;
    }
    cout << endl;

    cout << "Certificate: " << certificate() << endl;

    return;
}

