#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <list>
#include <vector>
#include <queue>
#include <bitset>
#include <cctype>
#include <algorithm>


typedef long long ll;
#define nn cout << '\n';
#define rep(n) for(int i = 0; i < n; i++)
#define repi(n) for(int i = 0; i <n; i++)

using namespace std;

int main() {

	ofstream f;
	f.open("Resources/WattsStrogatz1M.txt");

	// It must hold N >> K >> ln N >> 1

	int N = 1000000; // nodes in Graph
	int K = 120; // average degree
	double beta = 0.3; // 0 <= beta <= 1!


	vector<set<int>> adjlist(N);

	for (int i = 0; i < N; i++) {
		set<int> li;
		adjlist.push_back(li);
	}
	std::cout << "here";
	// 1. step Create Lattice for given K and N
	// two for loops can be *heavily* optimized to run in O(N * K)

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (0 < abs(i - j) % (N - 1 - K / 2) && abs(i - j) % (N - 1 - K / 2) <= K / 2 && i < j) {
				adjlist[i].insert(j);
				adjlist[j].insert(i);
			}
		}
		if(i % 100 == 0)
		std::cout << i;
	}
	std::cout << "further";
	int edges = 0;
	for (auto e : adjlist) {
		for (auto es : e) {
			edges++;
		}
	}
	std::cout << "1step";



	// 2. step
	// look at all edges connecting a node to a "right" neighbour and rewire it with probablity beta
	// we have a set that checks if an edge already exists

	for (int i = 0; i < N; i++) {
		for (int j = 1; j <= K / 2 + 1; j++) {
			if (!adjlist[i].count((i + j) % N) || !adjlist[(i + j) % N].count(i)) {
				continue;
			}
			double r = double(rand()) / (double(RAND_MAX) + 1.0);
			if (r <= beta) { // do rewire
				int newEdge = rand() % N;
				while (newEdge == i || adjlist[i].count(newEdge)) { // illegal new edge
					newEdge = rand() % N;
				}

				adjlist[i].erase((i + j) % N);
				adjlist[(i + j) % N].erase(i);
				adjlist[i].insert(newEdge);
				adjlist[newEdge].insert(i);
			}
		}
	}

	edges = 0;

	for (auto e : adjlist) {
		for (auto es : e) {
			edges++;
		}
	}


	f << N << " " << N * K / 2 << "\n";
	for (int i = 0; i < N; i++) {
		set<int> li = adjlist[i];
		for (auto p : li) {
			if (i < p) {
				f << i << " " << p << " " << rand() % 100 + 1 << "\n"; // Assign random weight in [1,100]
			}
		}
	}





	return 0;
}

/*

*/