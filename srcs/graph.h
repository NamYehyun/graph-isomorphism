#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Node {
public:
	int id;
	int tmp;

	Node(int _id);

	void print(void);
};

class Cell {
public:
	bool counted;
	vector<Node*> nodes;

	Cell(void);

	void print(void);
};

class Partition {
public:
	bool init;
	bool equitable;
	list<Cell> cells;

	Partition(void);

	void print(void);
};

class Graph {
public:
	string name;
	int num_nodes;
	int num_edges;
	unordered_set<Node*> nodes;
	unordered_map<int, Node*> id_to_node;
	unordered_map<Node*, unordered_set<Node*>> adj_list;

	Partition pi;

	Graph(void);
	~Graph(void);

	void init(int _num_nodes);
	void set_name(string _name);
	Node* get_node(int id);
	void add_edge(int u, int v);

	void refine(void);

	void print(void);
};

#endif

