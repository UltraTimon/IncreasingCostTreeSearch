#include "icts.h"

void ICTS(vector<Agent> agentList, list<Node> queue) {
    Node currentNode = queue.front();
    queue.pop_front();
    vector<int> optimalCostList = currentNode.data;

    cout << "Costs: ";
    printTree(&currentNode, 1);

    // BELOW WILL BE ADJUSTED LATER ON, AFTER REWRITING getAtLeastOnePathPerAgentWithoutConflict()
    // ---------------------------------------------------------------------------------------------

    // vector<vector<vector<int>>> atLeastOnePathPerAgentWithoutConflict = getAtLeastOnePathPerAgentWithoutConflict(agentList, optimalCostList);
    // if (atLeastOnePathPerAgentWithoutConflict.size() > 0)
    // {
    //     // announce the happy news
    //     cout << "Ladies and Gentleman, we've got a solution!!" << endl;
    //     for (auto pair : atLeastOnePathPerAgentWithoutConflict)
    //     {
    //         for (auto path : pair)
    //         {
    //             for (auto i : path)
    //             {
    //                 cout << i << " ";
    //             }
    //             cout << endl;
    //         }
    //         cout << endl;
    //     }
    // } else {
    //     // give error message
    //     cout << "No paths found" << endl;

    //     // generate the next level of the tree, add those nodes to the queue and go to the next node
    //     generateNextLevel(&currentNode);
    //     for(auto child : currentNode.children) {
    //         queue.push_back(child);
    //     }

    //     ICTS(agentList, queue);
    // }
}