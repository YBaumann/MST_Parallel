void ImpStep(vector<edge> &edgelist, vector<int> &outgoingSizes, vector<int> ParentVertex, int &n, int &m, int numThreads, vector<int> &mstOneHot, int TotalN)
{
	// Datastructures
	vector<edge> best(n, edge(0,0,0,0));
	vector<edge> PrefixScanVector(2 * numThreads, edge(0,0,0,0));

	// Divide Workload per processor
	int workloadPerProcessor = m / numThreads;
	int workloadLastProcessor = m - ((numThreads - 1) * workloadPerProcessor);
	// Find best edge for each vector

	TS;
#pragma omp parallel for
	for (int tr = 0; tr < numThreads; tr++)
	{
		int Tid = tr;
		int startWL = Tid * workloadPerProcessor;	  // included
		int endWL = (Tid + 1) * workloadPerProcessor; // not included
		if (Tid == numThreads - 1)
		{
			endWL = m;
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
	TE;
	timesMap["Calc Best"] += gT;

	// now do multiprefix scan, apparently really fast!

	TS;
	int differentVertices = 0;
	multiPrefixScan(PrefixScanVector, differentVertices);
	// now insert found edges in parallel
	// The first differentEdges entries of PrefixScanVec hold the edges we need
	TE;
	timesMap["Multi Pref"] += gT;

	int InsertWeights = 0;
	TS;

#pragma omp parallel for
	for (int i = 0; i < differentVertices; i++)
	{
		if(PrefixScanVector[i].weight > 0){
			best[PrefixScanVector[i].source] = PrefixScanVector[i];
		}
	}

	// Find Parent Vertex of all vertices
	int newn = n;
	findParents1(ParentVertex, best, newn);
	TE;
	timesMap["Find Parents"] += gT;

	
	// Setup vector to rewrite
	TS;
	vector<tuple<int, int, int>> arr(n); // tuple<int,int,int> = ID -> Size -> index
#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		arr[i] = make_tuple(ParentVertex[i], outgoingSizes[i], i);
	}

	// Get new Indices
	
	vector<int> newIdx(m);
	vector<int> newSizes;
	newSizes.reserve(m);
	
	rewriteVecIMP(arr, newIdx, newSizes, numThreads,m,edgelist,ParentVertex);
	TE;
	timesMap["Rewrite Vector"] += gT;

	// Count different Indices
	outgoingSizes = newSizes;

	// Insert found edges into mst
	TS;
	#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		if (best[i].weight > 0)
		{
			mstOneHot[best[i].idx] = 1;
		}
	}
	TE;
	timesMap["Insert into MST"] += gT;

	// Write edgelist to new Indices


	// Create map to new vertex ids -> TODO Parallelize
	TS;
	vector<int> mapper(n);
	n = newSizes.size();
	//int sum = 0;
	vector<int> prefSum;
	ParPrefixAnySize(prefSum,outgoingSizes,numThreads);
	for (int i = 0; i < outgoingSizes.size(); i++)
	{
		if(outgoingSizes[i] > 0){
		    mapper[edgelist[prefSum[i]].source] = i;
		}
	}
	// Rename Edgelist again
#pragma omp parallel for
	for (int i = 0; i < m; i++)
	{
		edgelist[i].source = mapper[edgelist[i].source];
		edgelist[i].dest = mapper[edgelist[i].dest];
		
	}
	TE;
	
	timesMap["Resort edgelist"] += gT;
	

	// delete Self edges, This can be done more efficient
	// cutEdgelist(edgelist, outgoingSizes ,m ,n ,numThreads);

}