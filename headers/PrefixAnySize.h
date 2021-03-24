using namespace std;

#define nn std::cout << "\n"

void ParPrefixPow2Adjusted(vector<int> &arr, int threadnr, int logN){
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
    
    //Downsweep
    arr[n-1] = 0;
    int pwd = pow(2,logN);
    for(int d = logN ; d >= 0; d--){
        int inc = pwd * 2;
        #pragma omp parallel for
        for(int k = 0; k < n; k += inc){
            int idx1 = k + pwd - 1;
            int idx2 = idx1 + pwd;
            double val = arr[idx2]; 
            double t = arr[idx1];
            arr[idx1] = val;
            arr[idx2] = t + val;
            
        }
        pwd /= 2;
    }
}




// solution with padding in place
void ParPrefixAnySize(vector<int> &arr, vector<int> given, int &Threadnr){
    arr = given;
    int n = arr.size();
    int logN = 1;
    int twos = 2;
    while(twos < n){
        twos *= 2;
        logN++;
    }
    arr.resize(twos);
    ParPrefixPow2Adjusted(arr, Threadnr, logN);
    arr.resize(n);


}