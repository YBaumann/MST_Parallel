#pragma once

#include "ParallelPrefixSum.h"
#include "BinSearchLowIdx.h"
#include "findParents.h"
#include "MultiPrefixScanPara.h"
#include "EdgeListUpdateParallel.h"

auto TotalTime = 0;


void BoruvkaStepPar(vector<edge> &edgelist, vector<int> &ParentVertex, set<int> &mst, int &n, int m, int totalN, int numThreads)
{
	// Divide Workload per processor
	int workloadPerProcessor = 2 * m / numThreads;
	int workloadLastProcessor = 2 * m - ((numThreads - 1) * workloadPerProcessor);

	//Datastructures
	vector<int> prefix(totalN); // assumed to be empty
	vector<edge> best(totalN);
	vector<edge> PrefixScanVector(2*numThreads);

	
	vector<vector<edge>> adjArr = edgeListToAdjArray(edgelist, n, totalN);
	// To parallelize prefix we need some power 2 vector
	prefixSeq(prefix, adjArr); 
	
	
// now comes parallel part
#pragma omp parallel for ordered
	for (int tr = 0; tr < numThreads; tr++)
	{
		// organize my workload
		int Tid = omp_get_thread_num();
		int startWL = Tid * workloadPerProcessor;	  // included
		int endWL = (Tid + 1) * workloadPerProcessor; // not included
		if (Tid == numThreads - 1){
			endWL = 2 * m;
		}

		// find my workloadstart with binsearch
		int startidx = binSearchlow(prefix, startWL);
		
		// TODO prettyfy
		vector<edge> localEdgeList;
		int edgeidx = prefix[startidx];
		int endidx = startidx;
		int Asize = adjArr.size();
		for(int i = startidx; i < Asize; i++){
			int AsizeI = adjArr[i].size();
			for(int j = 0; j < AsizeI; j++){
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
			if(e.source == e.dest){continue; }
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
	
	
    
	
	// now do multiprefix scan, apparently really fast!
	int differentEdges = 0;
	multiPrefixScan(PrefixScanVector, differentEdges);


	// now insert found edges in parallel
	// The first differentEdges entries of PrefixScanVec hold the edges we need

	#pragma omp parallel for
	for(int i = 0; i < differentEdges; i++){
		best[PrefixScanVector[i].source] = PrefixScanVector[i];
	}

	// next step is to contract all edges, this is done by a lookup table, where we do max log n steps
	// in order for all vertices to find their final parent -> this will be a runtime problem, if we dont adjust certain
	// details!!! 

	
	findParents1(ParentVertex, best, n);

	// now we have a finished LookUpTable for parent vertices
	// as a next step we create the new edgeList

	// We loop thorugh the full edgelist and adjust the vertex indices
	
	
	UpdateEdgelist(edgelist, ParentVertex);

	// We can parallelize this
	for(int i = 0; i < totalN; i++){
		if(best[i].weight > 0){
			mst.insert(best[i].idx);
		}
	}
	
	
}

vector<edge> MinimumSpanningTreeBoruvkaPar(vector<edge> edgelist, int n, int m, int numThreads){
	
	// OMP Design
	assert(numThreads < n && "behaves strangely if more processors than nodes!");
	omp_set_num_threads(numThreads);

	// Generate copy, to not alter given edgelist
	// If we receive the Edges by some other format, we can do the transition here
	vector<edge> edgelistCopy = edgelist;
	set<int> mst;

	// form MST
	int totalN = n;

	// Create ParentVertices to signal, to which Super Vertex we belong
	vector<int> ParentVertex(totalN);
	#pragma omp parallel for
	for(int i = 0; i< totalN; i++){ParentVertex[i] = i;}

	int StepNr = 1;
	
	// Steps until only one vertex remains <-> Mst has size n-1
	while (n > 10){
		
		
		BoruvkaStepPar(edgelistCopy, ParentVertex, mst, n, m, totalN, numThreads);
    	
	}

	set<int> s;
	for(auto e : edgelistCopy){
		s.insert(e.dest); s.insert(e.source);
	}
	std::cout << s.size() << " here " << n;nn;

	vector<edge> mst_res = doSequentialCutoff(n, edgelistCopy);


	// This still runs in O(n)! 
	for(auto e : mst){
		mst_res.push_back(edgelist[e]);
	}


	std::cout << "Total time measured: " << TotalTime / 1000000.0;nn;
	
	return mst_res;
}
