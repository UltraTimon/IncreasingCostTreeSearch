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

    // generateNextLevel(&root);
    // generateNextLevel(&root);
    // generateNextLevel(&root);

    printTree(&root); 

    getAtLeastOnePathPerAgentWithoutConflict(agentList, optimalCostList);



    

    return 0;
}