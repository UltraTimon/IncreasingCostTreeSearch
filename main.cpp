#include "icts.h"
#include "generateTree.h"

using namespace std;

int main(int argc, char **argv)
{
    /*
        Main questions for this algorithm:

        1) Does there exist a path of cost x?
        2) If yes, and if we are going to examine this cost node, does there exist a path for each agent of cost x that do not conflict?
    */

    vector<Agent> agentList = importAgents("resources/agents.txt");
    Graph g = importGraph("resources/graph.txt");

    vector<int> optimalCostList = calculateOptimalCost(agentList, &g);

    // TESTING generatePaths() -------------
    vector<vector<int>> pathss;
    generatePaths(&g, 0, 6, 4, &pathss);
    
    // print paths
    cout << "paths: " << endl;
    for(auto list : pathss) {
        for(int i : list) {
            cout << i << " ";
        }
        cout << endl;
    }
    // end of TESTING generatePaths() -------------

    Node root = generateRoot(optimalCostList);

    list<Node> queue;
    queue.push_back(root);
    
    // Run ICTS algorithm
    ICTS(agentList, queue);

    return 0;
}

