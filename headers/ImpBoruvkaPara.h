
void checkEdgelist(vector<edge> edgelist, int n, int m){
	assert(m == edgelist.size() && "Edgelist has too many edges");
	int count = 0;
	for(auto e : edgelist){
		assert(e.dest < n && e.dest >= 0 && "Some edge is connected to a vertex that doesnt exist");
		assert(e.source < n && e.dest >= 0 && "Some edge is connected to a vertex that doesnt exist");
	}
}

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
	
	rewriteVec(arr, newIdx, newSizes, numThreads,m);
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
	TS;
	vector<edge> edgelist2 = edgelist;
	
#pragma omp parallel for
	for (int i = 0; i < m; i++)
	{
		edgelist[i] = edgelist2[newIdx[i]];
		edgelist[i].source = ParentVertex[edgelist[i].source];
		edgelist[i].dest = ParentVertex[edgelist[i].dest];
		//edge e = edgelist2[newIdx[i]];
		//edgelist[i] = edge(ParentVertex[e.source],ParentVertex[e.dest],e.weight,e.idx);

	}
	TE;
	timesMap["Rename to Supervertex"] += gT;


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
		edge e = edgelist[i];
	}
	TE;
	
	timesMap["Resort edgelist"] += gT;
	

	// delete Self edges, This can be done more efficient
	// cutEdgelist(edgelist, outgoingSizes ,m ,n ,numThreads);

}




vector<edge> ParBoruvkaImp(vector<edge> edgelist, vector<edge> edgelistSingle, vector<int> outgoingSizes, int n, int m, int numThreads, int cutoff)
{
	omp_set_num_threads(numThreads);

	// We store the edges we need in our mst here. At this pount m will always be even!
	vector<int> mstSol(m/2);


	// OMP Design
	assert(numThreads < n && m == edgelist.size() &&  "behaves strangely if more processors than nodes!");

	std::cout << "Before Copy of edgelist\n";
	// get copy of edgelist
	vector<edge> edgelistcpy = edgelist;
	int totalN = n;
	int totalM = m;
	std::cout << "Edgelistsize: " << m;nn;
	// Steps until only one vertex remains <-> Mst has size n-1
	while (n > cutoff)
	{

		vector<int> ParentVertex(n);
	#pragma omp parallel for
		for (int i = 0; i < n; i++)
		{
			ParentVertex[i] = i;
		}
		//Check edgelist
		ImpStep(edgelist, outgoingSizes, ParentVertex, n, totalM, numThreads, mstSol, totalN);
		std::cout << "Edgelistsize: " << totalM << "\n";
	}

	TS;
	// Sequential Cutoff
	vector<edge> sols = MinimumSpanningTreeBoruvkaSeq(edgelist, n, m);

#pragma omp parallel for
	for(int i = 0; i < sols.size(); i++){
		mstSol[sols[i].idx] = 1;
	}
	TE;
	timesMap["cutoff"] += gT;



	TS;
	vector<edge> mst_res(totalN-1);
	vector<int> mst_Indices;

	ParPrefixAnySize(mst_Indices, mstSol ,numThreads);
#pragma omp parallel for
	for(int i = 0; i <  totalM / 2 ;i++){
		if(mstSol[i] == 1){
			mst_res[mst_Indices[i]] = edgelistSingle[i];
		}
	}
	TE;
	timesMap["Return MST"] += gT;


	return mst_res;
}
