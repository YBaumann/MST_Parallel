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


#define nn std::cout << "\n"

using namespace std;


// we need to first convert the edgelist to adjmatrix format

vector<edge> MinimumSpanningTreePrimSeq(vector<edge> edgelist, int n, int m) {
	vector<edge> mst;
	vector<vector<tuple<int,int,int>>> adjlist; // We could also have it set up as a normal adjlist...
	for (int i = 0; i < n; i++) {
		vector<tuple<int,int,int>> f;
		adjlist.push_back(f);
	}

	for (auto e : edgelist) {
		adjlist[e.source].push_back(make_tuple(-e.weight,e.dest,e.idx)); // make this so that prio queue has the correct ordering
		adjlist[e.dest].push_back(make_tuple(-e.weight,e.source,e.idx));
	}
	std::cout << "Checkpoint creation adjlist"; nn;
	vector<int> included(n);
	included[0] = 1;
	priority_queue<tuple<int,int, int>> pq;
	for(auto p : adjlist[0]){ // create starting point for queue
		pq.push(p);
	}
	tuple<int,int, int> p;
	while (mst.size() < n-1) {
		do {
			p = pq.top();
			pq.pop();
		} while (included[get<1>(p)] == 1); // already is included, but one is new
		included[get<1>(p)] = 1;
		mst.push_back(edgelist[get<2>(p)]);
		for (auto newEle : adjlist[get<1>(p)]) { // add newly reachable edges
			if (included[get<1>(newEle)] == 0) {
				pq.push(newEle);
			}
		}
	}

	std::cout << "Checkpoint: MST Prim Seq"; nn;

	return mst;
}
