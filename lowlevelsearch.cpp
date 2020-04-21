#include "lowlevelsearch.h"

/* 2 paths have a conflict if either:
    - they visit the same node at the same time, i.e. at most one agent may
        be at a node per time unit
    - one path uses an edge (a, b) at the same time another path 
        uses the edge (b, a), i.e. at most one agent may use an edge per time unit 
*/
bool pathsHaveConflict(list<int> pathA, list<int> pathB) {
    list<int>::iterator a, b;
    a = pathA.begin();
    b = pathB.begin();
    
    for (int i = 0; i < pathA.size(); i++)
    {
        // same node on same time
        if(*a == *b) {
            cout << "conflict at node " << *a << endl;
            return true;
        }

        // same edge on same moment of transition
        list<int>::iterator nextPositionOfA = next(a, 1);
        list<int>::iterator nextPositionOfB = next(b, 1);

        if(*a == *nextPositionOfB && *b == *nextPositionOfA) {
            cout << "conflict at edge " << *a << ", " << *b << endl;
            return true;
        }

        ++a;
        ++b;
    }
    return false;
}

