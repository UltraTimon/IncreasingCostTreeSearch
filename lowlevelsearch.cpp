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

list<list<list<int>>> getAtLeastOnePathPerAgentWithoutConflict(list<Agent> agentList, int optimalCost) {
    // add the paths to the agent objects
    for(auto it = agentList.begin(); it != agentList.end(); ++it) {
        list<list<int>> generatedPaths = generatePaths("resources/graph.txt", it->start, it->end, optimalCost);
        it->setPaths(generatedPaths);
    }


    // inner list; path
    // middle list; combination of one path for agent A, one path for agent B
    // outer list: list of combinations
    list<list<list<int>>> stub;

    // create sets of 2 paths, 1 for each agent and getting all permutations
    // all have the same amount of paths
    for(auto outerAgentIt = agentList.begin(); outerAgentIt != agentList.end(); ++outerAgentIt) {
        for(auto innerAgentIt = next(outerAgentIt, 1); innerAgentIt != agentList.end(); ++innerAgentIt) {
            for(auto outerPathIt = outerAgentIt->paths.begin(); outerPathIt != outerAgentIt->paths.end(); ++outerPathIt) {
                for(auto innerPathIt = outerAgentIt->paths.begin(); innerPathIt != outerAgentIt->paths.end(); ++innerPathIt) {
                    list<list<int>> temp;
                    if(!pathsHaveConflict(*outerPathIt, *innerPathIt)) {
                        temp.push_back(*outerPathIt);
                        temp.push_back(*innerPathIt);
                        stub.push_back(temp);
                    }
                }
            }    
        }
    }
    
    // print paths
    cout << "Paths without conflict for optimalCost = " << optimalCost << ": " << endl;
    int agentCounter = 0;
    for(auto outer : stub) {
        for(auto middle : outer) {
            for(auto i : middle) {
                cout << i << " ";
            }
            cout << endl;
        }
    }
    
    return stub;
}

