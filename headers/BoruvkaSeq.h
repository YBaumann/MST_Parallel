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
			if (e.weight < BestOutgoingEdges[superSource].weight || BestOutgoingEdges[superSource].weight == 0) { // check if its a best edge for source and replace
				BestOutgoingEdges[superSource] = e;
			}
			if (e.weight < BestOutgoingEdges[superDest].weight || BestOutgoingEdges[superDest].weight == 0) { // check if its a best edge for dest and replace
				BestOutgoingEdges[superDest] = e;
			}
		}
	}

	int BestWeights = 0;

	for (int i = 0; i < n; i++) {
		if (BestOutgoingEdges[i].weight != 0) {
			//std::cout << BestOutgoingEdges[i].idx << ' ';
			BestWeights += BestOutgoingEdges[i].weight;
			mst.insert(BestOutgoingEdges[i]);
			UF.merge(BestOutgoingEdges[i].source, BestOutgoingEdges[i].dest);
		}
	}
	std::cout << "Best Weight: " << BestWeights; nn;

}


vector<edge> MinimumSpanningTreeBoruvkaSeq(vector<edge> edgelist, int n, int m) { // This implementation is based on an undirected graph
	set<edge> mst; // edgelist of final MST
	UnionFind UF(n);

	int StepNr = 1;
	while (mst.size() < n - 1) { // while we dont have a full tree
		int weight = 0;
		std::cout << n-mst.size();nn;
		BoruvkaStepSeq(mst, edgelist, UF, n); // do one step
		for(auto e : mst){
			weight += e.weight;
		}
		std::cout << "Current Weight is: " << weight;nn;
		std::cout << "Current Size is: " << mst.size();nn;
    	
	}

	vector<edge> result;
	for (auto e : mst) {
		result.push_back(e);
	}
	nn;
	
	return result;
}

