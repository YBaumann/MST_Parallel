#pragma once
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <cassert>
#include <omp.h>
#include <chrono>


struct CSR_Matrix{
    int size;
    int rows;
    vector<int> row_idx;
    vector<int> col_idx;
    vector<double> val;
};


// We convert to sorted by source ascending edgelist
// This resembles setFromTriplets
void CSRtoEdgelist(struct CSR_Matrix &csr, vector<vector<edge>> &adjArr){
    int n = csr.size;
    int rows = csr.rows;
    vector<int> OutgoingEdges(n);

    #pragma omp parallel for
    for(int i = 0; i < rows-1; i++){
        OutgoingEdges[i] = csr.row_idx[i+1] - csr.row_idx[i];
        vector<edge> f(OutgoingEdges[i]);
        for(int j = csr.row_idx[i]; j < csr.row_idx[i+1]; j++){
            edge e; e.source = i; e.dest = csr.col_idx[j]; e.weight = csr.val[j]; e.idx = j;
            f[j - csr.row_idx[i]] = e;
        }
        adjArr[i] = f;
    }

}

void EdgelisttoCSR(vector<vector<edge>> &adjArr, struct CSR_Matrix &csr){



}