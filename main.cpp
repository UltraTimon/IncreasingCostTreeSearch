#include "basics.h"
#include "icts.h"
#include "generateTree.h"

using namespace std;

bool verbose;

int main(int argc, char **argv)
{
    verbose = true;

    /*
        Main questions for this algorithm:

        1) Does there exist a path of cost x?
        2) If yes, and if we are going to examine this cost node, does there exist a path for each agent of cost x that do not conflict?
    */

    vector<Agent> agentList = importAgents("resources/agents.txt");

    for(int i = 0; i < agentList.size(); i++) {
        agentList[i].graph = importGraph("resources/graph.txt");
    }

    vector<int> optimalCostList = calculateOptimalCost(agentList);

    Node root = generateRoot(optimalCostList);

    printTree(&root, 3);

    
    list<Node> queue;
    queue.push_back(root);
    
    // Run ICTS algorithm
    ICTS(agentList, queue);


    return 0;
}

