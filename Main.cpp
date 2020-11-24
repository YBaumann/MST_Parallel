#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <cassert>
#include "headers\Structures.h"

#include "headers\BoruvkaSeq.h"
#include "headers\KruskalSeq.h"
#include "headers\CheckConnectivity.h"
#include "headers\PrimSeq.h"



// We want the output to be an edgelist with edgeids, sorted

using namespace std;


int main() {
	ifstream f;
	f.open("Resources/WattsStrog.txt");
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
	//resultBoruvka = MinimumSpanningTreeBoruvkaSeq(edgeList,n,m);
	vector<edge> resultKruskal;
	//resultKruskal = MinimumSpanningTreeKruskalSeq(edgeList, n, m);
	vector<edge> resultPrim;
	//resultPrim = MinimumSpanningTreePrimSeq(edgeList, n, m);


	return 0;
}