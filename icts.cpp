#include "icts.h"

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