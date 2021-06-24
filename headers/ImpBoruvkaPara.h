int steps = 4;

void checkEdgelist(vector<edge> edgelist, int n, int m){
	assert(m == edgelist.size() && "Edgelist has too many edges");
	int count = 0;
	for(auto e : edgelist){
		assert(e.dest < n && e.dest >= 0 && "Some edge is connected to a vertex that doesnt exist");
		assert(e.source < n && e.dest >= 0 && "Some edge is connected to a vertex that doesnt exist");
	}
}

void BoruvkaStepPrim(vector<edge> &edgelist, vector<int> &outgoingSizes, vector<int> ParentVertex, int &n, int &m, int numThreads, vector<int> &mstOneHot, int TotalN)
{
	// Datastructures
	vector<edge> best(n, edge(0,0,0,0));
	int stepsize = 10;//steps; // Finds 2 new edges!
	int nrStarters = 40;//numThreads;
	vector<pair<edge,int>> res((stepsize - 1) * nrStarters); // Found edge, start

	vector<int> pref;
	ParPrefixAnySize(pref,outgoingSizes,numThreads);
	
	startTimer1;
	#pragma omp parallel for
	for (int tr = 0; tr < nrStarters; tr++)
	{
		std::cout << tr*n/nrStarters << ' ';
		vector<pair<int,int>> visPairs;
		
		vector<pair<edge,int>> res1 = doKPrimSteps(visPairs,edgelist,pref,outgoingSizes,tr*n/nrStarters,stepsize,n); // edge in mst + starter
		for(int i = 0; i < res1.size(); i++){
			res[tr*(stepsize-1) + i] = res1[i];
		}
		
	}
	endTimer1;
	printTime1;
	int idle = 0;
	for(int i = 0; i < res.size(); i++){
		std::cout << res[i].first.source << "->" << res[i].first.dest<< ' ';
		if(res[i].first.source == res[i].first.dest){
			
			idle++;
		}
	}
	nn;
	std::cout << idle <<  " Idle edges this round\n";
	
	vector<edge> Usefuledges;
	ParentVertex = findParentsP(res,n,numThreads,Usefuledges);

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

	// Count different Indices
	outgoingSizes = newSizes;

	int foundedges = 0;
	//#pragma omp parallel for
	for (int i = 0; i < Usefuledges.size(); i++)
	{
		if (Usefuledges[i].weight > 0 && Usefuledges[i].source != Usefuledges[i].dest)
		{
			mstOneHot[Usefuledges[i].idx] = 1;
			foundedges++;
		}
	}
	std::cout << "We found: " << foundedges << " Edges this round\n";

	// Write edgelist to new Indices

	vector<edge> edgelist2 = edgelist;
	
	for (int i = 0; i < m; i++)
	{
		edgelist[i] = edgelist2[newIdx[i]];
		edgelist[i].source = ParentVertex[edgelist[i].source];
		edgelist[i].dest = ParentVertex[edgelist[i].dest];
		//edge e = edgelist2[newIdx[i]];
		//edgelist[i] = edge(ParentVertex[e.source],ParentVertex[e.dest],e.weight,e.idx);
		//std::cout << edgelist[i].source << ' ' << edgelist[i].dest;nn;
	}
	
	
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
	
	// delete Self edges, This can be done more efficient
	// cutEdgelist(edgelist, outgoingSizes ,m ,n ,numThreads);
}




vector<edge> ParBoruvkaImp(vector<edge> edgelist, vector<edge> edgelistSingle, vector<int> outgoingSizes, int n, int m, int numThreads, int cutoff)
{
	int stepsBeforeCutoff = 0;
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
	while (n > cutoff && n > steps)
	{
		stepsBeforeCutoff++;
		vector<int> ParentVertex(n);
	#pragma omp parallel for
		for (int i = 0; i < n; i++)
		{
			ParentVertex[i] = i;
		}
		//Check edgelist
		if(n == totalN)
		BoruvkaStepPrim(edgelist, outgoingSizes, ParentVertex, n, totalM, numThreads, mstSol, totalN);
		std::cout << "Edgelistsize: " << totalM << "\n";
		std::cout << "n is now: " << n << "\n";
		if(n > cutoff && n > steps){
			vector<int> ParentVertex1(n);
	#pragma omp parallel for
		for (int i = 0; i < n; i++)
		{
			ParentVertex1[i] = i;
		}
			ImpStep(edgelist, outgoingSizes, ParentVertex1, n, totalM, numThreads, mstSol, totalN);
		}
	}
	std::cout << "We have " << stepsBeforeCutoff << " Steps before cutoff\n";

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
