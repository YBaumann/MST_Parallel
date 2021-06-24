vector<int> findParentsP(vector<pair<edge,int>> &best, int n, int thread,vector<edge> &ret){


    // Sort proposals
    auto compara1 = [](pair<edge,int> e1, pair<edge,int> e2){
		return e1.first.dest != e2.first.dest ? e1.first.dest < e2.first.dest : e1.second < e2.second;};
	std::sort(best.begin(), best.end(), compara1);

    vector<int> pars(n);
    for(int i = 0; i < n; i++){
        pars[i] = i;
    }
    vector<edge> usefuledges(n);
    for(int i = 0; i < n; i++){
        usefuledges[i] = edge(i,i,-1,-1);
    }
    
    // fill best edges
    usefuledges[best[0].first.dest] = best[0].first;
    for(int i = 1; i < best.size(); i++){
        if(best[i].first.dest != best[i-1].first.dest){
             usefuledges[best[i].first.dest] = best[i].first;
        }
    }  

    
    int bestSize = usefuledges.size();
    // We need to have consensus on n, this has O(n/p) timesteps

    // We need to have consensus on n, this has O(n/p) timesteps
    #pragma omp parallel for ordered
    for(int i = 0; i < bestSize; i++){
        if(usefuledges[usefuledges[i].source].source == usefuledges[i].dest && usefuledges[i].dest < usefuledges[i].source && usefuledges[i].weight > 0){
            usefuledges[i].source = usefuledges[i].dest;
        }
    }
    for(int i = 0; i < n; i++){
        //std::cout << usefuledges[i].source << ' ' << usefuledges[i].dest;nn;
    }
    nn;

    int PSize = pars.size();
    int logBS = std::log(bestSize);

    for(int it = 0; it < logBS+5; it++){
        #pragma omp parallel for
        for(int i = 0; i < PSize; i++){
            if(usefuledges[pars[i]].idx > -1){
            pars[i] = usefuledges[pars[i]].source;
            }
        }
    }
    ret = usefuledges;
    return pars;
}
