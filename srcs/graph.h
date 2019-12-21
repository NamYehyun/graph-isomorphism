#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Node {
public:
    int id;
    int colour;
    unordered_set<Node*> neighbors;

    Node(int _id);
    ~Node(void);
};

class Graph {
public:
    string name;
    int num_nodes;
    int num_edges;
    vector<Node*> nodes;
    unordered_map<int, Node*> id_to_node;

    Graph(void);
    Graph(int _num_nodes);
    ~Graph(void);

    void init(int _num_nodes);
    void set_name(string _name);

    Node* get_node(int id);
    void add_edge(int u, int v);

    void print(void);
};

#endif

