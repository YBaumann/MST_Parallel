// This functions sums the values in an integer array in parallel
// Further extensions could include arbitrary function


int aggregatePow2(vector<int> &arr,int logN, int Threadnr){
    int n = arr.size();
    logN--;
    // Upsweep
    int inc = 1;
    for(int d = 0; d <= logN; d++){
        int pwd = inc;
        inc *= 2;
        #pragma omp parallel for
        for(int k = 0; k < n; k += inc){
            int idx = k + pwd * 2 - 1;
            arr[idx] = arr[k + pwd - 1] + arr[idx];
        }
    }
    return arr[n-1];
}

void aggregate(int& res, vector<int> arr, int Threadnr){
    int n = arr.size();
    int logN = 1;
    int twos = 2;
    while(twos < n){
        twos *= 2;
        logN++;
    }
    arr.resize(twos);
    res = aggregatePow2(arr, logN,Threadnr);
    arr.resize(n);
}

