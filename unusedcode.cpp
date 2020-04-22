int main () {
    // loop over agents one by one, pair up paths for conflict checking
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
        // If no conflict, add pair to no-conflict list
        if(!pathsHaveConflict(currentPair.first, currentPair.second)) {
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

}

