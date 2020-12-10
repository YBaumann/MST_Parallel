
void prefixParallel(vector<int> &prefix, vector<vector<edge>> adjArr){
    // We can parallelize this later
    prefix[0] = adjArr[0].size();
    for(int i = 1; i < adjArr.size(); i++){
        prefix[i] = adjArr[i].size() + prefix[i-1];
    }
}