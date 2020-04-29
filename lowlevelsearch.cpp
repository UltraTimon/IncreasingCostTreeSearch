#include "lowlevelsearch.h"

/* 2 paths have a conflict if either:
    - they visit the same node at the same time, i.e. at most one agent may
        be at a node per time unit
    - one path uses an edge (a, b) at the same time another path 
        uses the edge (b, a), i.e. at most one agent may use an edge per time unit 
*/
bool pathsHaveConflict(vector<int> pathA, vector<int> pathB) {
    vector<int>::iterator a, b;
    a = pathA.begin();
    b = pathB.begin();
    
    for (int i = 0; i < pathA.size(); i++)
    {
        // same node on same time
        if(*a == *b) {
            return true;
        }

        // same edge on same moment of transition
        vector<int>::iterator nextPositionOfA = next(a, 1);
        vector<int>::iterator nextPositionOfB = next(b, 1);

        if(*a == *nextPositionOfB && *b == *nextPositionOfA) {
            return true;
        }

        ++a;
        ++b;
    }
    return false;
}

void getAtLeastOnePathPerAgentWithoutConflict(vector<Agent> agentList, vector<int> optimalCostList) {
    // generate a useful graph for each agent

    // conmbine those using the MDD magic from the paper

    // check for conflicts by running modified nodeIsUseful method that marks nodes as useful based on whether or not they do or do not conflict
    // we need a special graph structure for this, CombinedGraph or something like that

    // if the combined source node has been marked as useful we know that there exists at least one path and we can return true
    // then we can add the paths to a given pointer that holds the path data (which is stored in ICTS() or even main()), and add it just like in getPathsFromGraph()

    // And then we're done... Sounds good, right?
}

