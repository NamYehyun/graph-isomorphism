#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

#include "graph.h"

using namespace std;

/* Generate graph from DIMACS file */
bool read_graph(string input_path, Graph& graph);

#endif

