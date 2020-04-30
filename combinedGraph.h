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
        vector<CombinedGraphNode> *nodes;
    
    private:
        int combine2Graphs(int stepsTaken, int cost, int currentA, int currentB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2, CombinedGraph *cg);
    
    public:
        CombinedGraph(int numberOfSteps) {
            nodes = new vector<CombinedGraphNode>[numberOfSteps];
        };
        
        void createCombinedgraph(CombinedGraph *cg, vector<Agent> agentList, int cost);

};

#endif