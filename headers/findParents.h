
// The goal of this function is, to determine the final parent of all vertices in parallel
// we aim for a asymptotic complexity of O(n/p * log(n)) -> can this be improved?

#include <math.h>

// Improved Pointerjumping

void findParents1(vector<int> &ParentVertex, vector<edge> best, int &n){
    n = 0;

    // We need to have consensus on n!, this has O(n) timesteps
    #pragma omp parallel for ordered
    for(int i = 0; i < best.size(); i++){
        if(best[best[i].dest].dest == i && i < best[i].dest){
            best[i].dest = i;
            #pragma omp ordered
            {
                n++;
            }
        }
    }

    for(int it = 0; it < std::log(best.size()) + 10; it++){
        #pragma omp parallel for
        for(int i = 0; i < ParentVertex.size(); i++){
            ParentVertex[i] = ParentVertex[best[i].dest];
        }
    }
    std::cout << "Leaves\n";
}