#include "combinedGraph.h"

// an edge is illegal if two agents try to use it at the same time from opposite nodes
void CombinedGraph::removeIllegalEdges(CombinedGraph *cg, int graphListIndex, int currentCombinedGraphIndex, int maxCost)
{
    // the nodes at the last index have no edges
    // maxCost is the last level since cg has maxCost + 1 node lists (see constructor)
    if (graphListIndex == maxCost)
        return;

    CombinedGraphNode currentNode = cg->nodes[graphListIndex][currentCombinedGraphIndex];

    for (int i : currentNode.edges)
    {
        removeIllegalEdges(cg, graphListIndex + 1, i, maxCost);
    }

    if(verbose) {
        printf("Removing edges from node (");
        for (int i : currentNode.idList)
            cout << i << " ";
        cout << ")" << endl;
    }

    vector<int> goodEdges;

    for (int x : currentNode.edges)
    {
        bool goodEdge = true;
        CombinedGraphNode checkNode = cg->nodes[graphListIndex + 1][x];
        for (int i = 0; i < currentNode.idList.size(); i++)
        {
            int currentA = currentNode.idList[i];
            int currentB = checkNode.idList[i];

            for (int j = i + 1; j < currentNode.idList.size(); j++)
            {
                int nextA = currentNode.idList[j];
                int nextB = checkNode.idList[j];

                if (currentA == nextB && currentB == nextA)
                {
                    // there exist two agents that want to use the same edge
                    goodEdge = false;

                    printf("Removing edge (");
                    for (int i : checkNode.idList)
                        cout << i << " ";
                    cout << ")" << endl;

                    break;
                }
            }

            if (!goodEdge)
                break;
        }
        if (goodEdge)
            goodEdges.push_back(x);
    }

    // replace edges list for this node with good edges that have no conflict
    cg->nodes[graphListIndex][currentCombinedGraphIndex].edges.clear();
    for (int i : goodEdges)
        cg->nodes[graphListIndex][currentCombinedGraphIndex].edges.push_back(i);
}

int repeatA(int stepsTaken, int cost, int currentB, int finishA, int finishB, Graph *g1, Graph *g2, CombinedGraph *cg) {
    // if we are out of steps, stop building the graph
    if (stepsTaken > cost)
    {
        return -1;
    }
    // if the indices are the same, we have a conflict and we can't use this path
    if (finishA == currentB)
    {
        return -1;
    }
    if(!g2->nodes[currentB].useful)
        return -1;

    // create the new node
    CombinedGraphNode cgn = CombinedGraphNode(g1->nodes.size());
    cgn.idList.push_back(finishA);
    cgn.idList.push_back(currentB);

    if(currentB == finishB) {
        cg->nodes[stepsTaken].push_back(cgn);
        return cg->nodes[stepsTaken].size() - 1;
    }

    for(int edgeB : g2->nodes[currentB].edges) {
        if(!(finishA == edgeB && finishA == currentB)) {
            int indexOfChild = repeatA(stepsTaken + 1, cost, edgeB, finishA, finishB, g1, g2, cg);

            if (indexOfChild >= 0)
            {
                cgn.edges.push_back(indexOfChild);
            }
        }
    }

    cg->nodes[stepsTaken].push_back(cgn);
    return cg->nodes[stepsTaken].size() - 1;
}

int repeatB(int stepsTaken, int cost, int currentA, int finishA, int finishB, Graph *g1, Graph *g2, CombinedGraph *cg) {
    // if we are out of steps, stop building the graph
    if (stepsTaken > cost)
    {
        return -1;
    }
    // if the indices are the same, we have a conflict and we can't use this path
    if (finishB == currentA)
    {
        return -1;
    }
    if(!g1->nodes[currentA].useful)
        return -1;

    // create the new node
    CombinedGraphNode cgn = CombinedGraphNode(g1->nodes.size());
    cgn.idList.push_back(finishB);
    cgn.idList.push_back(currentA);

    if(currentA == finishA) {
        cg->nodes[stepsTaken].push_back(cgn);
        return cg->nodes[stepsTaken].size() - 1;
    }

    for(int edgeA : g1->nodes[currentA].edges) {
        if(!(finishB == edgeA && finishB == currentA)) {
            int indexOfChild = repeatA(stepsTaken + 1, cost, edgeA, finishA, finishB, g1, g2, cg);

            if (indexOfChild >= 0)
            {
                cgn.edges.push_back(indexOfChild);
            }
        }
    }

    cg->nodes[stepsTaken].push_back(cgn);
    return cg->nodes[stepsTaken].size() - 1;
}

// combines graph, deflects conflicing node pairs
int CombinedGraph::combine2Graphs(int stepsTaken, int cost, int currentA, int currentB, int finishA, int finishB, Graph *g1, Graph *g2,
                                  CombinedGraph *cg)
{
    // if we are out of steps, stop building the graph
    if (stepsTaken > cost)
    {
        return -1;
    }

    // if the indices are the same, we have a conflict and we can't use this path
    if (currentA == currentB)
    {
        return -1;
    }

    if(!g1->nodes[currentA].useful || !g2->nodes[currentB].useful)
        return -1;

    if(currentA == finishA)
        repeatA(stepsTaken, cost, currentB, finishA, finishB, g1, g2, cg);

    if(currentB == finishB)
        repeatB(stepsTaken, cost, currentA, finishA, finishB, g1, g2, cg);

    CombinedGraphNode cgn = CombinedGraphNode(g1->nodes.size());
    cgn.idList.push_back(currentA);
    cgn.idList.push_back(currentB);

    // make recursive calls
    for (int edgeA : g1->nodes[currentA].edges)
    {
        for (int edgeB : g2->nodes[currentB].edges)
        {
            // 2 agents cannot use the same edge from opposite sides at the same time
            if (!(edgeA == currentB && edgeB == currentA))
            {
                int indexOfChild = combine2Graphs(stepsTaken + 1, cost, edgeA, edgeB, finishA, finishB, g1, g2, cg);

                if (indexOfChild >= 0)
                {
                    cgn.edges.push_back(indexOfChild);
                }
            }
        }
    }

    cg->nodes[stepsTaken].push_back(cgn);
    return cg->nodes[stepsTaken].size() - 1;
   
}