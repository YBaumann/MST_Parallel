
void prefixParallel(vector<int> &prefix, vector<vector<edge>> adjArr){
    // We can parallelize this later
    prefix[0] = 0;
    for(int i = 1; i < adjArr.size(); i++){
        prefix[i] = adjArr[i-1].size() + prefix[i-1];
    }
}