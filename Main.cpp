#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <cassert>
#include "Structures.h"

#include "BoruvkaSeq.h"
#include "KruskalSeq.h"
#include "CheckConnectivity.h"
#include "PrimSeq.h"
#include "BaderCongSeq.h"



// We want the output to be an edgelist with edgeids, sorted

using namespace std;


int main() {
	ifstream f;
	f.open("WattsStrog.txt");
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

	vector<edge> resultBC;
	resultBC = MinimumSpanningTreeBaderCongSeq(edgeList, n, m);


	return 0;
}