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

#include "ParallelPrefixSum.h"
#include "BinSearchLowIdx.h"
#include "findParents.h"
#include "MultiPrefixScanPara.h"

#define nn std::cout << "\n"

// Design variables
	int numThreads = 12;


void BoruvkaStepPar(vector<edge> edgelist, set<int> &mst, int &n, int m)
{
	std::cout << "Checkpoint 1 ";
	// Divide Workload
	
	int workloadPerProcessor = 2 * m / numThreads;
	int workloadLastProcessor = 2 * m - ((numThreads - 1) * workloadPerProcessor);

	//Datastructures
	//create adjacency array --> can be parallelized?!
	vector<vector<edge>> adjArr = edgeListToAdjArray(edgelist, n);
	vector<int> prefix(n);
	vector<edge> best(n);
	vector<int> ParentVertex(n);
	vector<edge> PrefixScanVector(2*numThreads);
	for(int i = 0; i< n; i++){ParentVertex[i] = i;}
	prefixParallel(prefix, adjArr);

// now comes parallel part
#pragma omp parallel for ordered
	for (int tr = 0; tr < numThreads; tr++)
	{
		// organize my workload
		int Tid = omp_get_thread_num();
		int startWL = Tid * workloadPerProcessor;	  // included
		int endWL = (Tid + 1) * workloadPerProcessor; // not included
		if (Tid == numThreads - 1)
		{
			endWL = 2 * m;
		}

		// find my workloadstart with binsearch
		int startidx = binSearchlow(prefix, startWL); // Doesnt work yet
		

		vector<edge> localEdgeList;
		int edgeidx = prefix[startidx];
		int endidx = startidx;
		for(int i = startidx; i < adjArr.size(); i++){
			for(int j = 0; j < adjArr[i].size(); j++){
				if(edgeidx >= startWL && edgeidx < endWL){
					localEdgeList.push_back(adjArr[i][j]);
				}
				if(edgeidx == endWL){
					endidx = i;
					goto finished;
				}
				edgeidx++;
			}
		}
		finished:
		// now each thread has its own local edgelist ready
		// next step is, that each thread proposes a best edge for all vertices it looks at
		// if a processor has a best edge for a vertex that is neither start nor end, then it can write it to bestedges regardless
		vector<edge> proposalStartEnd(2); // first entry is proposal for start, second entry is proposal for end
		for(auto e : localEdgeList){
			if(e.source == startidx){ // proposal list
				if(proposalStartEnd[0].weight == 0 || (proposalStartEnd[0].weight > e.weight)){
					proposalStartEnd[0] = e;
				}
			}
			if(e.source == endidx){
				if(proposalStartEnd[1].weight == 0 || (proposalStartEnd[1].weight > e.weight)){
					proposalStartEnd[1] = e;
				}
			}
			
			if(e.source != endidx && e.source != startidx){
				if(best[e.source].weight == 0 || (best[e.source].weight > e.weight)){
					best[e.source] = e;
				}
			}
			
		}
		// Insert propositions into vector
		PrefixScanVector[2*Tid] = proposalStartEnd[0];
		PrefixScanVector[2*Tid+1] = proposalStartEnd[1];
	}
	std::cout << "Checkpoint 2 \n";
	// now do multiprefix scan

	int differentEdges = 0;
	multiPrefixScan(PrefixScanVector, differentEdges);

	// now insert found edges in parallel, we marked edges that we do not insert with weight = -1 
	// The first differentEdges entries of PrefixScanVec hold the edges we need

	#pragma omp parallel for
	for(int i = 0; i < differentEdges; i++){
		best[PrefixScanVector[i].source] = PrefixScanVector[i];
	}

	// next step is to contract all edges, this is done by a lookup table, where we do max log n steps
	// in order for all vertices to find their final parent -> this will be a runtime problem, if we dont adjust certain
	// details!!! 

	findParents(ParentVertex, best);

	// now we have a finished LookUpTable for parent vertices
	// as a next step we create the new edgeList

	// We loop thorugh the full edgelist and adjust the vertex indices
	
	for(int i = 0; i < edgelist.size(); i++){
		edgelist[i].source = ParentVertex[edgelist[i].source];
		edgelist[i].dest = ParentVertex[edgelist[i].dest];
	}

	for(auto e : best){
		mst.insert(e.idx);
	}


	
}

vector<edge> MinimumSpanningTreeBoruvkaPar(vector<edge> edgelist, int n, int m)
{
	omp_set_num_threads(numThreads);
	vector<edge> edgelistCopy = edgelist;
	// Actual Code
	set<int> mst;
	std::cout << m << ' ';
	// form MST
	int msShould = n;
	int t = 1;
	while (mst.size() < n-1)
	{
		
		BoruvkaStepPar(edgelist, mst, n, m);
		std::cout << mst.size() << ' ';
	}

	vector<edge> mst_res;

	for(auto e : mst){
		mst_res.push_back(edgelistCopy[e]);
	}


	return mst_res;
}
