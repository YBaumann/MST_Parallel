#pragma once

void checkEdgelist(vector<edge> edgelist, int n, int m){
	assert(m == edgelist.size() && "Edgelist has too many edges");
	int count = 0;
	for(auto e : edgelist){
		assert(e.dest < n && e.dest >= 0 && "Some edge is connected to a vertex that doesnt exist");
		assert(e.source < n && e.dest >= 0 && "Some edge is connected to a vertex that doesnt exist");
	}
}

void ImpStep(vector<edge> &edgelist, vector<int> &outgoingSizes, vector<int> ParentVertex, int &n, int m, int numThreads, set<int> &mst, int TotalN)
{

	// Datastructures
	vector<edge> best(n, edge(0,0,0,0));
	vector<edge> PrefixScanVector(2 * numThreads, edge(0,0,0,0));

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
		if (Tid == numThreads - 1)
		{
			endWL = edgelist.size();
		}
		// Find best edges for all vertices
		vector<edge> proposalStartEnd(2, edge(0,0,0,0)); // first entry is proposal for start, second entry is proposal for end
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
				if (proposalStartEnd[0] > e || proposalStartEnd[0].weight == 0)
				{
					proposalStartEnd[0] = e;
				}
			}
			else if (e.source == endVertexIndices)
			{ // Proposal coliding end
				if (proposalStartEnd[1] > e || proposalStartEnd[1].weight == 0)
				{
					proposalStartEnd[1] = e;
				}
			} else if (e.source < endVertexIndices && e.source > startVertexIndices)
			{ // non coliding
				if ((best[e.source] > e) || best[e.source].weight == 0)
				{
					best[e.source] = e;
				}
			}
		}

		// Insert propositions into vector
		PrefixScanVector[2 * tr] = proposalStartEnd[0];
		PrefixScanVector[2 * tr + 1] = proposalStartEnd[1];
	}

	// now do multiprefix scan, apparently really fast!
	int differentEdges = 0;
	multiPrefixScan(PrefixScanVector, differentEdges);

	// now insert found edges in parallel
	// The first differentEdges entries of PrefixScanVec hold the edges we need

	int InsertWeights = 0;
	nn;
	for (int i = 0; i < differentEdges; i++)
	{
		if(PrefixScanVector[i].weight > 0){
			best[PrefixScanVector[i].source] = PrefixScanVector[i];
		}
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

#pragma omp parallel for
	for (int i = 0; i < m; i++)
	{
		newIdx[i] = i;
	}

	// Get new Indices
	vector<int> newSizes;
	std::cout << "Timing for rewriting: \n";
	startTimer;
	rewriteVec(arr, newIdx, newSizes);
	endTimer;
	printTime;
	// Count different Indices
	outgoingSizes = newSizes;

	// Write egelist to new Indices
	vector<edge> edgelist2(edgelist.size());
	edgelist2 = edgelist;

#pragma omp parallel for
	for (int i = 0; i < edgelist.size(); i++)
	{
		edge e = edgelist2[newIdx[i]];
		e.source = ParentVertex[e.source];
		e.dest = ParentVertex[e.dest];
		edgelist[i] = e;
	}

	// Insert found edges into mst
	for (int i = 0; i < best.size(); i++)
	{
		if (best[i].weight > 0)
		{
			mst.insert(best[i].idx);
		}
	}

	// Create map to new vertex ids -> TODO Parallelize
	vector<int> mapper(n);
	n = newSizes.size();
	long long sum = 0;

	for (int i = 0; i < outgoingSizes.size(); i++)
	{
		if(outgoingSizes[i] > 0){
		mapper[edgelist[sum].source] = i;
		sum += outgoingSizes[i];
		}
	}
	// Rename Edgelist again

#pragma omp parallel for
	for (int i = 0; i < edgelist.size(); i++)
	{
		edgelist[i].source = mapper[edgelist[i].source];
		edgelist[i].dest = mapper[edgelist[i].dest];
	}
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
		
		vector<int> ParentVertex(n);
		for (int i = 0; i < n; i++)
		{
			ParentVertex[i] = i;
		}
		//Check edgelist
		ImpStep(edgelist, outgoingSizes, ParentVertex, n, m, numThreads, mst, totalN);
		//std::cout << n << ' ';
	}

	std::cout << "Time for MST Writeback:\n";
	startTimer;
	vector<edge> mst_res;
	for (int i = 0; i < edgelistcpy.size(); i++)
	{
		if (mst.count(edgelistcpy[i].idx) && edgelistcpy[i].source < edgelistcpy[i].dest)
		{
			mst_res.push_back(edgelistcpy[i]);
		}
	}
	endTimer;
	printTime;

	std::cout << "MST_Size: " << mst_res.size();
	nn;

	return mst_res;
}
