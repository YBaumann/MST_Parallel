#pragma once

vector<vector<edge>> edgeListToAdjArray(vector<edge> &edgelist, int n, int totalN)
{
    // initialize adjacency Vector
    vector<vector<edge>> result;
    for(int i = 0; i < totalN; i++){
        vector<edge> f;
        result.push_back(f);
    }

    // convert edgelist to adjarr
    // we rewrite edges, such that source dest matches
    for(edge e : edgelist){
        int source = e.source;
        int dest = e.dest;
        result[source].push_back(e);
        edge f; 
        f.source = dest; 
        f.dest = source; 
        f.weight = e.weight; 
        f.idx = e.idx;
        result[dest].push_back(f);
    }

    return result;
}