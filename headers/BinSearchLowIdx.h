
// returns idx if present, else returns idx of next lower one
// doesnt seem right somehow!!!!
int binSearchlow(vector<int> v, int value){
    int low = 0;
    int hi = v.size() -1;
    int middle = (low + hi) / 2;
    int i = 0;
    while(v[middle] != value && middle > 0 && middle < v.size()-1){
        if(v[middle-1] < value && v[middle] > value){
            return middle-1;
        } else if(middle == v.size()-1 || (v[middle] < value && v[middle+1] > value)){
            return middle;
        }
        if(v[middle] > value){
            hi = middle;
            middle = (low + hi) / 2;
        } else {
            low = middle;
            middle = (low + hi) / 2;
        }
    }

    return middle;

}