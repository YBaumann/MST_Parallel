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


void BoruvkaStepSeq(set<edge>& mst, vector<edge>& edgelist, UnionFind &UF, int n) {
	vector<edge> BestOutgoingEdges(n); // safe the best edges for each supervertex this round
	int m = edgelist.size();
	for (int i = 0; i < m; i++) { // loop through all edges and find the current best edges
		edge e = edgelist[i];
		int superSource = UF.find(e.source);
		int superDest = UF.find(e.dest);

		if (superSource != superDest) { // belong to different supervertices
			if ((BestOutgoingEdges[superSource] > e) || BestOutgoingEdges[superSource].weight == 0) { // check if its a best edge for source and replace
				BestOutgoingEdges[superSource] = e;
			}
			if ((BestOutgoingEdges[superDest] > e) || BestOutgoingEdges[superDest].weight == 0) { // check if its a best edge for dest and replace
				BestOutgoingEdges[superDest] = e;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		if (BestOutgoingEdges[i].weight != 0) {
			mst.insert(BestOutgoingEdges[i]);
			UF.merge(BestOutgoingEdges[i].source, BestOutgoingEdges[i].dest);
		}
	}

}


vector<edge> MinimumSpanningTreeBoruvkaSeq(vector<edge> edgelist, int n, int m) { // This implementation is based on an undirected graph
	set<edge> mst; // edgelist of final MST
	UnionFind UF(n);

	int StepNr = 1;
	nn;
	while (mst.size() < n - 1) { // while we dont have a full tree
		int weight = 0;
		BoruvkaStepSeq(mst, edgelist, UF, n); // do one step
	}

	vector<edge> result;
	for (auto e : mst) {
		result.push_back(e);
	}
	nn;
	
	return result;
}

