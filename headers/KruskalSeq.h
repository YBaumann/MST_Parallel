#pragma once
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <cassert>


#define nn std::cout << "\n"

using namespace std;

vector<edge> MinimumSpanningTreeKruskalSeq(vector<edge> edgelist, int n, int m) {
	vector<edge> mst;
	sort(edgelist.begin(), edgelist.end());
	UnionFind UF(n);
	std::cout << "Checkpoint UnioFind datastructure created"; nn;
	for (int i = 0; i < m; i++) {
		edge e = edgelist[i];
		if (!UF.equalSuper(e.source,e.dest)) {
			UF.merge( e.source, e.dest);
			mst.push_back(edgelist[i]);
		}
	}

	std::cout << "Checkpoint: Kruskal-Mst created"; nn;



	return mst;
}




