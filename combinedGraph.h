#ifndef COMBINED_GRAPH_Header
#define COMBINED_GRAPH_Header

#include "basics.h"
#include "graph.h"

class CombinedGraphNode 
{


    public:
        int numberOfNodes;
        vector<int> idList;
        vector<int> edges;
        bool useful;
        CombinedGraphNode(int nrOfNodes) {
            numberOfNodes = nrOfNodes;
            useful = false;
            idList.reserve(nrOfNodes * nrOfNodes);
            edges.reserve(nrOfNodes * nrOfNodes);
        }
        CombinedGraphNode copy(CombinedGraphNode toCopy) {
            CombinedGraphNode newNode = CombinedGraphNode(toCopy.numberOfNodes);
            for(int i : toCopy.idList)
                newNode.idList.push_back(i);
            for(int i : toCopy.edges)
                newNode.edges.push_back(i);
            newNode.useful = toCopy.useful;

            return newNode;
        }
};

class CombinedGraph
{
    private:
        int steps;

    public: 
        vector<CombinedGraphNode> *nodes;

        int combine2Graphs(int stepsTaken, int costA, int currentA, int currentB, int waypointA, int waypointB, bool reachedWaypointA, bool reachedWaypointB, int finishA, int finishB, Graph *g1, Graph *g2, CombinedGraph *cg);
        
        bool combinedNodeIsUseful(int current, int graphListIndex, vector<int> endIdList, int stepsLeft, CombinedGraph *g);

        int copyOldCombinedNodeToNewCombinedNodeWithSingleGraphNodeIncluded(int stepsLeft, int graphListIndex, int combinedGraphIndex, CombinedGraph *cg, int singleGraphIndex, Graph *g, CombinedGraph *newCG, int singleGraphFinishID);
        
        
    public:
        CombinedGraph(int numberOfSteps) {
            // + 1 since you want to also store the first node. That one does require an index but does not require a step
            steps = numberOfSteps;
            nodes = new vector<CombinedGraphNode>[numberOfSteps + 1];
        };
        
        CombinedGraph createCombinedgraph(vector<Agent> agentList, vector<int> optimalCostList, int maxCost);
        void removeIllegalEdges(CombinedGraph *cg, int graphListIndex, int currentCombinedGraphIndex, int maxCost);

};

#endif