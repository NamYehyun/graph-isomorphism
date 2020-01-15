#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <list>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Node {
public:
	Node(void);
};

class Cell {
public:
	bool counted;
	int begin, end;

	Cell(void);
	Cell(int _begin, int _end);

	void print(void) const;
};

class Partition {
public:
	bool equitable;
	vector<int> nodes;
	vector<vector<int>>& adj_list;
	list<Cell> cells;

	Partition(int _num_nodes, vector<vector<int>>& _adj_list);

	bool operator==(const Partition& other) const;

	void refine(void);
	void isolate(int target);

	void print(void) const;
};

class Graph {
public:
	string name;
	int num_nodes;
	int num_edges;
	vector<Node*> id_to_node;
	vector<vector<int>> adj_list;

	Partition* pi;

	Graph(void);
	~Graph(void);

	void init(int _num_nodes);
	void set_name(string _name);
	void add_node(int id);
	void add_edge(int u, int v);

	void print(void) const;
};

#endif

