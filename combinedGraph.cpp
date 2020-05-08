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
bool CombinedGraph::combinedNodeIsUseful(int current, int graphListIndex, vector<int> endIdList, int stepsLeft, CombinedGraph *g, vector<vector<bool>> visited, int cost, int maxNodes)
{
    if (stepsLeft < 0 || (stepsLeft == 0 && !vectorEquals(g->nodes[graphListIndex][current].idList, endIdList)))
    {
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
        return true;
    }

    vector<int> edgesThatNeedToBeRemoved;

    if (stepsLeft > 0)
    {
        for (int edgeCounter = 0; edgeCounter < g->nodes[graphListIndex][current].edges.size(); edgeCounter++)
        {
            int edge = g->nodes[graphListIndex][current].edges[edgeCounter];
            if (!visited.at(graphListIndex + 1).at(edge))
            {
                // make deepcopy of visited array s.t. other paths can still be discovered
                vector<vector<bool>> newVisited;
                for (int k = 0; k < cost + 1; k++)
                {
                    vector<bool> temp;
                    for (int j = 0; j < maxNodes; j++)
                    {
                        temp.push_back(visited.at(k).at(j));
                    }
                    newVisited.push_back(temp);
                }

                if (combinedNodeIsUseful(edge, graphListIndex + 1, endIdList, stepsLeft - 1, g, newVisited, cost, maxNodes))
                {
                    g->nodes[graphListIndex][current].useful = true;
                }
                else
                {
                    edgesThatNeedToBeRemoved.push_back(edgeCounter);
                }
            }
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











/*
Checks to be done;
- on the same node
- trying to use the same edge
- nodes useful or not
- visited

*/

// combines graph, deflects conflicing node pairs
int CombinedGraph::copyOldCombinedNodeToNewCombinedNodeWithSingleGraphNodeIncluded(int stepsLeft, int graphListIndex, int combinedGraphIndex, CombinedGraph *oldCG, int singleGraphIndex, Graph *g, CombinedGraph *newCG)
{
    // return if no steps are allowed to be taken
    if (stepsLeft < 0)
        return -1;

    // create new CGN object to store new data in
    CombinedGraphNode newCGN = CombinedGraphNode(g->nodes.size());

    // copy attributes from CGN that is pointed to by the combinedGraphEdge to new CGN object
    for (int i : oldCG->nodes[graphListIndex][combinedGraphIndex].idList)
    {
        newCGN.idList.push_back(i);
    }

    // add id of singleGraphNode to new CGN
    newCGN.idList.push_back(g->nodes[singleGraphIndex].id);


    // push newCGN object into newCG graphListIndex + 1's list

    // for SingleGraphEdge :  SingleGraphNode.edges
    for (int singleGraphEdge : g->nodes[singleGraphIndex].edges)
    {

        // for CombinedGraphEdge :  CombinedGraphNode.edges
        for (int combinedGraphEdge : oldCG->nodes[graphListIndex][combinedGraphIndex].edges)
        {

            // make a recursive call to it in which you specify the index it's placed in
            int indexOfNewChild = copyOldCombinedNodeToNewCombinedNodeWithSingleGraphNodeIncluded(stepsLeft - 1, graphListIndex + 1, combinedGraphIndex, oldCG, singleGraphEdge, g, newCG);

            if(indexOfNewChild >= 0) {
                newCGN.edges.push_back(indexOfNewChild);
            }
        }
    }

    int newCGNIndex = newCG->nodes[graphListIndex].size();
    newCG->nodes[graphListIndex].push_back(newCGN);

    printf("Created a new node with id: ");
    for(int id : newCG->nodes[graphListIndex][newCGNIndex].idList)
        cout << id << " ";
    printf(" and edges: ");
    for(int edge : newCG->nodes[graphListIndex][newCGNIndex].edges)
        cout << edge << " ";
    cout << endl;

    printf("returning index: %d\n", newCGNIndex);
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
        // visitedA[currentA] = false;
        // visitedB[currentB] = false;
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
                        if (verbose)
                            cout << "adding edge from (" << currentA << ", " << currentB << ") to (" << cg->nodes[stepsTaken + 1][indexOfChildX].idList.front() << ", " << cg->nodes[stepsTaken + 1][indexOfChildX].idList.back() << ")" << endl;
                        cgn.edges.push_back(indexOfChildX);
                    }

                    // Also a node where each of the agents does not move but stays on it's place -- A does not move here
                    int indexOfChildY = combine2Graphs(stepsTaken + 1, cost, currentA, edgeB, finishA, finishB, newVisitedA, newVisitedB, g1, g2, cg);

                    if (indexOfChildY >= 0)
                    {
                        if (verbose)
                            cout << "adding edge from (" << currentA << ", " << currentB << ") to (" << cg->nodes[stepsTaken + 1][indexOfChildY].idList.front() << ", " << cg->nodes[stepsTaken + 1][indexOfChildY].idList.back() << ")" << endl;
                        cgn.edges.push_back(indexOfChildY);
                    }

                    // Also a node where each of the agents does not move but stays on it's place -- B does not move here
                    int indexOfChildZ = combine2Graphs(stepsTaken + 1, cost, edgeA, currentB, finishA, finishB, newVisitedA, newVisitedB, g1, g2, cg);

                    if (indexOfChildZ >= 0)
                    {
                        if (verbose)
                            cout << "adding edge from (" << currentA << ", " << currentB << ") to (" << cg->nodes[stepsTaken + 1][indexOfChildZ].idList.front() << ", " << cg->nodes[stepsTaken + 1][indexOfChildZ].idList.back() << ")" << endl;
                        cgn.edges.push_back(indexOfChildZ);
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