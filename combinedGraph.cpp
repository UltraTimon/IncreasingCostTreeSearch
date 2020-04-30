#include "combinedGraph.h"

// assuming size of vectors is equal
bool vectorEquals(vector<int> vecA, vector<int> vecB) {
    for (int i = 0; i < vecA.size(); i++)
    {
        if(vecA[i] != vecB[i])
            return false;
    }
    return true;
}

// for general CombinedGraph, no restrictions on number of nodes
bool combinedNodeIsUseful(int current, int graphListIndex, vector<int> endIdList, int stepsLeft, CombinedGraph *g, vector<vector<bool>> visited, int cost, int maxNodes)
{
    if (stepsLeft < 0 || (stepsLeft == 0 && !vectorEquals(g->nodes[graphListIndex][current].idList, endIdList))) {
        cout << "I stopped early" << endl;
        return false;
    }

    // \/\/ calculations for a 1D array if I need to use that

    // graphListIndex ranges from 0 to cost, since g->nodes is an aray of vectors, and the array is cost + 1 size
    // current is current index on the vector of the floor we're at
    // to get to the graphListIndex-th floor, we do graphListIndex * maxNodes, since the floors lie on the ground after eachother 
    // int visitedIndex = graphListIndex * maxNodes + current;

    visited.at(graphListIndex).at(current) = true;

    if (stepsLeft == 0 && vectorEquals(g->nodes[graphListIndex][current].idList, endIdList))
    {
        g->nodes[graphListIndex][current].useful = true;
        cout << "I am useful!" << endl;
        return true;
    }


    if (stepsLeft > 0)
    {
        for (int edge : g->nodes[graphListIndex][current].edges)
        {
            if (!visited.at(graphListIndex).at(current))
            {
                // make deepcopy of visited array s.t. other paths can still be discovered
                vector<vector<bool>> newVisited;
                for (int k = 0; k < cost + 1; k++)
                {
                    for (int j = 0; j < maxNodes; j++)
                    {
                        newVisited.at(k).at(j) = visited.at(k).at(j);
                    }
                }

                if (combinedNodeIsUseful(edge, graphListIndex + 1, endIdList, stepsLeft - 1, g, newVisited, cost, maxNodes))
                {
                    g->nodes[graphListIndex][current].useful = true;
                }
            }
        }
    }

    if (g->nodes[graphListIndex][current].useful)
    {
        return true;
    } else {
        return false;
    }
}


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

// assuming there's 2 agents, so 2 graphs
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
    vector<int> endList;
    for(auto agent : agentList) {
        endList.push_back(agent.end);
    }
    int tempMax = 0;
    for (int i = 0; i < cost + 1; i++)
    {
        int size = cg->nodes[i].size();
        if(size > tempMax)
            tempMax = size;
    }
    int maxNodes = tempMax;
    


    vector<vector<bool>> newVisited;
    for (int k = 0; k < cost + 1; k++)
    {
        vector<bool> temp;
        for (int j = 0; j < maxNodes; j++)
        {
            temp.push_back(false);
        }
        newVisited.push_back(temp);
    }

    combinedNodeIsUseful(0, 0, endList, cost, cg, newVisited, cost, maxNodes);

    cout << "id lists: " << endl;
    for (int i = 0; i <= cost; i++)
    {
        for(auto cgn : cg->nodes[i]) {
            if(cgn.useful) {
                for(auto id : cgn.idList) {
                    cout << id << " ";
                }
                cout << " - ";
            }
        }
        cout << endl;
    }

    cout << "egde lists: " << endl;
    for (int i = 0; i <= cost; i++)
    {
        for(auto cgn : cg->nodes[i]) {
            if(cgn.useful) {
                for(auto edge : cgn.edges) {
                    cout << edge << " ";
                }
                cout << endl;
            }
        }
    }
    
}