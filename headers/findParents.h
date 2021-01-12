
// The goal of this function is, to determine the final parent of all vertices in parallel
// we aim for a asymptotic complexity of O(n/p * log(n)) -> can this be improved?

#include <math.h>

void findParents(vector<int> &ParentVertex, vector<edge> best){
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < best.size(); j++){
            edge e1 = best[j];
            if(ParentVertex[e1.dest] > ParentVertex[ParentVertex[e1.source]]){
                ParentVertex[e1.dest] = ParentVertex[ParentVertex[e1.source]];
            }
            if(ParentVertex[e1.dest] > ParentVertex[ParentVertex[e1.dest]]){
                ParentVertex[e1.dest] = ParentVertex[ParentVertex[e1.dest]];
            }
            if(ParentVertex[e1.source] > ParentVertex[ParentVertex[e1.dest]]){
                ParentVertex[e1.source] = ParentVertex[ParentVertex[e1.dest]];
            }
            if(ParentVertex[e1.source] > ParentVertex[ParentVertex[e1.source]]){
                ParentVertex[e1.source] = ParentVertex[ParentVertex[e1.source]];
            }
            
        }
    }
}