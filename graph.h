#ifndef GRAPH_Header
#define GRAPH_Header

#include "basics.h"

class GraphNode
{
public:
    int id;
    vector<int> edges;
    bool useful;
    bool *usefulWaypoint;
    GraphNode(int i, int numberOfWaypoints)
    {
        id = i;
        useful = false;
        bool usefulWaypoint[numberOfWaypoints];
        for (int i = 0; i < numberOfWaypoints; i++)
        {
            usefulWaypoint[i] = false;
        }
    }
};

class Graph
{
public:
    vector<GraphNode> nodes;
    int stepsLeft; // steps left after reaching the waypoint
    int numberOfWaypoints;
    Graph(int nrOfNodes, int nrOfWaypoints)
    {
        for (int i = 0; i < nrOfNodes; i++)
        {
            GraphNode node = GraphNode(i, numberOfWaypoints);
            nodes.push_back(node);
        }
        numberOfWaypoints = nrOfWaypoints;
    }
    // for Agent initialization
    Graph() {}
};

class Agent
{
public:
    int start;
    int *waypoints;
    int end;
    vector<vector<int>> paths;
    Graph graph;
    void setPaths(vector<vector<int>> paths);

    Agent(int givenStart, vector<int> givenWaypoints, int givenEnd, int numberOfWaypoints)
    {
        start = givenStart;

        int waypoints[numberOfWaypoints];
        for (int i = 0; i < numberOfWaypoints; i++)
        {
            waypoints[i] = givenWaypoints[i];
        }

        end = givenEnd;
    }
};

Graph importGraph(string filename);
vector<Agent> importAgents(string filename);

#endif