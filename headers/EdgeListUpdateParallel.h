#pragma once

void UpdateEdgelist(vector<edge> &edgelist, const vector<int> &ParentVertex){

	#pragma omp parallel for
    for(int i = 0; i < edgelist.size(); i++){
		edgelist[i].source = ParentVertex[edgelist[i].source];
		edgelist[i].dest = ParentVertex[edgelist[i].dest];
	}

}