#include "graph.h"

bool nodeIsUseful(int current, int end, int stepsLeft, Graph *g, bool visited[]) {
    
    if(stepsLeft < 0 || (stepsLeft == 0 && current != end))
        return 0;
    
    visited[current] = true;

    if(stepsLeft == 0 && current == end) {
        g->nodes[current].useful = true;
        cout << current << "is useful and final" << endl;
        return true;
    }

    if(stepsLeft > 0) {
        for(int i : g->nodes[current].edges) {
            if(nodeIsUseful(i, end, stepsLeft - 1, g, visited)) {
                g->nodes[current].useful = true;
                cout << current << "is useful" << endl;
            }
        }
    }

    if (g->nodes[current].useful) {
        return true;
    }
}

// Driver program
void generatePaths(string filename, int start, int end, int exactCost, vector<vector<int>> *paths)
{

    // Reading in the edges from a file
    ifstream myfile(filename);
    int nodeA, nodeB; 
    int nrOfNodes = 999;
    if (myfile.is_open())
    {
        myfile >> nrOfNodes;
    }
    Graph g = Graph(nrOfNodes);
    if (myfile.is_open())
    {
        while (myfile >> nodeA >> nodeB)
        {
            g.nodes[nodeA].edges.push_back(nodeB);
            g.nodes[nodeB].edges.push_back(nodeA);
        }
        myfile.close();
    }
    else
    {
        cout << "generatePaths: Unable to open file. Sorry" << endl;
    }

    // find paths
    bool visited[nrOfNodes];
    for (int i = 0; i < nrOfNodes; i++)
    {
        visited[i] = false;
    }
    nodeIsUseful(start, end, exactCost, &g, visited);
    
    cout << "all useful nodes: ";
    for (int i = 0; i < nrOfNodes; i++)
    {
        if(g.nodes[i].useful) {
            cout << i << " ";
        }
    }
}

// generate paths for each agent, add paths to agent object
// This will first only determine what the optimal cost is for every agent by calculating paths with an iteratively increasing cost until
//      each agent has at least one path with that cost
vector<int> calculateOptimalCost(vector<Agent> agentList) {
    vector<int> optimalCostList;

    // for (auto agentIterator = agentList.begin(); agentIterator != agentList.end(); ++agentIterator)
    // {
    //     int optimalCost = 1; //  assuming a minimal cost of 1 for each agent
    //     while(true) {
    //         vector<vector<int>> generatedPaths = generatePaths("resources/graph.txt", agentIterator->start, agentIterator->end, optimalCost);
    //         if (generatedPaths.size() == 0)
    //         {
    //             optimalCost++;
    //         }
    //         else
    //         {
    //             optimalCostList.push_back(optimalCost);
    //             break;
    //         }
    //     }
    // }

    // for testing
    optimalCostList.push_back(3);

    return optimalCostList;
}