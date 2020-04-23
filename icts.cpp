#include "icts.h"

void ICTS(list<Agent> agentList, list<Node> queue) {
    Node currentNode = queue.front();
    queue.pop_front();
    list<int> optimalCostList = currentNode.data;

    list<list<list<int>>> atLeastOnePathPerAgentWithoutConflict = getAtLeastOnePathPerAgentWithoutConflict(agentList, optimalCostList);
    if (atLeastOnePathPerAgentWithoutConflict.size() > 0)
    {
        // announce the happy news
        cout << "Ladies and Gentleman, we've got a solution!!" << endl;

        // print paths
        cout << "# Path pairs without conflict: " << atLeastOnePathPerAgentWithoutConflict.size() << endl;
        for (auto pair : atLeastOnePathPerAgentWithoutConflict)
        {
            for (auto path : pair)
            {
                for (auto i : path)
                {
                    cout << i << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
    } else {
        // generate the next level of the tree, add those nodes to the queue and go to the next node
        generateNextLevel(&currentNode);
        printTree(&currentNode);
        for(auto child : currentNode.children) {
            queue.push_back(child);
        }
        ICTS(agentList, queue);
    }
}