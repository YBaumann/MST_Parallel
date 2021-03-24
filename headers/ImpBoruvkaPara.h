#pragma once

// void prefix(vector<int> &prefix, vector<int> adjArr) returns prefix in vector<int> prefix

<<<<<<< HEAD
// tuple<int,int,int> = ID -> Size -> index

void rewriteEdges(vector<tuple<int,int,int>> &arr, vector<int> &toRewrite){
=======
// pair<int,int> = ID -> Size -> index

void rewriteVec(vector<tuple<int,int,int>> &arr, vector<int> &toRewrite, vector<int> &newSizes){ // ID -> Size -> index
	int threadn = 32;
>>>>>>> 32bd3999238e14225b5378ab77809e3b21c5761a
	int n = arr.size();
	int m = toRewrite.size();

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

<<<<<<< HEAD
	// sort after supervertex
	sort(arr.begin(),arr.end());

	// get sizes of sorted vertices
=======
	// Parallelize this later
	sort(arr.begin(),arr.end());

>>>>>>> 32bd3999238e14225b5378ab77809e3b21c5761a
	for(int i = 0; i < n; i++){
		sizesNewOrder[i] = get<1>(arr[i]);
	}

<<<<<<< HEAD
	// Calculate starting index in edgelist
	prefix(oldStartIndices, sizesOld);
	prefix(newStartIndices, sizesNew);

	// Rewrite the edgelist 
=======
	ParPrefixAnySize(oldStartIndices, sizesOld, threadn);
	ParPrefixAnySize(newStartIndices, sizesNewOrder, threadn);

	// calculate the indices for each edge of the new edgelist
#pragma omp parallel for
>>>>>>> 32bd3999238e14225b5378ab77809e3b21c5761a
	for(int i = 0; i < n; i++){
		int Idn = get<2>(arr[i]);
		int nrEdges = sizesOld[Idn];
		// Parallelize here
		for(int j = 0; j < nrEdges; j++){
			sol[newStartIndices[i] + j] = toRewrite[oldStartIndices[Idn] + j];
		}
	}
	toRewrite = sol;

<<<<<<< HEAD
	// Restore the number of outgoing vertices for the supervertices
	vector<int> countSupers(n);
	for(int i = 0; i < n-1; i++){
		if(get<0>arr[i] != get<0>arr[i+1]){
			countSupers = 1;
		}
	}
	prefix(countSupers);



=======
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
	
>>>>>>> 32bd3999238e14225b5378ab77809e3b21c5761a


}


