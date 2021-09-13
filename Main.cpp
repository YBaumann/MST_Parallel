#include <stdlib.h>
#include <map>
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

/*
To run on cluster do the following:

>>  export OMP_NUM_THREADS=numThreads
>>  g++ -fopenmp Main.cpp -std=gnu++11

*/

#define TimerStart a1 = std::chrono::high_resolution_clock::now()
#define TimerEnd a2 = std::chrono::high_resolution_clock::now()
#define getTime (double)std::chrono::duration_cast<std::chrono::microseconds>( a2 - a1 ).count() / 1000000.0
#define TS b1 = std::chrono::high_resolution_clock::now()
#define TE b2 = std::chrono::high_resolution_clock::now()
#define gT (double)std::chrono::duration_cast<std::chrono::microseconds>( b2 - b1 ).count() / 1000000.0
#define startTimer1 auto t1 = std::chrono::high_resolution_clock::now()
#define endTimer1 auto t2 = std::chrono::high_resolution_clock::now()
#define printTime1 auto durationSeq1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count(); std::cout << "Time Seq: " << durationSeq1 / 1000000.0<< " sec\n"
#define startTimer2 auto l1 = std::chrono::high_resolution_clock::now()
#define endTimer2 auto l2 = std::chrono::high_resolution_clock::now()
#define printTime2 auto durationSeq2 = std::chrono::duration_cast<std::chrono::microseconds>( l2 - l1 ).count(); std::cout << "Time Par: " << durationSeq2 / 1000000.0<< " sec\n"
#define nn std::cout << "\n"


using namespace std;

// Timing stuff
// To get times insert into timesmap
vector<pair<string,double>> times;
map<string,double> timesMap;
vector<string> toMap = {"Calc Best","Multi Pref","Find Parents","Rewrite Vector","Rename to Supervertex","Insert into MST","Resort edgelist","cutoff","Return MST"};

std::chrono::_V2::system_clock::time_point a1;
std::chrono::_V2::system_clock::time_point a2;

std::chrono::_V2::system_clock::time_point b1;
std::chrono::_V2::system_clock::time_point b2;


#include "headers/parasort.h"
#include "headers/Prefix.h"
#include "headers/ParAggregate.h"
#include "headers/PrefixAnySize.h"
#include "headers/vectorOperations.h"
#include "headers/FindCorrectPlace.h"
#include "headers/Structures.h"
#include "headers/finParentsUndirected.h"
#include "headers/primSteps.h"
#include "headers/CSR_Format.h"
#include "headers/EdgelistToAdjArray.h"
#include "headers/BoruvkaSeq.h"
#include "headers/KruskalSeq.h"
#include "headers/CheckConnectivity.h"
#include "headers/PrimSeq.h" 
#include "headers/SequentialCutoff.h"
#include "headers/BoruvkaPara.h"
#include "headers/rewriteVector.h"
#include "headers/cutEdgelist.h"
#include "headers/ImpStep.h"
#include "headers/ImpBoruvkaPara.h"
#include "headers/TestCases.h"

// We want the output to be an edgelist with edgeids, sorted






int main() {
	// Setup I/O and timing
	ifstream f;
	f.open("Resources/BarabasiSparse1M3E.txt");
	vector<edge> edgelist;
	vector<edge> edgelistSingle;
	for(auto e : toMap){
		timesMap.insert(make_pair(e,0));
	}

	int source,dest,weight;
	int n,m;
	int i = 0;
	while(f >> source && f >> dest && f >> weight){
		n = max(n,max(source,dest));
		m++;
		edge e1 = edge(source,dest,weight,i++);
		edge e2 = edge(e1.dest,e1.source, e1.weight, e1.idx);
		edgelist.push_back(e1);
		edgelist.push_back(e2);
		edgelistSingle.push_back(e1);
		assert(e1.weight > 0 && "Weights may not be 0");
	}
	// We add two edges
	n += 1;
	int nSafe = n;
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

	TimerStart;
	vector<edge> sols = MinimumSpanningTreeBoruvkaSeq(edgelistSingle, n, msingle);
	TimerEnd;
	times.push_back(make_pair("Sequential Runtime", getTime));

	std::cout << "Start Parallel:\n";

	TimerStart;
	std::cout << "Starts\n";
	int nrThreads = 8;
	int cutoff = 100;
	vector<edge> solp = ParBoruvkaImp(edgelist, edgelistSingle, outgoingEdges, n, m, nrThreads, cutoff);
	TimerEnd;
	times.push_back(make_pair("Parallel Runtime", getTime));

	assert(is_Connected(sols, nSafe) && "Solution seq is not connected");
	assert(is_Connected(solp, nSafe) && "Solution par is not connected");
	int parRes = 0;
	set<int> seqSet;
	set<int> parSet;
	std::cout << "Found Parallel MST:\n";
	for(auto e : solp){
		//std::cout << e.source << ' ' << e.dest;nn;
		parSet.insert(e.idx);
		parRes += e.weight;
	}
	nn;
	int seqRes = 0;
	for(auto e : sols){
		seqSet.insert(e.idx);
		seqRes += e.weight;
	} 

	std::cout << "ImpBor: " << parRes;nn;
	std::cout << "SeqBor: " << seqRes;nn;
	std::cout << "\nTiming:\n--------------------\n";
	for(auto e : times){
		std::cout << e.first << " : " << e.second;nn;
	}
	std::cout << "\nBreakdown:\n--------------------\n";
	for(auto e : timesMap){
		std::cout << e.first << " : " << e.second;nn;
	}
	nn;
	return 0;
}