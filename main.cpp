/*test.cpp*/

//
// An AVL unit test based on Catch framework
//

#include <iostream>
#include <vector>
#include <algorithm>

#include "avlt.h"


using namespace std;


int main(){
    
    avlt<int, int>  tree;

    vector<int> keys = { 100,50,150,32,75,125,175,2,40,57,88,120,130,170,180};
    vector<int> heights = { 3,2,2,1,1,1,1,0,0,0,0,0,0,0,0};

//    vector<int> keys = { 100,75,125,43,78,130,22,50 };
//    vector<int> heights = { 3,2,1,1,0,0,0,0 };
    for (int key : keys)
    {
      tree.insert(key, -key);
    }
 
    vector<int> like = tree.range_search(3,181);
    
    
    return 0;
}
