#pragma once

void checkEdgelist(vector<edge> edgelist, int n, int m){
	assert(m == edgelist.size() && "Edgelist has too many edges");
	int count = 0;
	for(auto e : edgelist){
		assert(e.dest < n && e.dest >= 0 && "Some edge is connected to a vertex that doesnt exist");
		assert(e.source < n && e.dest >= 0 && "Some edge is connected to a vertex that doesnt exist");
	}
}

void ImpStep(vector<edge> &edgelist, vector<int> &outgoingSizes, vector<int> &ParentVertex, int &n, int m, int numThreads, set<int> &mst, int TotalN)
{

	// Datastructures
	vector<edge> best(TotalN);
	vector<edge> PrefixScanVector(2 * numThreads);

	// Divide Workload per processor
	int workloadPerProcessor = m / numThreads;
	int workloadLastProcessor = m - ((numThreads - 1) * workloadPerProcessor);

	// Find best edge for each vector
#pragma omp parallel for
	for (int tr = 0; tr < numThreads; tr++)
	{
		int Tid = tr;
		int startWL = Tid * workloadPerProcessor;	  // included
		int endWL = (Tid + 1) * workloadPerProcessor; // not included
		std::cout << startWL << ' ' << endWL;
		nn;
		if (Tid == numThreads - 1)
		{
			endWL = edgelist.size();
		}

		// Find best edges for all vertices
		vector<edge> proposalStartEnd(2); // first entry is proposal for start, second entry is proposal for end
		for (int i = startWL; i < endWL; i++)
		{
			edge e = edgelist[i];

			// Find Vertices that might collide
			int startVertexIndices = edgelist[startWL].source;
			int endVertexIndices = edgelist[endWL - 1].source;

			// Compute best edges for my local vertices
			if (e.source == e.dest)
			{
				continue;
			}

			if (e.source == startVertexIndices)
			{ // proposal coliding start
				if (proposalStartEnd[0].weight == 0 || (proposalStartEnd[0] > e))
				{
					proposalStartEnd[0] = e;
				}
			}
			if (e.source == endVertexIndices)
			{ // Proposal coliding end
				if (proposalStartEnd[1].weight == 0 || (proposalStartEnd[1] > e))
				{
					proposalStartEnd[1] = e;
				}
			}
			if (e.source != endVertexIndices && e.source != startVertexIndices)
			{ // non coliding
				if (best[e.source].weight == 0 || (best[e.source] > e))
				{
					best[e.source] = e;
				}
			}
		}

		// Insert propositions into vector
		PrefixScanVector[2 * Tid] = proposalStartEnd[0];
		PrefixScanVector[2 * Tid + 1] = proposalStartEnd[1];
	}

	// now do multiprefix scan, apparently really fast!
	int differentEdges = 0;
	multiPrefixScan(PrefixScanVector, differentEdges);

	// now insert found edges in parallel
	// The first differentEdges entries of PrefixScanVec hold the edges we need

#pragma omp parallel for
	for (int i = 0; i < differentEdges; i++)
	{
		best[PrefixScanVector[i].source] = PrefixScanVector[i];
	}

	// Find Parent Vertex of all vertices
	int newn = n;
	findParents1(ParentVertex, best, newn);

	// Setup vector to rewrite
	vector<tuple<int, int, int>> arr(n); // tuple<int,int,int> = ID -> Size -> index
	for (int i = 0; i < arr.size(); i++)
	{
		arr[i] = make_tuple(ParentVertex[i], outgoingSizes[i], i);
	}
	vector<int> newIdx(m);
	for (int i = 0; i < m; i++)
	{
		newIdx[i] = i;
	}

	// Get new Indices
	vector<int> newSizes;
	rewriteVec(arr, newIdx, newSizes);

	outgoingSizes = newSizes;

	// Write egelist to new Indices
	vector<edge> edgelist2(edgelist.size());
	edgelist2 = edgelist;

	for (int i = 0; i < edgelist.size(); i++)
	{
		edgelist[i] = edgelist2[newIdx[i]];
	}

	// Rename edges, to their supervertex
	for (int i = 0; i < edgelist.size(); i++)
	{
		edgelist[i].source = ParentVertex[edgelist[i].source];
		edgelist[i].dest = ParentVertex[edgelist[i].dest];
	}

	// Insert found edges into mst

	for (int i = 0; i < best.size(); i++)
	{
		if (best[i].weight > 0)
		{
			mst.insert(best[i].idx);
		}
	}
	nn;

	// Create map to new vertex ids -> TODO Parallelize
	vector<int> mapper(n);
	n = newn;
	long long sum = 0;
	std::cout << "Gets to mapping\n";
	for (int i = 0; i < outgoingSizes.size(); i++)
	{
		assert(sum < edgelist.size() && "Sum was too big");
		// assert(mapper.size() > edgelist[sum].source && "Mapper didnt cover all remaining vertices!");
		if(TotalN <= edgelist[sum].source){
			std::cout << "Outgoing Size: " << outgoingSizes.size();nn;
			std::cout << "Mapper Size: " << mapper.size();nn;
			std::cout << "edgelist[sum].source: " << edgelist[sum].source; nn;
			std::cout << "i: " << i;
			exit(1);
		}
		if(outgoingSizes[i] > 0){
		mapper[edgelist[sum].source] = i;
		sum += outgoingSizes[i];
		}
	}

	// Print mapping
	std::cout << "n: " << n << "\nPrint Mapping: \n";
	for(int i = 0; i < mapper.size(); i++){
		if(mapper[i] > 0){
		//std::cout << mapper[i] << ' ';
		}
	}
	nn;

	std::cout << "Gets to rename edgelist\n";
	// Rename Edgelist again
	for (int i = 0; i < edgelist.size(); i++)
	{
		edgelist[i].source = mapper[edgelist[i].source];
		edgelist[i].dest = mapper[edgelist[i].dest];
	}
	std::cout << "fininshes Step\n";
}

// ParBoruvkaImp(sol, edgelist, outgoingEdges, n, m)
vector<edge> ParBoruvkaImp(vector<edge> edgelist, vector<int> outgoingSizes, int n, int m, int numThreads)
{
	// OMP Design
	assert(numThreads < n && "behaves strangely if more processors than nodes!");
	omp_set_num_threads(numThreads);

	std::cout << "Before Copy of edgelist\n";
	// get copy of edgelist
	vector<edge> edgelistcpy = edgelist;
	set<int> mst;
	int totalN = n;

	// Steps until only one vertex remains <-> Mst has size n-1
	while (n > 1)
	{
		std::cout << "Remaining Vertices: " << n;nn;
		vector<int> ParentVertex(n);
		for (int i = 0; i < n; i++)
		{
			ParentVertex[i] = i;
		}
		// Check edgelist
		checkEdgelist(edgelist, n, m);
		ImpStep(edgelist, outgoingSizes, ParentVertex, n, m, numThreads, mst, totalN);
	}

	vector<edge> mst_res;
	for (int i = 0; i < edgelistcpy.size(); i++)
	{
		if (mst.count(edgelistcpy[i].idx) && edgelistcpy[i].source < edgelistcpy[i].dest)
		{
			mst_res.push_back(edgelistcpy[i]);
		}
	}

	return mst_res;
}
