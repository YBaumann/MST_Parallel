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
#include <omp.h>
#include <chrono>

// here we store all possibilities for sequential cutoff algorithms


vector<edge> doSequentialCutoff(int n, vector<edge> edgelist){
    std::cout << "before seqCutoff\n";
    return MinimumSpanningTreeBoruvkaSeq(edgelist, n, edgelist.size());
}