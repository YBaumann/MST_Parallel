using namespace std;

#define nn std::cout << "\n"


// We get three arrays
//  -arr1 is filled with the amount of outgoind edges from vertex i
//  -arr2 contains the parentVertex of vertex i
//  -arr3 returns starting idx


// 1. does some sort of parallel prefix work, where we only compare values if they have the same parent?
// 2. aggregate all indices of a supervertex and then calc prefix of these vectors

void findStartingIdx(vector<int> sizes, vector<int> parents, vector<int> &startIdx){
    int size = sizes.size();
    vector<int> NewSize(size);
}
















