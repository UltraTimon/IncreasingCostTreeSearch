#include "mdd.h"

MDD generateMDD(vector<vector<int>> pathList, int numberOfNodes) {
    MDD mddList[numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++)
    {
        mddList[i] = MDD(i);
    }
    for(auto vector : pathList) {
        for (int i = 0; i < vector.size() - 1; i++)
        {
            // mddList[vector[i]]
        }
        
    }
}