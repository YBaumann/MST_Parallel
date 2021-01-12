#pragma once
#include <queue>

// we do a simple BFS to check connectivity!

bool is_Connected(vector<edge> edgeList, int n, int m) {
	vector<vector<int>> adjlist(n);
	for (int i = 0; i < n;i++) {
		vector<int> f;
		adjlist.push_back(f);
	}
	for (auto e : edgeList) {
		adjlist[e.source].push_back(e.dest);
		adjlist[e.dest].push_back(e.source);
	}
	vector<int> reachable(n);
	reachable[0] = 1;
	queue<int> q;
	q.push(0);

	while (q.size() > 0) {
		int nex = q.front();
		q.pop();
		for (auto e : adjlist[nex]) {
			if (!reachable[e]) {
				reachable[e] = 1;
				q.push(e);
			}
		}
	}
	
	for (int i = 0; i < n;i++) {
		if (!reachable[i]) {
			return false;
		}
	}

	return true;
}