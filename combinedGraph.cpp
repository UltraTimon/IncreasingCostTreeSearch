#include "combinedGraph.h"

int CombinedGraph::combine2Graphs(int stepsTaken, int cost, int currentA, int currentB, int finishA, int finishB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2,
                                  CombinedGraph *cg)
{
    // if we are out of steps, stop building the graph
    if(stepsTaken > cost)
        return -1;

    // if the nodes are not useful, stop building the graph
    if(!g1->nodes[currentA].useful || !g2->nodes[currentB].useful)
        return -1;

    visitedA[currentA] = true;
    visitedB[currentB] = true;

    CombinedGraphNode cgn = CombinedGraphNode();
    cgn.idList.push_back(currentA);
    cgn.idList.push_back(currentB);

    cout << "current: " << currentA << " and " << currentB << endl;


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
                cout << "recursive call to " << edgeA << " and " << edgeB << endl; 
                int indexOfChild = combine2Graphs(stepsTaken + 1, cost, edgeA, edgeB, finishA, finishB, newVisitedA, newVisitedB, g1, g2, cg);
                
                if(indexOfChild >= 0)
                    cgn.edges.push_back(indexOfChild);
            }
        }
    }

    bool thisIsTheFinalStep = stepsTaken == cost;
    bool aIsDone = currentA == finishA;
    bool bIsDone = currentB == finishB;

    if(!thisIsTheFinalStep || (thisIsTheFinalStep && aIsDone && bIsDone)) {
        cout << "adding cgn with: " << currentA << " and " << currentB << ", #steps: " << stepsTaken << endl;
        int indexInMatrix = cg->nodes[stepsTaken].size();
        cg->nodes[stepsTaken].push_back(cgn);
        return indexInMatrix;
    } else {
        return -1;
    }
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

    int startA = agentList[0].start;
    int startB = agentList[1].start;

    int finishA = agentList[0].end;
    int finishB = agentList[1].end;

    cg->combine2Graphs(0, cost, startA, startB, finishA, finishB, visitedA, visitedB, g1, g2, cg);

    cout << "id lists: " << endl;
    for (int i = 0; i <= cost; i++)
    {
        for(auto cgn : cg->nodes[i]) {
            for(auto id : cgn.idList) {
                cout << id << " ";
            }
            cout << " - ";
        }
        cout << endl;
    }

    cout << "egde lists: " << endl;
    for (int i = 0; i <= cost; i++)
    {
        for(auto cgn : cg->nodes[i]) {
            for(auto edge : cgn.edges) {
                cout << edge << " ";
            }
            cout << endl;
        }
    }
    
}