#include "icts.h"

class MDD {
    public:
        int nodeNumber;
        vector<int> edges;
        MDD() {
            nodeNumber = -1;
        }
        MDD(int n) {
            this->nodeNumber = n;
        }
};

MDD generateMDD(vector<vector<int>> pathList, int numberOfNodes);