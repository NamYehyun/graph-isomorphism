#include <iomanip>
#include <iostream>
#include <getopt.h>

#include "graph.h"
#include "utils.h"

using namespace std;

void usage(void);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		usage();
		return EXIT_FAILURE;
	}

	bool verbose = 0;
	string input_path = "";

	while (true) {
		static struct option long_options[] = {
			{"input", required_argument, nullptr, 'i'},
			{"verbose", no_argument, nullptr, 'v'},
			{"help", no_argument, nullptr, 'h'},
			{0, 0, 0, 0}
		};

		int option = getopt_long(argc, argv, "i:vh", long_options, nullptr);
		if (option < 0) {
			break;
		}

		switch (option) {
			case 'i':
				input_path = string(optarg);
				break;
			case 'v':
				verbose = true;
				break;
			case 'h':
				usage();
				return EXIT_SUCCESS;
			default:
				break;
		}
	}

	if (input_path == "") {
		cout << "Input path is not given." << endl;
		return EXIT_FAILURE;
	}

	#ifdef BENCHMARK
	verbose = true;
	#endif

	Timer timer;
	timer.click();

	Graph graph;
	if (!read_graph(input_path, graph)) {
		return EXIT_FAILURE;
	}

	if (verbose) {
		cout << endl;
		cout << setfill('.') << left << setw(20) << "  Generate graph" << right << setw(50) << to_string(1e-3 * timer.click()) + " ms elapsed" << endl << setfill(' ');
	}

	if (verbose) {
		cout << setfill('.') << left << setw(70) << "  Refine graph" << endl << setfill(' ');
	}

	graph.refine();

	if (verbose) {
		cout << "  " << setfill('.') << right << setw(68) << to_string(1e-3 * timer.click()) + " ms elapsed" << endl << setfill(' ');
	}

	cout << endl;
	graph.print();
	cout << endl;

	return EXIT_SUCCESS;
}

void usage(void) {
	cout << "Usage: ./graph-isomorphism -i <input path>" << endl;
	return;
}

