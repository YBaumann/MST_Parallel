#pragma once
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <chrono>
#include <omp.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;



vector<pair<edge,int>> doKPrimSteps(vector<pair<int,int>> &visitedPairs, vector<edge> edgelist, vector<int> prefixSizes, vector<int> sizes, int start, int k, int n){
    
    vector<int> visited(n,-1);
    vector<pair<edge,int>> res;
    visited[start] = 0;
    
    priority_queue<tuple<int,int,int>> pq; // distance, vertex, edge Index

    pq.push(make_tuple(0,start,-1));
    tuple<int,int,int> p; // distance, vertex, edge idx
    //std::cout << "starts1\n";
    while(k > 0){
        do{
        p = pq.top(); pq.pop();
        } while(visited[get<1>(p)] > 0);
        k--;
        if(get<2>(p) >= 0){
            edge q = edgelist[get<2>(p)];
            res.push_back(make_pair(edgelist[get<2>(p)],start)); // edge and starting vertex
        }
        visited[get<1>(p)] = -get<0>(p);
        visitedPairs.push_back(make_pair(get<1>(p),start));
        
        for(int i = prefixSizes[get<1>(p)]; i < prefixSizes[get<1>(p)] + sizes[get<1>(p)]; i++){
            edge e = edgelist[i];
            if(visited[e.dest] == -1){
                pq.push(make_tuple(-e.weight, e.dest,i));
            }
        }
        
    }
    
    //std::cout << "returns\n";
    return res;

}