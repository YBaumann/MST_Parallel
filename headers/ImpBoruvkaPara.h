#pragma once

void ImpStep(vector<edge> &edgelist, vector<int> outgoingSizes, vector<int> &ParentVertex, int &n, int m, int numThreads, set<int> &mst)
{

	// Datastructures
	vector<edge> best(n);
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
			endWL = m;
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
				if (proposalStartEnd[0].weight == 0 || (proposalStartEnd[0].weight > e.weight))
				{
					proposalStartEnd[0] = e;
				}
			}
			if (e.source == endVertexIndices)
			{ // Proposal coliding end
				if (proposalStartEnd[1].weight == 0 || (proposalStartEnd[1].weight > e.weight))
				{
					proposalStartEnd[1] = e;
				}
			}
			if (e.source != endVertexIndices && e.source != startVertexIndices)
			{ // non coliding
				if (best[e.source].weight == 0 || (best[e.source].weight > e.weight))
				{
					best[e.source] = e;
				}
			}
		}

		// Insert propositions into vector
		PrefixScanVector[2*Tid] = proposalStartEnd[0];
		PrefixScanVector[2*Tid+1] = proposalStartEnd[1];
		std::cout << "Propend: " << proposalStartEnd[1].source;nn;
	}

	// now do multiprefix scan, apparently really fast!
	std::cout << "Start Multi Prefix\n";

	nn;
	int differentEdges = 0;
	multiPrefixScan(PrefixScanVector, differentEdges);
	std::cout << "Leaves multiP\n";

	// now insert found edges in parallel
	// The first differentEdges entries of PrefixScanVec hold the edges we need

#pragma omp parallel for
	for (int i = 0; i < differentEdges; i++)
	{
		best[PrefixScanVector[i].source] = PrefixScanVector[i];
	}

	// Find Parent Vertex of all vertices
	std::cout << "Find Parents\n";
	int newn = n;
	findParents1(ParentVertex, best, newn);

	// Setup vector to rewrite
	vector<tuple<int, int, int>> arr(n); // tuple<int,int,int> = ID -> Size -> index
	for (int i = 0; i < n; i++)
	{
		arr[i] = make_tuple(ParentVertex[i], outgoingSizes[i], i);
	}
	vector<int> newIdx(m);
	for (int i = 0; i < m; i++)
	{
		newIdx[i] = i;
	}
	vector<int> newSizes;

	// Get new Indices
	rewriteVec(arr, newIdx, newSizes);
	n = newSizes.size();

	// Write egelist to new Indices
	vector<edge> edgelist2(edgelist.size());

	for (int i = 0; i < edgelist.size(); i++)
	{
		edgelist2[i] = edgelist[newIdx[i]];
	}
	edgelist = edgelist2;
	for (int i = 0; i < edgelist.size(); i++)
	{
		edgelist[i].source = ParentVertex[edgelist2[i].source];
		edgelist[i].dest = ParentVertex[edgelist2[i].dest];
	}
	

	// Insert found edges into mst
	for (int i = 0; i < best.size(); i++)
	{
		if (best[i].weight > 0)
		{
			mst.insert(best[i].idx);
		}
	}

	std::cout << "finishes loop\n";
	for(int i = 0; i < edgelist.size(); i++){
		std::cout << edgelist[i].source << " " << edgelist[i].dest;nn;
	}
}

// ParBoruvkaImp(sol, edgelist, outgoingEdges, n, m)
vector<edge> ParBoruvkaImp(vector<edge> edgelist, vector<int> outgoingSizes, int n, int m, int numThreads)
{
	// OMP Design
	assert(numThreads < n && "behaves strangely if more processors than nodes!");
	omp_set_num_threads(numThreads);

	// get copy of edgelist
	vector<edge> edgelistcpy = edgelist;
	set<int> mst;

	// Create ParentVertex Vector
	vector<int> ParentVertex(n);
	for (int i = 0; i < n; i++)
	{
		ParentVertex[i] = i;
	}

	// Steps until only one vertex remains <-> Mst has size n-1
	while (n > 1)
	{
		ImpStep(edgelist, outgoingSizes, ParentVertex, n, m, numThreads, mst);
		std::cout << "Remaining vertices: " << n;
		nn;
	}

	std::cout << mst.size() << " Get to return\n";
	vector<edge> mst_res;
	for (auto e : mst)
	{
		mst_res.push_back(edgelistcpy[e]);
	}

	return mst_res;
}
