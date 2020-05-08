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

// for general CombinedGraph, no restrictions on number of nodes
// remove illegal edges
bool CombinedGraph::combinedNodeIsUseful(int current, int graphListIndex, vector<int> endIdList, int stepsLeft, CombinedGraph *g)
{
    if (stepsLeft < 0 || (stepsLeft == 0 && !vectorEquals(g->nodes[graphListIndex][current].idList, endIdList)))
    {
        return false;
    }

    if (stepsLeft == 0 && vectorEquals(g->nodes[graphListIndex][current].idList, endIdList))
    {
        g->nodes[graphListIndex][current].useful = true;
        return true;
    }

    vector<int> edgesThatNeedToBeRemoved;

    if (stepsLeft > 0)
    {
        for (int edgeCounter = 0; edgeCounter < g->nodes[graphListIndex][current].edges.size(); edgeCounter++)
        {
            int edge = g->nodes[graphListIndex][current].edges[edgeCounter];
            if (combinedNodeIsUseful(edge, graphListIndex + 1, endIdList, stepsLeft - 1, g))
            {
                g->nodes[graphListIndex][current].useful = true;
            }
            else
            {
                edgesThatNeedToBeRemoved.push_back(edgeCounter);
            }
            // }
        }
    }

    // remove bad edges
    vector<int> edgesToCheck = g->nodes[graphListIndex][current].edges;
    vector<int> checkedEdges;
    for (int x = 0; x < edgesToCheck.size(); x++)
    {
        bool good = true;
        for (int i : edgesThatNeedToBeRemoved)
        {
            if (i == x)
                good = false;
        }
        if (good)
        {
            checkedEdges.push_back(x);
        }
    }
    g->nodes[graphListIndex][current].edges = checkedEdges;

    // return whether this node is a beneficial member of society
    if (g->nodes[graphListIndex][current].useful)
    {
        return true;
    }
    else
    {
        return false;
    }
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
    if(vectorContains(oldCG->nodes[graphListIndex][combinedGraphIndex].idList, singleGraphIndex))
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
    if(vectorContains(copyOfPreviousIdList, g->nodes[singleGraphIndex].id))
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

    // return -1 if SGN is the same as one of the ids in the idlist of the CGN
    if(vectorContains(oldCG->nodes[graphListIndex][combinedGraphIndex].idList, g->nodes[singleGraphIndex].id))
        return -1;

    // create new CGN object to store new data in
    CombinedGraphNode newCGN = CombinedGraphNode(g->nodes.size());

    // copy over the idList
    for (int i : oldCG->nodes[graphListIndex][combinedGraphIndex].idList)
    {
        newCGN.idList.push_back(i);
    }

    // repeat this CGN if needed
    if(oldCG->nodes[graphListIndex + 1].size() == 0) {
        return repeatCombinedNode(stepsLeft, graphListIndex, singleGraphIndex, g, newCG, newCGN.idList);
    }
    // repeat singleGraphNode if needed
    if(g->nodes[singleGraphIndex].id == singleGraphFinishID) {
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

















// combines graph, deflects conflicing node pairs
int CombinedGraph::combine2Graphs(int stepsTaken, int cost, int currentA, int currentB, int finishA, int finishB, bool *visitedA, bool *visitedB, Graph *g1, Graph *g2,
                                  CombinedGraph *cg)
{
    // if we are out of steps, stop building the graph
    if (stepsTaken > cost)
    {
        return -1;
    }

    // if the nodes are not useful, stop building the graph
    if (!g1->nodes[currentA].useful || !g2->nodes[currentB].useful)
    {
        return -1;
    }

    // if the indices are the same, we have a conflict and we can't use this path
    if (currentA == currentB)
    {
        return -1;
    }

    visitedA[currentA] = true;
    visitedB[currentB] = true;

    CombinedGraphNode cgn = CombinedGraphNode(g1->nodes.size());
    cgn.idList.push_back(currentA);
    cgn.idList.push_back(currentB);

    // if one of them is already at the final node, allow it to stay there until the other one takes its last steps
    bool aIsDone = currentA == finishA;
    bool bIsDone = currentB == finishB;

    vector<int> edgesA, edgesB;
    if (aIsDone && !bIsDone)
    {
        edgesA.push_back(currentA);
        visitedA[currentA] = false;
        edgesB = g2->nodes[currentB].edges;
    }
    else if (!aIsDone && bIsDone)
    {
        edgesA = g1->nodes[currentA].edges;
        edgesB.push_back(currentB);
        visitedB[currentB] = false;
    }
    else
    {
        edgesA = g1->nodes[currentA].edges;
        edgesB = g2->nodes[currentB].edges;
    }

    // make recursive calls
    for (int edgeA : edgesA)
    {
        for (int edgeB : edgesB)
        {
            if (!visitedA[edgeA] && !visitedB[edgeB])
            {
                // 2 agents cannot use the same edge from opposite sides at the same time
                if (!(edgeA == currentB && edgeB == currentA))
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

                    int indexOfChildX = combine2Graphs(stepsTaken + 1, cost, edgeA, edgeB, finishA, finishB, newVisitedA, newVisitedB, g1, g2, cg);

                    if (indexOfChildX >= 0)
                    {
                        cgn.edges.push_back(indexOfChildX);
                    }
                }
            }
        }
    }

    bool thisIsTheFinalStep = stepsTaken == cost;

    if (!thisIsTheFinalStep || (thisIsTheFinalStep && aIsDone && bIsDone))
    {
        int indexInMatrix = cg->nodes[stepsTaken].size();
        cg->nodes[stepsTaken].push_back(cgn);
        return indexInMatrix;
    }
    else
    {
        return -1;
    }
}