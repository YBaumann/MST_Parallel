#pragma once

// void prefix(vector<int> &prefix, vector<int> adjArr) returns prefix in vector<int> prefix

// tuple<int,int,int> = ID -> Size -> index

void rewriteEdges(vector<tuple<int,int,int>> &arr, vector<int> &toRewrite, vector<int> &newSizes){
	int n = arr.size();
	int m = toRewrite.size();
	int threadn = 32;
	
	// Set up all vectors needed
	vector<int> sizesOld(n);
	vector<int> sizesNewOrder(n);
	vector<int> oldStartIndices(n);
	vector<int> newStartIndices(n);
	vector<int> sol(m);

	// get old number of outgoing edges per vertex
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
	for(int i = 0; i < n; i++){
		int Idn = get<2>(arr[i]);
		int nrEdges = sizesOld[Idn];
		// Parallelize here
		for(int j = 0; j < nrEdges; j++){
			sol[newStartIndices[i] + j] = toRewrite[oldStartIndices[Idn] + j];
		}
	}

	toRewrite = sol;

	// calculate the new number of outgoing edges per supervertex
	newSizes = vector<int>(n);
	newSizes[0] = 1;

	for(int i = 1; i < n; i++){
		// Previous ID != This ID
		if(get<0>(arr[i-1]) != get<0>(arr[i])){
			newSizes[i] = 1;
		}
	}
	vector<int> prefNewSizes(n);

	// Get new Id for each Index
	ParPrefixAnySize(prefNewSizes, newSizes, threadn);

	// get number of supervertices for new edgelist
	int newSize = prefNewSizes[n-1]+1;


	// Write number of edges per supervertex into respective index
	vector<int> numEdgesBefore(newSize+1);

	for(int i = 1; i < n; i++){
		if(newSizes[i] == 1){
			numEdgesBefore[prefNewSizes[i]] = newStartIndices[i];
		}
	}
	
	numEdgesBefore[newSize] = toRewrite.size();

	std::cout << newSize;nn;

	newSizes = vector<int>(newSize);
	newSizes[0] = numEdgesBefore[0];

	// Calculate exact number of edges and return
	for(int i = 1; i < newSize+1; i++){
		newSizes[i-1] = numEdgesBefore[i] - numEdgesBefore[i-1];
	}

	


}


