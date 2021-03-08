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


using namespace std;


struct edge {
	int source, dest, weight, idx;

};

bool operator<(const edge& e1, const edge& e2) {
	if (e1.weight != e2.weight) {
		return e1.weight < e2.weight;
	}
	else if (e1.source != e2.source) {
		return e1.source < e2.source;
	}
	else {
		return e1.dest < e2.dest;
	}

}


class UnionFind {
	vector<int> id;
	vector<int> sz; // size
	int cnt; // count

public:
	// Create an empty union find data structure with N isolated sets.
	UnionFind(int N) {
		cnt = N; 
		for (int i = 0; i < N; i++) {
			id.push_back(i);
			sz.push_back(1);
		}
	}

	// Somehow this fails for multiedges
	// Return the id of component corresponding to object p.
	int find(int p) {
		int root = p;
		
		for(int i = 0; i < cnt; i++){
			std::cout << id[i] << ' ';
		}

		while (root != id[root]) { // find highest parent
			std::cout << " comes ";
			root = id[root];
			std::cout << root << " <- root \n";
		}
		
		while (p != root) { // p = parent[p] & id[p] = root
			int newp = id[p]; 
			id[p] = root; 
			p = newp; 
		}
		
		return root;
	}

	// Replace sets containing x and y with their union.
	void merge(int x, int y) {
		int i = find(x); int j = find(y); if (i == j) return;
		// make smaller root point to larger one
		if (sz[i] < sz[j]) { id[i] = j, sz[j] += sz[i]; }
		else { id[j] = i, sz[i] += sz[j]; }
		cnt--;
	}
	// Are objects x and y in the same set?
	bool equalSuper(int x, int y) {
		return find(x) == find(y);
	}
	// Return the number of disjoint sets.
	int count() { 
		return cnt;
	}




};