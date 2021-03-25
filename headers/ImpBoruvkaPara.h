#pragma once

// void prefix(vector<int> &prefix, vector<int> adjArr) returns prefix in vector<int> prefix

// pair<int,int> = ID -> Size -> index

void rewriteVec(vector<tuple<int,int,int>> &arr, vector<int> &toRewrite, vector<int> &newSizes){ // ID -> Size -> index
	int threadn = 32;
	int n = arr.size();
	int m = toRewrite.size();

	// Set up all vectors needed
	vector<int> sizesOld(n);
	vector<int> sizesNewOrder(n);
	vector<int> oldStartIndices(n);
	vector<int> newStartIndices(n);
	vector<int> sol(m);
	for(int i = 0; i < n; i++){
		sizesOld[i] = get<1>(arr[i]);
	}

	// Parallelize this later
	sort(arr.begin(),arr.end());

	for(int i = 0; i < n; i++){
		sizesNewOrder[i] = get<1>(arr[i]);
	}

	ParPrefixAnySize(oldStartIndices, sizesOld, threadn);
	ParPrefixAnySize(newStartIndices, sizesNewOrder, threadn);

	// calculate the indices for each edge of the new edgelist
#pragma omp parallel for
	for(int i = 0; i < n; i++){
		int Idn = get<2>(arr[i]);
		int nrEdges = sizesOld[Idn];
		for(int j = 0; j < nrEdges; j++){
			sol[newStartIndices[i] + j] = toRewrite[oldStartIndices[Idn] + j];
		}
	}

	toRewrite = sol;

	// calculate the new number of outgoing edges per supervertex
	newSizes = vector<int>(n);

	for(int i = 1; i < n-1; i++){
		// Previous ID != This ID
		if(get<0>(arr[i-1]) != get<0>(arr[i])){
			newSizes[i] = 1;
		}
	}
	vector<int> prefNewSizes(n);

	// Get new Id for each Index
	ParPrefixAnySize(prefNewSizes, newSizes, threadn);

	// get number of vertices in edgelist
	int newSize = prefNewSizes[n-1] + 1;
	

	// Write number of edges per supervertex into respective index
	vector<int> numEdgesBefore(newSize);

	for(int i = 0; i < n; i++){
		if(newSizes[i] == 1){
			numEdgesBefore[prefNewSizes[i]] = newStartIndices[i];
		}
	}
	numEdgesBefore[newSize-1] = toRewrite.size();

	newSizes = vector<int>(newSize);
	newSizes[0] = numEdgesBefore[0];

	// Calculate exact number of edges and return
	for(int i = 1; i < newSize; i++){
		newSizes[i] = numEdgesBefore[i] - numEdgesBefore[i-1];
	}
	


}


void ImpStep(vector<edge> edgelist, vector<int> outgoingSizes, int n, int m, int numThreads, set<int> mst){

	// Datastructures
	vector<edge> best(n);
	vector<edge> PrefixScanVector(2*numThreads);

	// Divide Workload per processor
	int workloadPerProcessor = m / numThreads;
	int workloadLastProcessor = m - ((numThreads - 1) * workloadPerProcessor);
	
	// Find best edge for each vector
	for(int tr = 0; tr < numThreads; tr++){
		int Tid = tr;
		int startWL = Tid * workloadPerProcessor;	  // included
		int endWL = (Tid + 1) * workloadPerProcessor; // not included
		std::cout << startWL << ' ' << endWL;nn;
		if (Tid == numThreads - 1){
			endWL = 2 * m;
		}
	

		// now each thread has its own local edgelist ready
		// next step is, that each thread proposes a best edge for all vertices it looks at
		// if a processor has a best edge for a vertex that is neither start nor end, then it can write it to bestedges regardless
		vector<edge> proposalStartEnd(2); // first entry is proposal for start, second entry is proposal for end
		for(int i = startWL; i < endWL; i++){
			edge e = edgelist[i];
			if(e.source == e.dest){continue; }
			if(e.source == startWL){ // proposal list
				if(proposalStartEnd[0].weight == 0 || (proposalStartEnd[0].weight > e.weight)){
					proposalStartEnd[0] = e;
				}
			}
			if(e.source == endWL){
				if(proposalStartEnd[1].weight == 0 || (proposalStartEnd[1].weight > e.weight)){
					proposalStartEnd[1] = e;
				}
			}
			if(e.source != endWL && e.source != startWL){
				if(best[e.source].weight == 0 || (best[e.source].weight > e.weight)){
					best[e.source] = e;
				}
			}
		}
		

		PrefixScanVector[2*Tid] = proposalStartEnd[0];
		PrefixScanVector[2*Tid+1] = proposalStartEnd[1];

		// now do multiprefix scan, apparently really fast!
		int differentEdges = 0;
		multiPrefixScan(PrefixScanVector, differentEdges);

		#pragma omp parallel for
		for(int i = 0; i < differentEdges; i++){
			best[PrefixScanVector[i].source] = PrefixScanVector[i];
		}

		for(int i = 0; i < best.size(); i++){
			std::cout << best[i].source << ' ';
		}
		nn;
		



	}
	std::cout << "finishes";

}


// ParBoruvkaImp(sol, edgelist, outgoingEdges, n, m)
vector<edge> ParBoruvkaImp(vector<edge> edgelist, vector<int> outgoingSizes, int n, int m, int numThreads){
	// OMP Design
	assert(numThreads < n && "behaves strangely if more processors than nodes!");
	omp_set_num_threads(numThreads);
	// get copy of edgelist

	set<int> mst;

	// Steps until only one vertex remains <-> Mst has size n-1
	while(n > 1){
		ImpStep(edgelist, outgoingSizes, n, m, numThreads, mst);
		n = 1;
	}

	vector<edge> mst_res = vector<edge>();
	
	return mst_res;

}

