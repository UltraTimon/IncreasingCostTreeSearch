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

vector<vector<vector<int>>> getAtLeastOnePathPerAgentWithoutConflict(vector<Agent> agentList, vector<int> optimalCostList) {
    // add the paths to the agent objects
    for(auto it = agentList.begin(); it != agentList.end(); ++it) {
        // auto it = agentList.begin();
        auto costIt = optimalCostList.begin();
        while(it != agentList.end() && costIt != optimalCostList.end()) {
            // cout << "optimalCost index: " << *costIt << endl;
            vector<vector<int>> generatedPaths;
            generatePaths("resources/graph.txt", it->start, it->end, *costIt, &generatedPaths);
            it->setPaths(generatedPaths);
            ++it;
            ++costIt;
        }
    }

}

