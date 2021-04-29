#include <math.h>

void prefixSeq(vector<int> &prefix, vector<vector<edge>> adjArr){
    // We can parallelize this later
    prefix[0] = 0;
    int Asize = adjArr.size();
    for(int i = 1; i < Asize; i++){
        prefix[i] = adjArr[i-1].size() + prefix[i-1];
    }
}

void prefix(vector<int> &prefix, vector<int> adjArr, int useless){
    // We can parallelize this later
    prefix[0] = 0;
    int Asize = adjArr.size();
    for(int i = 1; i < Asize; i++){
        prefix[i] = adjArr[i-1] + prefix[i-1];
    }
}


void prefixPar(vector<int> &prefix, vector<vector<edge>> &adjArr){

    #pragma omp parallel for
    for(int i = 0; i < prefix.size(); i++){
        prefix[i] = adjArr[i].size();
    }
    
    // Now do Prefix: Up-sweep
    int n = prefix.size();
    for(int d = 0; d < log(n); d++){
        for(int k = 0; k < n; k += pow(2,d+1)){
            prefix[k + pow(2,d+1)-1] = prefix[k + pow(2,d+1) -1] + prefix[k + pow(2,d)-1];
        }
    }

    // DownSweep
    prefix[n-1] = 0;
    for(int d = log(n)-1; d >= 0; d--){
        for(int k = 0; k < n; k += pow(2,d+1)){
            int t = prefix[k + pow(2,d) - 1];
            prefix[k + pow(2,d) - 1] = prefix[k + pow(2,d+1) - 1];
            prefix[k + pow(2,d+1) - 1] = t + prefix[k + pow(2,d+1) - 1];
        }
    }


}