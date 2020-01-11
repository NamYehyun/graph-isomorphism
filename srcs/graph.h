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

	void print(void) const;
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
	bool init;
	bool equitable;

	vector<Node*> nodes;
	list<Cell> cells;

	Partition(void);

	void print(void) const;
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

	void print(void) const;
};

#endif

