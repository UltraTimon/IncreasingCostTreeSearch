#ifndef COMBINED_GRAPH_Header
#define COMBINED_GRAPH_Header

#include "basics.h"
#include "graph.h"

class CombinedGraphNode 
{
    public:
        vector<int> idList;
        vector<int> edges;
        bool useful;
        CombinedGraphNode() {
            useful = false;
        }
};

class CombinedGraph
{
    public:
        int combine2Graphs(int stepsTaken, int currentA, int currentB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2, vector<CombinedGraphNode> *combinedGraphNodes);
        vector<CombinedGraphNode> nodes;
        CombinedGraph(vector<Agent> agentList);
};

#endif