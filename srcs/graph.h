#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <vector>

using namespace std;

class Graph {
public:
    int num_nodes;
    int num_edges;
    vector<vector<bool>> adj_matrix;

    Graph(void);
    Graph(int _num_nodes);

    void init(int _num_nodes);

    bool add_edge(int u, int v);
    string certificate(void);

    void print(void);
};

#endif

