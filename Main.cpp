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
	for(int i = 0; i< 1000; i++){
	resultBoruvka = MinimumSpanningTreeBoruvkaPar(edgeList,n,m);
	std::cout <<i;
	}
	std::cout << resultBoruvka.size();

	return 0;
}