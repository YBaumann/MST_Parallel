#pragma once

bool CorrectnessTestMST();
bool CorrectnessTestPrefix();
bool CorrectnessTestMultiPrefix();
bool CorrectnessTestBinarySearch();

void runAllTests(){

    assert(CorrectnessTestMST() && "MST Test Failed");
    assert(CorrectnessTestPrefix() && "Prefix Test Failed");
    assert(CorrectnessTestMultiPrefix() && "Multiprefix Test Failed");
    assert(CorrectnessTestBinarySearch() && "BinarySearch TestFailed");
    std::cout << "All Tests were successfully Passed!";
}


bool CorrectnessTestMST(){
    // We will generate random connected graphs, then we assert that Sequential and Parallel MST gives same result


    return true;
}


bool CorrectnessTestPrefix(){
    // Test for a few arrays the correctness of our parallel prefix

    return true;
}

bool CorrectnessTestMultiPrefix(){
    // Test for a few arrays the correctness of our multiprefix

    return true;
}


bool CorrectnessTestBinarySearch(){
    // Test if Binsearch returns correct index (next lower value)
    bool check = true;
    for(int i = 2; i < 1000000; i *= 2){
        vector<int> arr(i);
        double val = 1;
        for(int j = 0; j < i; j++){
            val += rand() % 30;
            arr[j] = val;
        }
        int idx = rand() % (i-1);
        int v = arr[idx] + 1;
        check = Searchlow(arr, v) == binSearchlow(arr, v);
        if(!check){
            int sidx = Searchlow(arr, v);
            int bsidx = binSearchlow(arr, v);
            std::cout << "BinSearch Will fail!\n";
            std::cout << sidx << " != " << bsidx << "\n";
            std::cout << "Array size: " << i << " Value: " << v;nn;
            std::cout << "Normalsearch: " << arr[sidx] << " Bsearch: " << arr[bsidx] << "\n";
            break;
        }
        std::cout << "binSearch Test for i = " << i << " passed!\n";
    }


    return check;
}



