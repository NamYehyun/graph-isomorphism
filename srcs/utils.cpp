#include "utils.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/* Generate graph from DIMACS file */
bool read_graph(string input_path, Graph& graph) {
	ifstream input(input_path);
	if (!input.is_open()) {
		return false;
	}

	string line;
	while (getline(input, line)) {
		istringstream iss(line);

		char key;
		string name;
		string token;
		int num_nodes = 0;
		int num_edges = 0;
		int u, v;

		iss >> key;
		switch (key) {
			case 'c': /* comment line */
				iss >> name;
				while (iss >> token) {
					name += ("_" + token);
				}
				graph.set_name(name);
				break;
			case 'p': /* problem line */
				iss >> token >> num_nodes >> num_edges;
				graph.init(num_nodes);
				break;
			case 'e': /* edge */
				iss >> u >> v;
				graph.add_edge(u-1, v-1);
				break;
		}
	}

	input.close();

	return true;
}

