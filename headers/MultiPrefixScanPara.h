#include <stdio.h>
#include <stdlib.h>

#include "Structures.h"

/*

We give a parallel implementation of a multi prefix scan of edges

Work = O(n)
depth = O(log(n))

We receive a vector of edges, on which we do a multiprefix scan and return the vector scanned
e.source = key
e.weight = value
we search for smallest value

*/

void merge(vector<edge> &list1, int idx1, vector<edge> &list2, int idx2, vector<int> &sizes){
    // Runs in O(p)
    if(list1[sizes[idx1]-1].source == list2[0].source){
        list1[sizes[idx1]-1] = list1[sizes[idx1]-1].weight <= list2[0].weight ? list1[sizes[idx1]-1] : list2[0];
        for(int i = 1; i < sizes[idx2]; i++){
            list1[sizes[idx1]++] = list2[i];
        }
    } else {
        for(int i = 0; i < sizes[idx2]; i++){
            list1[sizes[idx1]++] = list2[i];
        }
    }
}



void doScan(vector<vector<edge>> &ArrOfLists, vector<int> &sizes){
    int n = sizes.size();
    int jDist = 1;

    while(jDist < n){
        #pragma omp parallel for
        for(int i = 0; i < n - jDist; i += 2*jDist){
            merge(ArrOfLists[i], i, ArrOfLists[i + jDist], i+jDist, sizes);
        }
        jDist *= 2;
    }
}



void multiPrefixScan(vector<edge> &arr, int &foundSize)
{
    int arrLen = arr.size();
    std::cout << "print arr:\n";
    for(int i = 0; i < arrLen; i++){
        std::cout << arr[i].source <<' ';
    }
    std::cout << arrLen;nn;
    vector<vector<edge>> ArrOfLists;
    ArrOfLists.reserve(arrLen*arrLen);
    std::cout << "Reaches to for\n";
    vector<int> sizes(arrLen,1);


    
    for(int i = 0; i < arrLen; i++){
        vector<edge> f(arrLen);
        f[0] = arr[i];
        // ArrOfLists[i] = f;
        ArrOfLists.push_back(f);
    }
    std::cout << "Reach doScan\n";

    doScan(ArrOfLists, sizes); // result is saved in position 0 of arrOfLists

    foundSize = sizes[0];
    arr = ArrOfLists[0];
}