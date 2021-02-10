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

#define startTimer auto t1 = std::chrono::high_resolution_clock::now()
#define endTimer auto t2 = std::chrono::high_resolution_clock::now()
#define printTime auto durationSeq = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count(); std::cout << "Time: " << durationSeq / 1000000.0<< " sec\n"
#define nn std::cout << "\n"

using namespace std;



int main() {
	ifstream f;
	f.open("Resources/WattsStrogatz100K.txt");
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

	int numThreads = 2; 


	auto S1 = std::chrono::high_resolution_clock::now();
    vector<edge> msts = MinimumSpanningTreeBoruvkaSeq(edgel1, n, m);
    auto S2 = std::chrono::high_resolution_clock::now();

    auto durationSeq = std::chrono::duration_cast<std::chrono::microseconds>( S2 - S1 ).count();

	auto P1 = std::chrono::high_resolution_clock::now();
	vector<edge> mstp = MinimumSpanningTreeBoruvkaPar(edgel2, n, m, numThreads);
    auto P2 = std::chrono::high_resolution_clock::now();

    auto durationPar = std::chrono::duration_cast<std::chrono::microseconds>( P2 - P1 ).count();

	double pw = 0;
	double sw = 0;

	for(auto e : mstp){
		pw += e.weight;
	}
	std::cout << "Calculated weight Parallel: " << pw << " Time: " << durationPar / 1000000.0 << "\n";

	for(auto e : msts){
		sw += e.weight;
	}
	std::cout << "Calculated weight Sequential: " << sw << " Time: " << durationSeq / 1000000.0<< "\n";

	

	//runAllTests();

	


	return 0;
}