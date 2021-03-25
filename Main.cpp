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
		edge e;
		e.idx = i;
		f >> e.source >> e.dest >> e.weight;
		edgelist.push_back(e);
	}

	vector<int> sizes(n);
	vector<int> indices(m);
	vector<int> newSizes;

	for(int i = 0; i < m; i++){
		sizes[edgelist[i].source]++;
		indices[i] = i;
	}

	vector<tuple<int,int,int>> arr(n); // tuple<int,int,int> = ID -> Size -> index

	for(int i = 0; i < n; i++){
		arr[i] = make_tuple(rand() % 4, sizes[i], i);
	}

	

	rewriteEdges(arr, indices, newSizes);

	for(int i = 0; i < n; i++){
		std::cout << get<0>(arr[i]) << ' ';
	}
	nn;
	for(int i = 0; i < n; i++){
		std::cout << get<1>(arr[i]) << ' ';
	}
	nn;

	for(int i = 0; i < newSizes.size(); i++){
		std::cout << newSizes[i] << ' ';
	}


	return 0;
}