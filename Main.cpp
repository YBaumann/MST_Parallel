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
	f.open("Resources/WattsStrogatz100.txt");
	vector<edge> edgeList;

	int n; f >> n;
	int m; f >> m;
	for (int i = 0; i < m; i++) {
		edge e;
		e.idx = 2*i;
		f >> e.source >> e.dest >> e.weight;
		edgeList.push_back(e);
	}

	int numThreads = 12; 
	vector<tuple<int,int,int>> testArr;
	int testsize = 4;
	int idsize = 2;
	int sizesize = 5;
	int su = 0;
	for(int i = 0; i < testsize; i++){
		int si = rand() %sizesize + 1;
		su += si;
		testArr.push_back(make_tuple(rand() % idsize, si,i));
	}
	
	vector<int> toRewrite(su);


	for(int i = 0; i < su; i++){
		toRewrite[i] = i;
	}

	vector<int> newSizes;


	// toRewrite[i] = i --> new indices for all edges
	// testArr will store the current sizes supervertex indices and starting indices
	// new Sizes wil store the number of outgoing edges of the superverteices
	// Next: try for edges
	rewriteEdges(testArr, toRewrite, newSizes);



	return 0;
}