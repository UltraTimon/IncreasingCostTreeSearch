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
    int nrOfWaypoints;
    int start, end, waypoint;



    if (myfile.is_open())
    {   
        myfile >> nrOfWaypoints;
        while(myfile.peek() != EOF) {
            myfile >> start;
            vector<int> waypoints;
            for (int i = 0; i < nrOfWaypoints; i++)
            {
                myfile >> waypoint;
                waypoints.push_back(waypoint);
            }
            Agent agent = Agent(start, waypoints, end, nrOfWaypoints);
            agentList.push_back(agent);
        }
        
        myfile.close();
    }
    else
    {
        cout << "ImportAgents: Unable to open file. Sorry" << endl;
    }

    return agentList;
}