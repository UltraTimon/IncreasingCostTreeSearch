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

void ICTS(vector<Agent> agentList, list<Node> queue, int nrOfNodex) {
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

        printPaths(pathsA);
        printPaths(pathsB);
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