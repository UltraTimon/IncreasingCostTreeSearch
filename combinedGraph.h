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
        
        int combine2Graphs(int stepsTaken, int costA, int currentA, int currentB, int finishA, int finishB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2, CombinedGraph *cg);



        void addSingleGraphToCombinedGraph(int stepsTaken, int cost, int graphListIndex, int currentCG, int currentG, vector<int> finishCG, int finishG, vector<vector<bool>> visitedCG, bool *visitedG, int maxNodes, CombinedGraph *cg, Graph *g, bool repeatCG, vector<int> repeatedIdList);
    
    public:
        CombinedGraph(int numberOfSteps) {
            // + 1 since you want to also store the first node. That one does require an index but does not require a step
            nodes = new vector<CombinedGraphNode>[numberOfSteps + 1];
        };
        
        void createCombinedgraph(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg);

};

#endif