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

// the idea for parallel boruvka is, to compute the Bourvka steps in parallel.
// In each Boruvka step we loop through all remaining vertices and "acquire" the smallest incident edge.
// then we can contract these edges in a sequential step (or we do a BSP contraction.)
// if we make the mst unique we can even use a sequential Union Find (non BSP)

void BoruvkaStepPar(set<edge>& mst, vector<edge>& edgelist, UnionFind UF, int n){
    vector<edge> BestOutgoingEdges(n); // safe the best edges for each supervertex this round



    // We could theoretically have atomic writes to bestoutgoing edges and then do this in parallel,
    // Can we think of a version that does not need sequential consistency?
    #pragma omp parallel for
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

	//std::cout << "Checkpoint finding best edges of round done!"; nn;

	for (int i = 0; i < n; i++) {
		if (BestOutgoingEdges[i].weight != 0) {
			mst.insert(BestOutgoingEdges[i]);
			UF.merge(BestOutgoingEdges[i].source, BestOutgoingEdges[i].dest);
		}
	}
	//std::cout << "Checkpoint: contraction done"; nn;


}

vector<edge> MinimumSpanningTreeBoruvkaPar(vector<edge> edgelist, int n, int m) { // This implementation is based on an adjacency list
    // design variables
    int numThreads = 32;
    omp_set_num_threads(numThreads);

    // Actual Code

    set<edge> mst; // edgelist of final MST
	UnionFind UF(n);

	//std::cout << "Checkpoint Unionfind for Parallel created"; nn;

	int rounds = 0;
	int last_round_size = mst.size();
	while (mst.size() < n - 1) { // while we dont have a full tree
		BoruvkaStepPar(mst, edgelist, UF, n); // do one step
		rounds++;
		//std::cout << "We have done " << rounds << " Boruvka steps"; nn;
		assert(last_round_size < mst.size() && "Size has not increased in this round!");
		last_round_size = mst.size();
		//std::cout << mst.size(); nn;
	}

	vector<edge> result;
	for (auto e : mst) {
		result.push_back(e);
	}



	return result;
}

