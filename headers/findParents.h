
// The goal of this function is, to determine the final parent of all vertices in parallel
// we aim for a asymptotic complexity of O(n/p * log(n)) -> can this be improved?

#include <math.h>

// Improved Pointerjumping

void findParents1(vector<int> &ParentVertex, vector<edge> best, int &n){
    n = 0;

    // We need to have consensus on n, this has O(n) timesteps
    #pragma omp parallel for ordered
    for(int i = 0; i < best.size(); i++){
        if(best[best[i].dest].dest == best[i].source && best[i].source < best[i].dest && best[i].weight > 0){
            best[i].dest = best[i].source;
            #pragma omp ordered
            {
                n++;
            }
        }
    }

    for(int it = 0; it < std::log(best.size())+100; it++){
        if(it % 1000 == 0){
            std::cout << it << endl;
        }
        #pragma omp parallel for
        for(int i = 0; i < ParentVertex.size(); i++){
            ParentVertex[i] = best[ParentVertex[i]].dest;
        }
    }
    std::cout << "Leaves\n";
}