using namespace std;

#define nn std::cout << "\n"

vector<int> ParPrefixPow2(vector<int> arr, int threadnr){
    int n = arr.size();
    int logN = 1;
    int twos = 2;
    while(twos < n){
        twos *= 2;
        logN ++;
    }
    logN--;
    std::cout << logN;
    
    // Upsweep
    int inc = 1;
    for(int d = 0; d <= logN; d++){
        inc *= 2;
        int pwd = pow(2,d);
        #pragma omp parallel for num_threads(threadnr)
        for(int k = 0; k < n; k += inc){
            int idx = k + pwd * 2 - 1;
            arr[idx] = arr[k + pwd - 1] + arr[idx];
        }
    }
    
    //Downsweep
    arr[n-1] = 0;
    for(int d = logN; d >= 0; d--){
        int pwd = pow(2,d);
        int inc = pwd * 2;
        #pragma omp parallel for num_threads(threadnr)
        for(int k = 0; k < n; k += inc){
            int idx1 = k + pwd - 1;
            int idx2 = idx1 + pwd;
            double t = arr[idx1];
            arr[idx1] = arr[idx2];
            arr[idx2] = t + arr[idx2];
        }
    }

    return arr;

}


vector<int> SeqPrefixPow2(vector<int> arr, int useless){
    vector<int> sol(arr.size());
    int n = arr.size();
    for(int i = 0; i < n; i++){
        sol[i+1] = sol[i] + arr[i];
    }
    return sol;
}

vector<int> SeqPrefixAnySize(vector<int> arr, int useless){
    vector<int> sol(arr.size());
    int n = arr.size();
    for(int i = 0; i < n; i++){
        sol[i+1] = sol[i] + arr[i];
    }
    return sol;
}



