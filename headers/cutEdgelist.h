// This function takes an edgelist and deletes all self edges in Parallel


void cutEdgelist(vector<edge> &edgelist, int &m){
    vector<int> useful(m);
    vector<int> newIndices;

#pragma omp parallel for
    for(int i = 0; i <  m; i++){
        if(edgelist[i].source != edgelist[i].dest){
            useful[i] = 1;
        }
    }



}