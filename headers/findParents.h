
// The goal of this function is, to determine the final parent of all vertices in parallel
// we aim for a asymptotic complexity of O(n/p * log(n)) -> can this be improved?

#include <math.h>

// Improved Pointerjumping

void findParents1(vector<int> &ParentVertex, vector<edge> best, int &n){
    n = 0;
    int bestSize = best.size();
    // We need to have consensus on n, this has O(n/p) timesteps
    #pragma omp parallel for ordered
    for(int i = 0; i < bestSize; i++){
        if(best[best[i].dest].dest == best[i].source && best[i].source < best[i].dest && best[i].weight > 0){
            best[i].dest = best[i].source;
        }
    }
    int PSize = ParentVertex.size();

    int logBS = std::log(bestSize);
    for(int it = 0; it < logBS+5; it++){
        #pragma omp parallel for
        for(int i = 0; i < PSize; i++){
            ParentVertex[i] = best[ParentVertex[i]].dest;
        }
    }
}