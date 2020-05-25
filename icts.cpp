#include "icts.h"

void printPaths(vector<vector<int>> paths) {
    cout << "Paths:" << endl;

    for(auto path : paths) {
        for(int node : path) {
            cout << node << " ";
        }
        cout << endl;
    }
}

void ICTS(vector<Agent> agentList, list<Node> queue) {
    Node currentNode = queue.front();
    queue.pop_front();
    vector<int> optimalCostList = currentNode.data;

    cout << "Costs: ";
    printTree(&currentNode, 1);

    int maxCost = 0;
    for(int i : optimalCostList)
        if(i > maxCost)
            maxCost = i;

    CombinedGraph cg = CombinedGraph(maxCost); 
    if(getAtLeastOnePathPerAgentWithoutConflict(agentList, optimalCostList, &cg, maxCost)) 
    {
        // announce the happy news
        cout << "Ladies and Gentleman, we've got a solution!!" << endl;

        vector<vector<int>> pathsA, pathsB;

        getPathsFromCombinedGraph(maxCost, &cg, &pathsA, &pathsB);

        bool getOnlyTheFirstPath = true;
        
        // get Only the first path, remove all others
        int nrOfPaths = pathsA.size();
        for (int i = 0; i < nrOfPaths - 1; i++)
        {
            pathsA.pop_back();
            pathsB.pop_back();
        }
        
        printPaths(pathsA);
        printPaths(pathsB);

        vector<vector<int>> finalPaths;
        finalPaths.push_back(pathsA[0]);
        finalPaths.push_back(pathsB[0]);
    }
    else {
        // give error message
        cout << "No paths found" << endl;

        // generate the next level of the tree, add those nodes to the queue and go to the next node
        generateNextLevel(&currentNode);
        for(auto child : currentNode.children) {
            queue.push_back(child);
        }

        ICTS(agentList, queue);
    }
}