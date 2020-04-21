#include "lowlevelsearch.h"

bool pathsHaveConflict(list<int> pathA, list<int> pathB) {
    list<int>::iterator a, b;
    a = pathA.begin();
    b = pathB.begin();
    
    for (int i = 0; i < pathA.size(); i++)
    {
        // same node on same time
        if(*a == *b)
            return true;

        // same edge on same moment of transition
        list<int>::iterator nextPositionOfA = next(a, 1);
        list<int>::iterator nextPositionOfB = next(b, 1);

        if(*a == *nextPositionOfB && *b == *nextPositionOfA)
            return true;

        ++a;
        ++b;
    }
    return false;
}

