#include "generatePaths.h"
#include "agent.h"
#include "lowlevelsearch.h"

using namespace std;

int main() {
    int exactCost = 3;

    // get list of agent objects read form file
    list<Agent> agentList = importAgents("resources/agents.txt");

    // generate paths for each agent, add paths to agent object
    list<Agent>::iterator it;
    for(it = agentList.begin(); it != agentList.end(); ++it) {
        list<list<int>> generatedPaths = generatePaths("resources/graph.txt", it->start, it->end, exactCost);
        it->setPaths(generatedPaths);
    }

    // // check for each pair of paths whether they have a conflict, as defined in "lowlevelsearch.cpp"
    // // generate pairs of paths for 2 agents, will later be generalized to multiple agents
    // list<pair<list<int>, list<int>>> pathPairs;
    // for(auto pathA : agentList.front().paths) {
    //     for(auto pathB : agentList.back().paths) {
    //         pair<list<int>, list<int>> newPathPair = pair<list<int>, list<int>>(pathA, pathB);
    //         pathPairs.push_back(newPathPair);
    //     }
    // }

    // now for > 2 agents
    list<pair<list<int>, list<int>>> multiAgentPathPairs;
    list<Agent>::iterator masterIterator;
    list<Agent>::iterator subIterator;

    // loop over agents one by one
    for(masterIterator = agentList.begin(); masterIterator != agentList.end(); ++masterIterator) {
        // sub is 1 agent ahead of master
        subIterator = masterIterator;
        ++subIterator;
        // loop over agents one by one, starting from the masterIterator
        while(subIterator != agentList.end()) {
            // loop over paths from master agent, one by one
            for(auto pathA : masterIterator->paths) {
                // loop over paths from sub agent, one by one
                for(auto pathB : subIterator->paths) {
                    pair<list<int>, list<int>> newPathPair = pair<list<int>, list<int>>(pathA, pathB);
                    multiAgentPathPairs.push_back(newPathPair);
                }
            }
            ++subIterator;
        }
    }

    // check for conflict
    // NOTE: in the final ICTS algorithm this will only be done on pairs that are examined, not all of them beforehand
    cout << "nr of pairs before coflict check: " << multiAgentPathPairs.size() <<  endl;
    bool conflicts = false;
    int pairIndex = 0;
    list<pair<list<int>, list<int>>> multiAgentPathPairsWithoutConflict;
    for(auto currentPair : multiAgentPathPairs) {
        if(pathsHaveConflict(currentPair.first, currentPair.second)) {
            cout << "conflict!" << endl;
        } else {
            multiAgentPathPairsWithoutConflict.push_back(currentPair);
        }
    }
    cout << "nr of multi agent pairs after coflict check: " << multiAgentPathPairsWithoutConflict.size() <<  endl;
    
    



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

    return 0;
}