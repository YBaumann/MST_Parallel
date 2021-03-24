#pragma once

// void prefix(vector<int> &prefix, vector<int> adjArr) returns prefix in vector<int> prefix

// tuple<int,int,int> = ID -> Size -> index

void rewriteEdges(vector<tuple<int,int,int>> &arr, vector<int> &toRewrite){
	int n = arr.size();
	int m = toRewrite.size();
	vector<int> sizesOld(n);
	vector<int> sizesNew(n);
	vector<int> oldStartIndices(n);
	vector<int> newStartIndices(n);
	vector<int> sol(m);

	// get old number of outgoing edges per vertex
	for(int i = 0; i < n; i++){
		sizesOld[i] = get<1>(arr[i]);
	}

	// sort after supervertex
	sort(arr.begin(),arr.end());

	// get sizes of sorted vertices
	for(int i = 0; i < n; i++){
		sizesNew[i] = get<1>(arr[i]);
	}

	// Calculate starting index in edgelist
	prefix(oldStartIndices, sizesOld);
	prefix(newStartIndices, sizesNew);

	// Rewrite the edgelist 
	for(int i = 0; i < n; i++){
		int Idn = get<2>(arr[i]);
		int nrEdges = sizesOld[Idn];
		// Parallelize here
		for(int j = 0; j < nrEdges; j++){
			sol[newStartIndices[i] + j] = toRewrite[oldStartIndices[Idn] + j];
		}
	}
	toRewrite = sol;

	// Restore the number of outgoing vertices for the supervertices
	vector<int> countSupers(n);
	for(int i = 0; i < n-1; i++){
		if(get<0>arr[i] != get<0>arr[i+1]){
			countSupers = 1;
		}
	}
	prefix(countSupers);





}


