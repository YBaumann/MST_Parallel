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
#include "headers\BoruvkaSeq.h"
#include "headers\KruskalSeq.h"
#include "headers\CheckConnectivity.h"
#include "headers\PrimSeq.h" 
#include "headers\BoruvkaPara.h"



// We want the output to be an edgelist with edgeids, sorted

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
 
	std::cout << "Hi";

	assert(m == edgeList.size() && "m != edgelist size");
	std::cout << "Checkpoint reading InputGraph"; nn;

	assert(is_Connected(edgeList, n, m));
	std::cout << "Checkpoint is Connected"; nn;
	vector<edge> resultBoruvka;

	auto totalDur = 0;
	long long ma = 0;
	long long mi = 100000000;
	for(int i = 0; i< 100; i++){
	auto t1 = std::chrono::high_resolution_clock::now();
	resultBoruvka = MinimumSpanningTreeBoruvkaPar(edgeList,n,m);
    auto t2 = std::chrono::high_resolution_clock::now();
	ma = max(ma, std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count());
	mi = min(mi, std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count());
	std::cout <<i;
	totalDur += std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
	}
	nn;
	std::cout << "Time is: ";
	std::cout << totalDur / 100.0;nn;
	std::cout << "The min is: " << mi << " and the max is: " << ma;


	return 0;
}