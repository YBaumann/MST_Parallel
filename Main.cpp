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
#include "headers\rewriteVector.h"
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
	f.open("Resources/WattsStrogatz100K.txt");
	vector<edge> edgelist;
	vector<edge> edgelistSingle;
	int n; f >> n;
	int m; f >> m;
	int nSafe = n;
	for (int i = 0; i < m; i++) {
		edge e1 = edge(0,0,0,0);
		e1.idx = i;
		f >> e1.source >> e1.dest >> e1.weight;
		edge e2 = edge(e1.dest,e1.source, e1.weight, e1.idx);
		edgelist.push_back(e1);
		edgelist.push_back(e2);
		edgelistSingle.push_back(e1);
		assert(e1.weight > 0 && "Weights may not be 0");
	}
	// We add two edges
	int msingle = m;
	m = 2*m;
	std::cout << "Read everything\n";

	// Sort the edgelist
	auto compara = [](edge e1, edge e2){return e1.source < e2.source;};
	sort(edgelist.begin(), edgelist.end(), compara);

	std::cout << "Passes Sorting\n";

	// Count outgoing sizes -> This can be parallel
	vector<int> outgoingEdges(n);
	for(int i = 0; i < m; i++){
		outgoingEdges[edgelist[i].source]++;
	}
	std::cout << "Edge Size: " << outgoingEdges[0];nn; 

	int nrThreads = 1;
	std::cout << "Calc MST\n";
	vector<edge> solp = ParBoruvkaImp(edgelist, outgoingEdges, n, m, nrThreads);
	nn;nn;
	vector<edge> sols = MinimumSpanningTreeBoruvkaSeq(edgelistSingle, n, msingle);
	assert(is_Connected(sols, nSafe) && "Solution seq is not connected");
	assert(is_Connected(solp, nSafe) && "Solution par is not connected");
	int parRes = 0;
	for(auto e : solp){
		parRes += e.weight;
	}
	int seqRes = 0;
	for(auto e : sols){
		seqRes += e.weight;
	}
	nn;
	std::cout << "ImpBor: " << parRes;nn;
	std::cout << "SeqBor: " << seqRes;nn;

	return 0;
}