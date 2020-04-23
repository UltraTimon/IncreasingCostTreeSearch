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
    // for(auto it = agentList.begin(); it != agentList.end(); ++it) {
    auto it = agentList.begin();
    auto costIt = optimalCostList.begin();
    while(it != agentList.end() && costIt != optimalCostList.end()) {
        vector<vector<int>> generatedPaths = generatePaths("resources/graph.txt", it->start, it->end, *costIt);
        it->setPaths(generatedPaths);
        ++it;
        ++costIt;
    }


    // inner vector; path
    // middle vector; combination of one path for agent A, one path for agent B, one path for agent C, ... etc.
    // outer vector: vector of combinations
    vector<vector<vector<int>>> stub;

    // create sets of 2 paths, 1 for each agent and getting all permutations
    // all have the same amount of paths
    for(auto outerAgentIt = agentList.begin(); outerAgentIt != agentList.end(); ++outerAgentIt) {
        for(auto innerAgentIt = next(outerAgentIt, 1); innerAgentIt != agentList.end(); ++innerAgentIt) {
            for(auto outerPathIt = outerAgentIt->paths.begin(); outerPathIt != outerAgentIt->paths.end(); ++outerPathIt) {
                for(auto innerPathIt = innerAgentIt->paths.begin(); innerPathIt != innerAgentIt->paths.end(); ++innerPathIt) {
                    if(!pathsHaveConflict(*outerPathIt, *innerPathIt)) {
                        vector<vector<int>> temp;
                        temp.push_back(*outerPathIt);
                        temp.push_back(*innerPathIt);
                        stub.push_back(temp);
                    }
                }
            }    
        }
    }
    
    return stub;
}

