#pragma once
// returns idx if present, else returns idx of next lower one
// doesnt seem right somehow!!!!
int binSearchlow(vector<int> v, int value){
    int low = 0;
    int hi = v.size() -1;
    for(int i = 0; i < v.size(); i++){
        if(value < v[i]){
            return i-1;
        }
    }

    return hi;

}