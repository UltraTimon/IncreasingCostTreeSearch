#ifndef GRAPH_Header
#define GRAPH_Header

#include "basics.h"

class GraphNode
{
public:
    int id;
    vector<int> edges;
    bool useful;
    vector<bool> usefulWaypoint;
    GraphNode(int i, int numberOfWaypoints)
    {
        id = i;
        useful = false;
        for (int i = 0; i < numberOfWaypoints; i++)
        {
            usefulWaypoint.push_back(false);
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
    vector<int> waypoints;
    int end;
    vector<vector<int>> paths;
    Graph graph;
    void setPaths(vector<vector<int>> paths);

    Agent(int givenStart, int givenEnd)
    {
        start = givenStart;
        end = givenEnd;
    }
};

Graph importGraph(string filename);
vector<Agent> importAgents(string filename);

#endif