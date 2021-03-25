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
#include <math.h>


#include "headers\Prefix.h"
#include "headers\PrefixAnySize.h"
#include "headers\vectorOperations.h"
#include "headers\FindCorrectPlace.h"
#include "headers\Structures.h"
#include "headers\CSR_Format.h"
#include "headers\EdgelistToAdjArray.h"
#include "headers\BoruvkaSeq.h"
#include "headers\KruskalSeq.h"
#include "headers\CheckConnectivity.h"
#include "headers\PrimSeq.h" 
#include "headers\SequentialCutoff.h"
#include "headers\BoruvkaPara.h"
#include "headers\ImpBoruvkaPara.h"
#include "headers\TestCases.h"

// We want the output to be an edgelist with edgeids, sorted

#define startTimer auto t1 = std::chrono::high_resolution_clock::now()
#define endTimer auto t2 = std::chrono::high_resolution_clock::now()
#define printTime auto durationSeq = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count(); std::cout << "Time: " << durationSeq / 1000000.0<< " sec\n"
#define nn std::cout << "\n"

using namespace std;



int main() {
	ifstream f;
	f.open("Resources/WattsStrog.txt");
	vector<edge> edgelist;

	int n; f >> n;
	int m; f >> m;
	for (int i = 0; i < m; i++) {
		edge e1;
		edge e2;
		e1.idx = 2*i;
		e2.idx = 2*i+1;
		f >> e1.source >> e1.dest >> e1.weight;
		e2.source = e1.dest;
		e2.dest = e1.source;
		e2.weight = e1.weight;
		edgelist.push_back(e1);
		edgelist.push_back(e2);
	}
	// We add two edges
	m = 2*m;

	// Sort the edgelist
	auto compara = [](edge e1, edge e2){return e1.source <= e2.source;};
	sort(edgelist.begin(), edgelist.end(), compara);

	// Count outgoing sizes -> This can be parallel
	vector<int> outgoingEdges(n);
	for(int i = 0; i < m; i++){
		outgoingEdges[edgelist[i].source]++;
	}

	int nrThreads = 2;
	vector<edge> sol = ParBoruvkaImp(edgelist, outgoingEdges, n, m, nrThreads);

	

	return 0;
}