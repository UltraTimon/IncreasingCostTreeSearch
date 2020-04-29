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
    private:
        void combineGraph(int *current, int stepsLeft, vector<bool> *visited, deque<int> *pathUpToNow, vector<Graph*> graphList, 
            vector<CombinedGraphNode> *combinedGraphNodes, int previousCombinedGraphNode);

    public:
        vector<CombinedGraphNode> nodes;
        CombinedGraph(vector<Graph*> graphList, vector<Agent> agentList);
};

#endif