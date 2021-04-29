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

#define startTimer1 auto t1 = std::chrono::high_resolution_clock::now()
#define endTimer1 auto t2 = std::chrono::high_resolution_clock::now()
#define printTime1 auto durationSeq1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count(); std::cout << "Time Seq: " << durationSeq1 / 1000000.0<< " sec\n"
#define startTimer2 auto l1 = std::chrono::high_resolution_clock::now()
#define endTimer2 auto l2 = std::chrono::high_resolution_clock::now()
#define printTime2 auto durationSeq2 = std::chrono::duration_cast<std::chrono::microseconds>( l2 - l1 ).count(); std::cout << "Time Par: " << durationSeq2 / 1000000.0<< " sec\n"
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

	int nrThreads = 107;
	startTimer2;
	vector<edge> solp = ParBoruvkaImp(edgelist, outgoingEdges, n, m, nrThreads);
	endTimer2;
	printTime2;

	startTimer1;
	vector<edge> sols = MinimumSpanningTreeBoruvkaSeq(edgelistSingle, n, msingle);
	endTimer1;
	printTime1;
	assert(is_Connected(sols, nSafe) && "Solution seq is not connected");
	assert(is_Connected(solp, nSafe) && "Solution par is not connected");
	int parRes = 0;
	set<int> seqSet;
	set<int> parSet;
	for(auto e : solp){
		parSet.insert(e.idx);
		parRes += e.weight;
	}
	int seqRes = 0;
	for(auto e : sols){
		seqSet.insert(e.idx);
		seqRes += e.weight;
	}
	std::cout << "Par: \n";
	for(auto e : parSet){
		//std::cout << e << ' ';
	}
	nn;
	std::cout << "Seq: \n";
	for(auto e : seqSet){
		//std::cout << e << ' ';
	}
	nn;
	std::cout << "ImpBor: " << parRes;nn;
	std::cout << "SeqBor: " << seqRes;nn;

	return 0;
}