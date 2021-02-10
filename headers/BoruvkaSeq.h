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


void BoruvkaStepSeq(set<edge>& mst, vector<edge>& edgelist, UnionFind UF, int n) {
	vector<edge> BestOutgoingEdges(n); // safe the best edges for each supervertex this round

	for (int i = 0; i < edgelist.size(); i++) { // loop through all edges and find the current best edges
		edge e = edgelist[i];
		int superSource = UF.find(e.source);
		int superDest = UF.find(e.dest);
		
		if (superSource != superDest) { // belong to different supervertices
			if (e.weight < BestOutgoingEdges[superSource].weight || BestOutgoingEdges[superSource].weight == 0) { // check if its a best edge for source and replace
				BestOutgoingEdges[superSource] = e;
			}
			if (e.weight < BestOutgoingEdges[superDest].weight || BestOutgoingEdges[superDest].weight == 0) { // check if its a best edge for dest and replace
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


vector<edge> MinimumSpanningTreeBoruvkaSeq(vector<edge> edgelist, int n, int m) { // This implementation is based on an adjacency list
	set<edge> mst; // edgelist of final MST
	UnionFind UF(n);

	int StepNr = 1;
	while (mst.size() < n - 1) { // while we dont have a full tree
		
		BoruvkaStepSeq(mst, edgelist, UF, n); // do one step
    	
	}

	vector<edge> result;
	for (auto e : mst) {
		result.push_back(e);
	}

	return result;
}

