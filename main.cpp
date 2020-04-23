#include "lowlevelsearch.h"
#include "generateTree.h"

using namespace std;

int main(int argc, char **argv)
{
    /*
        Main questions for this algorithm:

        1) Does there exist a path of cost x?
        2) If yes, and if we are going to examine this cost node, does there exist a path for each agent of cost x that do not conflict?
    */

    list<Agent> agentList = importAgents("resources/agents.txt");

    list<int> optimalCostList = calculateOptimalCost(agentList);

    Node root = generateRoot(optimalCostList);
    // printTree(&root);

    list<Node> queue;
    queue.push_back(root);
    

    ICTS(agentList, queue);

    return 0;
}

void ICTS(list<Agent> agentList, list<Node> queue) {

        Node currentNode = queue.front();
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
            generateNextLevel(&currentNode);
            for(auto child : currentNode.children) {
                ICTS(agentList, child);
            }
        }
}