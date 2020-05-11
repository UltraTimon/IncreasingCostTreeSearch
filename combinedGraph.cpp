#include "combinedGraph.h"

bool vectorEquals(vector<int> vecA, vector<int> vecB)
{
    if (vecA.size() != vecB.size())
        return false;
    for (int i = 0; i < vecA.size(); i++)
    {
        if (vecA[i] != vecB[i])
            return false;
    }
    return true;
}

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

// for general CombinedGraph, no restrictions on number of nodes
// remove illegal edges
bool CombinedGraph::combinedNodeIsUseful(int current, int graphListIndex, vector<int> endIdList, int stepsLeft, CombinedGraph *g)
{
    if (stepsLeft < 0)
    {
        return false;
    }

    if (vectorEquals(g->nodes[graphListIndex][current].idList, endIdList))
    {
        g->nodes[graphListIndex][current].useful = true;
        return true;
    }

    if (stepsLeft > 0)
    {
        for (int edge : g->nodes[graphListIndex][current].edges)
        {
            if (combinedNodeIsUseful(edge, graphListIndex + 1, endIdList, stepsLeft - 1, g))
            {
                g->nodes[graphListIndex][current].useful = true;
            }
        }
    }

    // return whether this node is a beneficial member of society
    return g->nodes[graphListIndex][current].useful;
}

bool vectorContains(vector<int> vec, int x)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec.at(i) == x)
        {
            return true;
        }
    }
    return false;
}

bool usingTheSameEdge(vector<int> currentIdList, vector<int> nextIdList, int currentId, int nextId)
{
    for (int i = 0; i < currentIdList.size(); i++)
    {
        if (currentIdList[i] == nextId && nextIdList[i] == currentId)
        {
            return true;
        }
    }
    return false;
}

int repeatSingleGraphNode(int stepsLeft, int graphListIndex, int combinedGraphIndex, CombinedGraph *oldCG, int singleGraphIndex, CombinedGraph *newCG, int amountOfNodes)
{
    // return if no steps are allowed to be taken
    if (stepsLeft < 0)
        return -1;

    // return -1 if SGN is the same as one of the ids in the idlist of the CGN
    if (vectorContains(oldCG->nodes[graphListIndex][combinedGraphIndex].idList, singleGraphIndex))
        return -1;

    // create new CGN object to store new data in
    CombinedGraphNode newCGN = CombinedGraphNode(amountOfNodes);

    // copy over the idList
    for (int i : oldCG->nodes[graphListIndex][combinedGraphIndex].idList)
    {
        newCGN.idList.push_back(i);
    }

    // add id of singleGraphNode to new CGN
    newCGN.idList.push_back(singleGraphIndex);

    // for combinedGraphEdge :  CombinedGraphNode.edges
    for (int combinedGraphEdge : oldCG->nodes[graphListIndex][combinedGraphIndex].edges)
    {
        // make a recursive call to it in which you specify the index it's placed in
        int indexOfNewChild = repeatSingleGraphNode(stepsLeft - 1, graphListIndex + 1, combinedGraphEdge, oldCG, singleGraphIndex, newCG, amountOfNodes);

        if (indexOfNewChild >= 0)
        {
            newCGN.edges.push_back(indexOfNewChild);
        }
    }

    // push newCGN object into newCG graphListIndex + 1's list
    int newCGNIndex = newCG->nodes[graphListIndex].size();
    newCG->nodes[graphListIndex].push_back(newCGN);

    return newCGNIndex;
}

int repeatCombinedNode(int stepsLeft, int graphListIndex, int singleGraphIndex, Graph *g, CombinedGraph *newCG, vector<int> copyOfPreviousIdList)
{
    // return if no steps are allowed to be taken
    if (stepsLeft < 0)
        return -1;

    // return -1 if SGN is the same as one of the ids in the idlist of the CGN
    if (vectorContains(copyOfPreviousIdList, g->nodes[singleGraphIndex].id))
        return -1;

    // create new CGN object to store new data in
    CombinedGraphNode newCGN = CombinedGraphNode(g->nodes.size());

    // copy attributes from copied IDList that is pointed to by the combinedGraphEdge to new CGN object
    for (int i : copyOfPreviousIdList)
    {
        newCGN.idList.push_back(i);
    }

    // add id of singleGraphNode to new CGN
    newCGN.idList.push_back(g->nodes[singleGraphIndex].id);

    // for SingleGraphEdge :  SingleGraphNode.edges
    for (int singleGraphEdge : g->nodes[singleGraphIndex].edges)
    {
        // make a recursive call to it in which you specify the index it's placed in
        int indexOfNewChild = repeatCombinedNode(stepsLeft - 1, graphListIndex + 1, singleGraphEdge, g, newCG, copyOfPreviousIdList);

        if (indexOfNewChild >= 0)
        {
            newCGN.edges.push_back(indexOfNewChild);
        }
    }

    // push newCGN object into newCG graphListIndex + 1's list
    int newCGNIndex = newCG->nodes[graphListIndex].size();
    newCG->nodes[graphListIndex].push_back(newCGN);

    return newCGNIndex;
}

// combines graph, deflects conflicing node pairs
int CombinedGraph::copyOldCombinedNodeToNewCombinedNodeWithSingleGraphNodeIncluded(int stepsLeft, int graphListIndex, int combinedGraphIndex, CombinedGraph *oldCG, int singleGraphIndex, Graph *g, CombinedGraph *newCG, int singleGraphFinishID)
{
    // return if no steps are allowed to be taken
    if (stepsLeft < 0)
        return -1;

    if (oldCG->nodes[graphListIndex].empty())
        return -1;

    // return -1 if SGN is the same as one of the ids in the idlist of the CGN
    if (vectorContains(oldCG->nodes[graphListIndex][combinedGraphIndex].idList, g->nodes[singleGraphIndex].id))
        return -1;

    // create new CGN object to store new data in
    CombinedGraphNode newCGN = CombinedGraphNode(g->nodes.size());

    // copy over the idList
    for (int i : oldCG->nodes[graphListIndex][combinedGraphIndex].idList)
    {
        newCGN.idList.push_back(i);
    }

    // repeat this CGN if needed
    if (oldCG->nodes[graphListIndex + 1].size() == 0)
    {
        return repeatCombinedNode(stepsLeft, graphListIndex, singleGraphIndex, g, newCG, newCGN.idList);
    }
    // repeat singleGraphNode if needed
    if (g->nodes[singleGraphIndex].id == singleGraphFinishID)
    {
        return repeatSingleGraphNode(stepsLeft, graphListIndex, combinedGraphIndex, oldCG, singleGraphIndex, newCG, g->nodes.size());
    }

    // add id of singleGraphNode to new CGN
    newCGN.idList.push_back(g->nodes[singleGraphIndex].id);

    // for SingleGraphEdge :  SingleGraphNode.edges
    for (int singleGraphEdge : g->nodes[singleGraphIndex].edges)
    {
        // for CombinedGraphEdge :  CombinedGraphNode.edges
        for (int combinedGraphEdge : oldCG->nodes[graphListIndex][combinedGraphIndex].edges)
        {
            // make a recursive call to it in which you specify the index it's placed in
            int indexOfNewChild = copyOldCombinedNodeToNewCombinedNodeWithSingleGraphNodeIncluded(stepsLeft - 1, graphListIndex + 1, combinedGraphIndex, oldCG, singleGraphEdge, g, newCG, singleGraphFinishID);

            if (indexOfNewChild >= 0)
            {
                newCGN.edges.push_back(indexOfNewChild);
            }
        }
    }

    // push newCGN object into newCG graphListIndex's list
    int newCGNIndex = newCG->nodes[graphListIndex].size();
    newCG->nodes[graphListIndex].push_back(newCGN);

    return newCGNIndex;
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