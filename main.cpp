#include "generatePaths.h"
#include "agent.h"
#include "lowlevelsearch.h"
#include "generateTree.h"

using namespace std;

int main(int argc, char **argv)
{
    // get list of agent objects read form file
    list<Agent> agentList = importAgents("resources/agents.txt");
    int numberOfAgents = agentList.size();

    // // generate paths for each agent, add paths to agent object
    // // This will first only determine what the optimal cost is for every agent by calculating paths with an iteratively increasing cost until
    // //      each agent has at least one path with that cost
    int optimalCost = 1;
    bool everyAgentHasAtLeastOnePath = false;
    list<Agent>::iterator agentIterator;

    while (!everyAgentHasAtLeastOnePath)
    {
        for (auto agentIterator = agentList.begin(); agentIterator != agentList.end(); ++agentIterator)
        {
            list<list<int>> generatedPaths = generatePaths("resources/graph.txt", agentIterator->start, agentIterator->end, optimalCost);
            if (generatedPaths.size() == 0)
            {
                optimalCost++;
                cout << "optimalCost increased to " << optimalCost << endl;
                break;
            }
            else
            {
                cout << "every agent has at least one path with length " << optimalCost << endl;
                everyAgentHasAtLeastOnePath = true;
                break;
            }
        }
    }

    /*
        1) Does there exist a path of cost x?
        2) If yes, and we are going to examine this cost node, does there exist a path for each agent of that cost that do not conflict?
    */
    Node root = generateRoot(optimalCost, numberOfAgents);
    root.children = generateNextLevelOfChildren(root);

    generateNextLevel(&root);
    generateNextLevel(&root);

    printTree(&root); 

    return 0;
}