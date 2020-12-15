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
#include "headers\EdgelistToAdjArray.h"
#include "headers\BoruvkaSeq.h"
#include "headers\KruskalSeq.h"
#include "headers\CheckConnectivity.h"
#include "headers\PrimSeq.h" 
#include "headers\BoruvkaPara.h"

// We want the output to be an edgelist with edgeids, sorted

using namespace std;


int main() {
	ifstream f;
	f.open("Resources/WattsStrogatz100.txt");
	vector<edge> edgeList;
	int n; f >> n;
	int m; f >> m;
	for (int i = 0; i < m; i++) {
		edge e;
		e.idx = i;
		f >> e.source >> e.dest >> e.weight;
		edgeList.push_back(e);
	}
 
	
	vector<edge> mstp = MinimumSpanningTreeBoruvkaPar(edgeList, n, m);
	//vector<edge> msts = MinimumSpanningTreeBoruvkaSeq(edgeList, n, m);

	int pw = 0;
	int sw = 0;


	for(int i = 0; i < mstp.size(); i++){
		//pw += mstp[i].weight;
		sw += mstp[i].weight;
	}
	std::cout << pw << ' ' << sw;
	





	return 0;
}