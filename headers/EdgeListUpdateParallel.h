#pragma once

void UpdateEdgelist(vector<edge> &edgelist, vector<int> ParentVertex, int &n){

    set<int> s;

    for(int i = 0; i < edgelist.size(); i++){
		edgelist[i].source = ParentVertex[edgelist[i].source];
		edgelist[i].dest = ParentVertex[edgelist[i].dest];
        s.insert(edgelist[i].dest);
        s.insert(edgelist[i].source);
	}

    n = s.size();

}