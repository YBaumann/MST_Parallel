#pragma once

vector<vector<edge>> edgeListToAdjArray(vector<edge> edgelist, int n)
{
    // initialize adjacency Vector
    vector<vector<edge>> result;
    for(int i = 0; i < n; i++){
        vector<edge> f;
        result.push_back(f);
    }

    // convert edgelist
    for(edge e : edgelist){
        int source = e.source;
        int dest = e.dest;
        result[source].push_back(e);
        result[dest].push_back(e);
    }

    return result;
}