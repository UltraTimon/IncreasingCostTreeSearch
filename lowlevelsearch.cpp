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

list<list<int>> getAtLeastOnePathPerAgentWithoutConflict() {
    // get list of agent objects read from file
    list<Agent> agentList = importAgents("resources/agents.txt");
    int optimalCost = 3;

    // actually add the paths to the agent objects
    for(auto it = agentList.begin(); it != agentList.end(); ++it) {
        list<list<int>> generatedPaths = generatePaths("resources/graph.txt", it->start, it->end, optimalCost);
        it->setPaths(generatedPaths);
    }

    // print paths
    int agentCounter = 0;
    for(auto agent : agentList) {
        cout << "Agent " << agentCounter << ":" << endl;
        agentCounter++;

        int printPathCounter = 0;
        for (auto list : agent.paths)
        {
            cout << "path " << printPathCounter << ": ";
            printPathCounter++;
            for (auto i : list)
            {
                cout << i << " ";
            }
            cout << endl;
        }
    }

    list<list<int>> stub;

    // // create sets of k paths, 1 for each agent and getting all permutations
    // for(auto agentIterator = agentList.begin(); agentIterator != agentList.end(); ++agentIterator) {
    //     for(auto outerPathIterator = agentIterator->paths.begin(); agentIterator != agentList.end(); ++agentIterator) {
            
    //     }
    // }
    
    return stub;
}

