#include "graph.h"

Graph importGraph(string filename) {
    // Reading in the edges from a file
    ifstream myfile(filename);
    int nodeA, nodeB;
    int nrOfNodes = 999;
    int nrOfWaypoints = 999;
    if (myfile.is_open())
    {
        myfile >> nrOfNodes;
        myfile >> nrOfWaypoints;
    }
    Graph g = Graph(nrOfNodes, nrOfWaypoints);
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

    // add self edges
    for (int i = 0; i < g.nodes.size(); i++)
    {
        g.nodes[i].edges.push_back(i);
    }

    return g;
}

void Agent::setPaths(vector<vector<int>> givenPaths) {
    paths = givenPaths;
}


vector<Agent> importAgents(string filename) {
    vector<Agent> agentList;

    ifstream myfile(filename);
    int start, end, waypoint;

    if (myfile.is_open())
    {
        while (myfile >> start >> waypoint >> end)
        {
            Agent newAgent = Agent(start, waypoint, end);
            agentList.push_back(newAgent);
        }
        myfile.close();
    }
    else
    {
        cout << "ImportAgents: Unable to open file. Sorry" << endl;
    }

    return agentList;
}