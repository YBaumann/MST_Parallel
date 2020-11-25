#pragma once
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <cassert>


#define nn std::cout << "\n"


vector<edge> MinimumSpanningTreeBoruvkaPar(vector<edge> edgelist, int n, int m) { // This implementation is based on an adjacency list
	vector<edge> result;
    omp_set_num_threads(1000);

    cout << omp_get_num_threads() << "\n";
    cout << omp_get_num_procs();



	return result;
}

