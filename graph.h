#ifndef GRAPH_Header
#define GRAPH_Header

#include "basics.h"



class GraphNode 
{
    public:
        int id;
        vector<int> edges;
        bool useful;
        GraphNode(int i) {
            id = i;
            useful = false;
        }
};

class Graph
{
    public:
        vector<GraphNode> nodes;
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
        vector<vector<int>> paths;
        Graph graph;
        Agent(int start, int end);
        void setPaths(vector<vector<int>> paths);
};

Graph importGraph(string filename);
vector<Agent> importAgents(string filename);

#endif