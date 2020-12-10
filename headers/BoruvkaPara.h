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

#define nn std::cout << "\n"

// Design variables
	int numThreads = 32;


void BoruvkaStepPar(vector<vector<edge>> &adjArr, vector<edge> &mst, int n, int m)
{
	// Divide Workload
	int workloadPerProcessor = 2 * m / numThreads;
	int workloadLastProcessor = 2 * m - ((numThreads - 1) * workloadPerProcessor);
	vector<int> prefix(n);
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
		int startidx = binSearchlow(prefix, startWL);
		vector<edge> localEdgeList;
		int edgeidx = prefix[startidx];

		for(int i = startidx; i < adjArr.size(); i++){
			for(int j = 0; j < adjArr[i].size(); j++){
				if(edgeidx >= startWL && edgeidx < endWL){
					localEdgeList.push_back(adjArr[i][j]);
				}
				if(edgeidx > endWL){
					goto finished;
				}
				edgeidx++;
			}
		}

		finished:
		std::cout << localEdgeList.size() << " " << endWL << "\n";
	}
}

vector<edge> MinimumSpanningTreeBoruvkaPar(vector<edge> edgelist, int n, int m)
{
	omp_set_num_threads(numThreads);

	// Actual Code
	vector<edge> mst;

	//create adjacency array --> can be parallelized
	vector<vector<edge>> adjArr = edgeListToAdjArray(edgelist, n);

	// form MST

	int t = 1;
	while (t--)
	{
		BoruvkaStepPar(adjArr, mst, n, m);
	}

	return mst;
}
