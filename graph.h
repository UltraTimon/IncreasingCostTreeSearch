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
        GraphNode(int i, int numberOfWaypoints) {
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
        Graph(int nrOfNodes) {
            for (int i = 0; i < nrOfNodes; i++)
            {
                GraphNode node = GraphNode(i);
                nodes.push_back(node);
            }
        }
        // for Agent initialization
        Graph() {}
};


class Agent {
    public:
        int start;
        int end;
        int waypoint;
        vector<vector<int>> paths;
        Graph graph;
        Agent(int start, int end, int waypoint);
        void setPaths(vector<vector<int>> paths);
};

Graph importGraph(string filename);
vector<Agent> importAgents(string filename);

#endif