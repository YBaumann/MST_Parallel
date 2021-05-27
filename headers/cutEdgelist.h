// This function takes an edgelist and deletes all self edges in Parallel


void cutEdgelist(vector<edge> &edgelist, vector<int> &outgoingsizes, int &m, int n, int Threadnr){
    vector<int> useful(m);
    vector<int> newIndices;

    // Find useful edges
    
#pragma omp parallel for
    for(int i = 0; i < m; i++){
        if(edgelist[i].source != edgelist[i].dest){
            useful[i] = 1;
        }
    }
    

    // Calc new indices for these edges
    ParPrefixAnySize(newIndices, useful, Threadnr);

    int newm = newIndices[newIndices.size() -1] + 1;
    vector<edge> newedgelist(newm);

#pragma omp parallel for
    for(int i = 0; i < m; i++){
        if(useful[i] == 1){
            newedgelist[newIndices[i]] = edgelist[i];
        }
    }

    m = newm;
    edgelist = newedgelist;
    
    vector<int> sizes(n);


    // Figure out Parallel version for sizes
    // TODO Parallelize
    for(int i = 0; i < m; i++){
        sizes[edgelist[i].source]++;
    }
    

    outgoingsizes = sizes;



}