

int ParentStep(vector<int> ParentVertex, vector<edge> best){
    int notFinished = 0;
    int n = ParentVertex.size();
    #pragma omp parallel for ordered
    for(int i = 0; i < n; i++){
        int currParent = ParentVertex[i];
        int PoP = ParentVertex[best[i].dest]; // Parent of Parent
        if(PoP){

        }

    }


    return 0;

}


// The goal of this function is, to determine the final parent of all vertices in parallel
// we aim for a asymptotic complexity of O(n/p * log(n))

void findParents(vector<int> ParentVertex, vector<edge> best){
    int notFinished = 1;
    while(notFinished){
        notFinished = ParentStep(ParentVertex, best);
    }
}