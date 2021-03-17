
// arr updates such that it now only holds the entries with marks

void rewriteVectorPar(vector<int> &arr, vector<int> &marks, int threadN){
    int n = marks.size();
    int val = marks[n-1];

    ParPrefixAnySize(marks,marks, threadN);
    marks.push_back(marks[n-1] + val);

    int newArrSize = marks[n];
    vector<int> sol(newArrSize);

    #pragma omp parallel for num_threads(threadN)
    for(int i = 0; i < marks.size()-1; i++){
        if(marks[i] < marks[i+1]){
            sol[marks[i]] = arr[i];
        }
    }
    arr = sol;

}


void rewriteVectorSeq(vector<int> &arr, vector<int> &marks, int threadN){
    vector<int> sol;

    for(int i = 0; i < marks.size(); i++){
        if(marks[i] == 1){
            sol.push_back(arr[i]);
        }
    }
    arr = sol;
}
