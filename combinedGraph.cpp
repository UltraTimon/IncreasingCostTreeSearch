#include "combinedGraph.h"

int CombinedGraph::combine2Graphs(int stepsTaken, int cost, int currentA, int currentB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2,
                                  CombinedGraph *cg)
{
    if(stepsTaken >= cost)
        return -1;

    visitedA[currentA] = true;
    visitedB[currentB] = true;

    CombinedGraphNode cgn = CombinedGraphNode();
    int indexInMatrix = cg->nodes[stepsTaken].size();
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
                int indexOfChild = combine2Graphs(stepsTaken + 1, cost, edgeA, edgeB, newVisitedA, newVisitedB, g1, g2, cg);
                
                if(indexOfChild >= 0)
                    cgn.edges.push_back(indexOfChild);
            }
        }
    }

    cout << "idlist; " << cgn.idList.front() << " " << cgn.idList.back() << endl;
    cg->nodes[stepsTaken].push_back(cgn);
    return indexInMatrix;
}

// assuming there's 2 agents
void CombinedGraph::createCombinedgraph(CombinedGraph *cg, vector<Agent> agentList, int cost)
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

    int currentA = agentList[0].start;
    int currentB = agentList[1].start;

    cg->combine2Graphs(0, cost, currentA, currentB, visitedA, visitedB, g1, g2, cg);

    cout << "id lists: " << endl;
    for (int i = 0; i < cost; i++)
    {
        for(auto cgn : cg->nodes[i]) {
            for(auto id : cgn.idList) {
                cout << id << " ";
            }
            cout << endl;
        }
    }
    
}