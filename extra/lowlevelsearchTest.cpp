#include "lowlevelsearch.h"

bool sameNode() {
    vector<int> a({1});
    vector<int> b({1});
    return pathsHaveConflict(a, b) == true;
}
bool seperateNodes() {
    vector<int> a({1, 2});
    vector<int> b({3, 4});
    return pathsHaveConflict(a, b) == false;
}
bool sameEdge() {
    vector<int> a({1, 2});
    vector<int> b({2, 1});
    return pathsHaveConflict(a, b) == true;
}
bool longListNoConflict() {
    vector<int> a({4, 3, 2, 5, 7, 3, 5, 4, 2, 8});
    vector<int> b({7, 5, 5, 1, 5, 8, 9, 0, 3, 2});
    return pathsHaveConflict(a, b) == false;
}
bool longListNodeConflict() {
    vector<int> a({4, 3, 2, 3, 5, 0, 2, 8});
    vector<int> b({7, 5, 5, 1, 5, 0, 9, 0, 3, 2});
    return pathsHaveConflict(a, b) == true;
}
bool longListEdgeConflict() {
    vector<int> a({4, 7, 3, 5, 2, 2, 8});
    vector<int> b({7, 5, 5, 2, 5, 8, 9, 0, 3, 2});
    return pathsHaveConflict(a, b) == true;
}


int main() {
    if(sameNode() && seperateNodes() && sameEdge() && longListNoConflict() && longListNodeConflict() && longListEdgeConflict()) {
        cout << "All test passed" << endl;
        return 0;
    }
    else {
        cerr << "Test failed" << endl;
        return 1;
    }
}
