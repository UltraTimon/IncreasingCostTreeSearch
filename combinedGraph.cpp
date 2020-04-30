#include "combinedGraph.h"

int CombinedGraph::combine2Graphs(int stepsTaken, int currentA, int currentB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2,
                                  vector<CombinedGraphNode> *combinedGraphNodes)
{

    // if (!g1->nodes[currentA].useful || !g2->nodes[currentB].useful)
    //     return -1;

    visitedA[currentA] = true;
    visitedB[currentB] = true;

    CombinedGraphNode cgn = CombinedGraphNode();
    int indexInMatrix = combinedGraphNodes[stepsTaken].size();
    combinedGraphNodes[stepsTaken].push_back(cgn);

    cgn.idList.push_back(currentA);
    cgn.idList.push_back(currentB);

    for (int edgeA : g1->nodes[currentA].edges)
    {
        for (int edgeB : g2->nodes[currentB].edges)
        {
            if (!visitedA[edgeA] && !visitedB[edgeB])
            {
                // make deepcopy of visited arrays to allow for weird loopy paths
                bool *newVisitedA = new bool[g1->nodes.size()];
                for (int j = 0; j < g1->nodes.size(); j++)
                {
                    newVisitedA[j] = visitedA[j];
                }
                bool *newVisitedB = new bool[g2->nodes.size()];
                for (int j = 0; j < g2->nodes.size(); j++)
                {
                    newVisitedB[j] = visitedB[j];
                }
                int indexOfChild = combine2Graphs(stepsTaken + 1, edgeA, edgeB, newVisitedA, newVisitedB, g1, g2, combinedGraphNodes);
                
                if(indexOfChild >= 0)
                    cgn.edges.push_back(indexOfChild);
            }
        }
    }

    return indexInMatrix;
}

// assuming there's 2 agents
CombinedGraph::CombinedGraph(vector<Agent> agentList)
{
    Graph *g1 = &agentList[0].graph;
    Graph *g2 = &agentList[1].graph;

    bool visitedA[g1->nodes.size()];
    bool visitedB[g2->nodes.size()];

    for (int i = 0; i < g1->nodes.size(); i++)
    {
        visitedA[i] = false;
    }
    for (int i = 0; i < g2->nodes.size(); i++)
    {
        visitedB[i] = false;
    }

    vector<CombinedGraphNode> combinedGraphNodes[max(g1->nodes.size(), g2->nodes.size())];

    int currentA = agentList[0].start;
    int currentB = agentList[1].start;

    combine2Graphs(0, currentA, currentB, visitedA, visitedB, g1, g2, combinedGraphNodes);

    cout << "id lists: " << endl;
    for(auto nodeList : combinedGraphNodes) {
        for(auto node : nodeList) {
            for(auto id : node.idList) {
                cout << id << " ";
            }
            cout << endl;
        }
    }
}