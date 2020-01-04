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

	int verbose = 0;
	string input_path = "";

	while (true) {
		static struct option long_options[] = {
			{"input", required_argument, nullptr, 'i'},
			{"verbose", no_argument, &verbose, 'v'},
			{"help", no_argument, nullptr, 'h'},
			{0, 0, 0, 0}
		};

		int option = getopt_long(argc, argv, "i:h", long_options, nullptr);
		if (option < 0) {
			break;
		}

		switch (option) {
			case 'i':
				input_path = string(optarg);
				break;
			case 'v':
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

	Graph graph;
	if (!read_graph(input_path, graph)) {
		return EXIT_FAILURE;
	}

	graph.refine();
	graph.print();

	return EXIT_SUCCESS;
}

void usage(void) {
	cout << "Usage: ./graph-isomorphism -i <input path>" << endl;
	return;
}

