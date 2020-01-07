#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <chrono>

#include "graph.h"

using namespace std;

using milliseconds = chrono::milliseconds;
using microseconds = chrono::microseconds;
using time_point = chrono::system_clock::time_point;

/* Generate graph from DIMACS file */
bool read_graph(string input_path, Graph& graph);

class Timer {
public:
	time_point prev;
	time_point curr;

	Timer(void);

	/* Get elapsed time after the most recent click() */
	long click(void);
};

#endif

