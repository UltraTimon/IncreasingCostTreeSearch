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
        CombinedGraphNode(int nrOfNodes) {
            useful = false;
            idList.reserve(nrOfNodes * nrOfNodes);
            edges.reserve(nrOfNodes * nrOfNodes);
        }
};

class CombinedGraph
{
    public: 
        vector<CombinedGraphNode> *nodes;

        int combine2Graphs(int stepsTaken, int costA, int currentA, int currentB, int finishA, int finishB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2, CombinedGraph *cg);
        
        bool combinedNodeIsUseful(int current, int graphListIndex, vector<int> endIdList, int stepsLeft, CombinedGraph *g);

        int copyOldCombinedNodeToNewCombinedNodeWithSingleGraphNodeIncluded(int stepsLeft, int graphListIndex, int combinedGraphIndex, CombinedGraph *cg, int singleGraphIndex, Graph *g, CombinedGraph *newCG, int singleGraphFinishID);
        
        
    public:
        CombinedGraph(int numberOfSteps) {
            // + 1 since you want to also store the first node. That one does require an index but does not require a step
            nodes = new vector<CombinedGraphNode>[numberOfSteps + 1];
        };
        
        void createCombinedgraph(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg);

};

#endif