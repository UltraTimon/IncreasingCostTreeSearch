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
        int combine2Graphs(int stepsTaken, int cost, int currentA, int currentB, int finishA, int finishB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2, CombinedGraph *cg);
    
    public:
        CombinedGraph(int numberOfSteps) {
            // + 1 since you want to also store the first node. That one does require an index but does not require a step
            nodes = new vector<CombinedGraphNode>[numberOfSteps + 1];
        };
        
        void createCombinedgraph(CombinedGraph *cg, vector<Agent> agentList, int cost);

};

#endif