#pragma once

// void prefix(vector<int> &prefix, vector<int> adjArr) returns prefix in vector<int> prefix

// pair<int,int> = (id,size)

void rewriteVec(vector<tuple<int,int,int>> &arr, vector<int> &toRewrite){ // ID -> Size -> index
	int threadn = 32;
	int n = arr.size();
	int m = toRewrite.size();
	vector<int> sizesOld(n);
	vector<int> sizesNew(n);
	vector<int> oldStartIndices(n);
	vector<int> newStartIndices(n);
	vector<int> sol(m);
	for(int i = 0; i < n; i++){
		sizesOld[i] = get<1>(arr[i]);
	}
	sort(arr.begin(),arr.end());
	for(int i = 0; i < n; i++){
		sizesNew[i] = get<1>(arr[i]);
	}

	ParPrefixAnySize(oldStartIndices, sizesOld, threadn);
	ParPrefixAnySize(newStartIndices, sizesNew, threadn);

#pragma omp parallel for
	for(int i = 0; i < n; i++){
		int Idn = get<2>(arr[i]);
		int nrEdges = sizesOld[Idn];
		for(int j = 0; j < nrEdges; j++){
			sol[newStartIndices[i] + j] = toRewrite[oldStartIndices[Idn] + j];
		}
	}

	toRewrite = sol;


	nn;
}


