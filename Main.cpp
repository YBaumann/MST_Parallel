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

#include "headers\Structures.h"
#include "headers\EdgelistToAdjArray.h"
#include "headers\BoruvkaSeq.h"
#include "headers\KruskalSeq.h"
#include "headers\CheckConnectivity.h"
#include "headers\PrimSeq.h" 
#include "headers\BoruvkaPara.h"
#include "headers\TestCases.h"

// We want the output to be an edgelist with edgeids, sorted

using namespace std;


int main() {
	ifstream f;
	f.open("Resources/WattsStrogatz100.txt");
	vector<edge> edgeList;
	int n; f >> n;
	int m; f >> m;
	for (int i = 0; i < m; i++) {
		edge e;
		e.idx = i;
		f >> e.source >> e.dest >> e.weight;
		edgeList.push_back(e);
	}
	vector<edge> edgel1 = edgeList;
	vector<edge> edgel2 = edgeList;

	int numThreads = 32;
	/*
	vector<edge> msts = MinimumSpanningTreeBoruvkaSeq(edgel1, n, m);
	vector<edge> mstp = MinimumSpanningTreeBoruvkaPar(edgel2, n, m, numThreads);
	
	double pw = 0;
	double sw = 0;

	for(auto e : mstp){
		pw += e.weight;
	}
	std::cout << "Calculated weight Parallel: " << pw << "\n";

	for(auto e : msts){
		sw += e.weight;
	}
	std::cout << "Calculated weight Sequential: " << sw << "\n";

*/
	runAllTests();



	return 0;
}