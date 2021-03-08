#pragma once
// returns idx if present, else returns rightmost idx of next lower one
 int Searchlow(vector<int> v, int value){
     int low = 0;
     int hi = v.size() -1;
     for(int i = 0; i < v.size(); i++){
         if(value < v[i]){
             return i-1;
         }
     }

     return hi;

 }

int binSearchlow(vector<int> v, int value){
    int low = 0;
    int hi = v.size()-1;
    int mid;

    if(v[hi] == value) return hi;

    while(hi - low > 1){
        mid = (low + hi) / 2;
        if(value < v[mid]){
            hi = mid;
        } else {
            low = mid;
        }
    }
    return low;
}