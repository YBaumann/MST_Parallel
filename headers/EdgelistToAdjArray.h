#define startTimer auto t1 = std::chrono::high_resolution_clock::now()
#define endTimer auto t2 = std::chrono::high_resolution_clock::now()
#define printTime auto durationSeq = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count(); std::cout << "Time: " << durationSeq / 1000000.0<< " sec\n"
#define nn std::cout << "\n"

vector<vector<edge>> edgeListToAdjArray(vector<edge> &edgelist, int n, int totalN)
{
    // initialize adjacency Vector
    vector<vector<edge>> result(totalN);
    

    // convert edgelist to adjarr
    // we rewrite edges, such that source dest matches
    for(int i = 0; i < edgelist.size(); i++){
        edge e = edgelist[i];
        edge f = edge(e.source,e.dest,e.weight,e.idx);
        result[e.source].push_back(e); 
        result[e.dest].push_back(f);
    }

    return result;
}