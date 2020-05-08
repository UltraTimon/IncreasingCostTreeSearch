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
bool combinedNodeIsUseful(int current, int graphListIndex, vector<int> endIdList, int stepsLeft, CombinedGraph *g, vector<vector<bool>> visited, int cost, int maxNodes)
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
    printf("size of vec: %d\n", vec.size());
    for (int i = 0; i < vec.size(); i++) {
        printf("comparing %d to %d\n", vec.at(i), x);
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

int error_value = -1;

void copyPreviousCGNHelper(CombinedGraphNode *cgn, vector<int> previousIdList, vector<int> previousEdges)
{
    for (int i : previousIdList)
        cgn->idList.push_back(i);

    for (int i : previousEdges)
        cgn->edges.push_back(i);
}

// combines graph, deflects conflicing node pairs
int CombinedGraph::addSingleGraphToCombinedGraph(int stepsTaken, int cost, int graphListIndex, int currentCombinedGIndex, int currentSingleGIndex, vector<int> finishCombinedGIdList, int finishSingleGIndex, vector<vector<bool>> visitedCombinedG, bool *visitedSingleG, int maxNodes, CombinedGraph *cg, Graph *g, bool copyPreviousCGN, vector<int> previousIdList, vector<int> previousEdges)
{
    CombinedGraphNode currentCGN = cg->nodes[graphListIndex][currentCombinedGIndex];
    if (stepsTaken > cost || vectorContains(currentCGN.idList, currentSingleGIndex))
    {
        printf("bad input\n");
        return true;
    }



    bool singleGIsDone = currentSingleGIndex == finishSingleGIndex;
    bool combinedGIsDone = vectorEquals(finishCombinedGIdList, currentCGN.idList);

    if(stepsTaken == cost) {
        // we're done, let's decide how we end it
        if(singleGIsDone && combinedGIsDone) {
            currentCGN.idList.push_back(currentSingleGIndex);
            return currentCombinedGIndex;
        } else {
            return -1;
        }
    }

    vector<int> newPreviousIdList;
    vector<int> newPreviousEdges;
    if (copyPreviousCGN)
    {
        newPreviousIdList = previousIdList;
        newPreviousEdges = previousEdges;
    } 
    else
    {
        for (int i : currentCGN.idList)
            newPreviousIdList.push_back(i);
        for (int i : currentCGN.edges)
            newPreviousEdges.push_back(i);
    }


    CombinedGraphNode copyOfPreviousCGN = CombinedGraphNode();
    int indexOfCopy;
    if (copyPreviousCGN)
    {
        copyPreviousCGNHelper(&copyOfPreviousCGN, previousIdList, previousEdges);

        // set values s.t. the copy becomes current
        indexOfCopy = cg->nodes[graphListIndex].size();
        cg->nodes[graphListIndex].push_back(copyOfPreviousCGN);
        currentCGN = cg->nodes[graphListIndex][indexOfCopy];
    }

    // set current nodes to visited
    visitedSingleG[currentSingleGIndex] = true;
    visitedCombinedG.at(graphListIndex).at(currentCombinedGIndex) = true;

    // add current singleGNode to idList
    currentCGN.idList.push_back(currentSingleGIndex);


    // make recursive calls
    for (int edgeG : currentCGN.edges)
    {
        for (int edgeCG : g->nodes[currentSingleGIndex].edges)
        {
            if (!visitedSingleG[edgeG] && !visitedCombinedG.at(graphListIndex + 1).at(edgeCG))
            {
                // 2 agents cannot use the same edge from opposite sides at the same time
                if (!usingTheSameEdge(newPreviousIdList, cg->nodes[graphListIndex + 1][edgeCG].idList, currentSingleGIndex, edgeG))
                {
                    // make deepcopy of visited arrays to allow for weird loopy paths
                    bool *newVisitedG = new bool[g->nodes.size()];
                    for (int j = 0; j < g->nodes.size(); j++)
                    {
                        newVisitedG[j] = newVisitedG[j];
                    }
                    vector<vector<bool>> newVisitedCG;
                    for (int k = 0; k < cost + 1; k++)
                    {
                        vector<bool> temp;
                        for (int j = 0; j < maxNodes; j++)
                        {
                            temp.push_back(visitedCombinedG.at(k).at(j));
                        }
                        newVisitedCG.push_back(temp);
                    }

                    // the normal call in which both next edges are new and the graphList is advanced
                    int resX = addSingleGraphToCombinedGraph(stepsTaken + 1, cost, graphListIndex + 1, edgeCG, edgeG, finishCombinedGIdList, finishSingleGIndex, newVisitedCG, newVisitedG, maxNodes, cg, g, false, newPreviousIdList, newPreviousEdges);
                    if (resX >= 0 && resX >= currentCGN.edges.size())
                    {
                        cg->nodes[graphListIndex][currentCombinedGIndex].edges.push_back(resX);
                    }
                }
            }
        }
    }

    return -1;
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

    CombinedGraphNode cgn = CombinedGraphNode();
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

void createInitialCombinedGraph(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg)
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

    int cost = max(optimalCostList[0], optimalCostList[1]);

    cg->combine2Graphs(0, cost, startA, startB, finishA, finishB, visitedA, visitedB, g1, g2, cg);

    vector<int> endList;
    endList.push_back(agentList[0].end);
    endList.push_back(agentList[1].end);

    int tempMax = 0;
    for (int i = 0; i < cost + 1; i++)
    {
        int size = cg->nodes[i].size();
        if (size > tempMax)
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
}

void printCombinedGraph(CombinedGraph *cg, int cost)
{
    if (cg->nodes[0].front().useful && verbose)
    {
        cout << "id lists: " << endl;
        for (int i = 0; i <= cost; i++)
        {
            for (auto cgn : cg->nodes[i])
            {
                if (cgn.useful)
                {
                    for (auto id : cgn.idList)
                    {
                        cout << id << " ";
                    }
                    cout << " - ";
                }
            }
            if (cg->nodes[i].size() > 0)
                cout << endl;
        }

        cout << "egde lists: " << endl;
        for (int i = 0; i <= cost; i++)
        {
            for (auto cgn : cg->nodes[i])
            {
                if (cgn.useful)
                {
                    cout << "(";
                    for (int x : cgn.idList)
                        cout << x << " ";
                    cout << "): ";

                    for (auto edge : cgn.edges)
                    {
                        cout << "(";
                        for (int id : cg->nodes[i + 1][edge].idList)
                            cout << id << " ";
                        cout << ") - ";
                    }
                    cout << endl;
                }
            }
        }
    }
    cout << "-- Done printing graph" << endl;
}

void CombinedGraph::createCombinedgraph(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg)
{
    if (agentList.size() <= 1)
    {
        cout << "Come on, give me a challenge! I won't do it for less than 2 agents." << endl;
    }
    if (agentList.size() == 2)
    {
        createInitialCombinedGraph(agentList, optimalCostList, cg);
        int cost = 0;
        for (int j : optimalCostList)
            if (j > cost)
                cost = j;
    }
    if (agentList.size() > 2)
    {
        createInitialCombinedGraph(agentList, optimalCostList, cg);
        int cost = 0;
        for (int j : optimalCostList)
            if (j > cost)
                cost = j;

        for (int i = 2; i < agentList.size(); i++)
        {
            vector<int> finishCG;
            for (int j = 0; j < i; j++)
            {
                finishCG.push_back(agentList[j].end);
            }

            int maxNodes = 0;
            for (int j = 0; j < cost + 1; j++)
            {
                int size = cg->nodes[j].size() * 3;
                if (size > maxNodes)
                    maxNodes = size;
            }

            vector<vector<bool>> newVisitedCG;
            for (int k = 0; k < cost + 1; k++)
            {
                vector<bool> temp;
                for (int j = 0; j < maxNodes; j++)
                {
                    temp.push_back(false);
                }
                newVisitedCG.push_back(temp);
            }

            bool newVisitedG[agentList[i].graph.nodes.size()];

            for (int j = 0; j < agentList[j].graph.nodes.size(); j++)
            {
                newVisitedG[j] = false;
            }

            vector<int> emptyList;
           
            printf("agent start values: start: %d, end: %d\n", agentList[i].start, agentList[i].end);
            printf("initial id list: %d, %d\n", cg->nodes[0][0].idList.front(), cg->nodes[0][0].idList.back());
            cg->addSingleGraphToCombinedGraph(0, cost, 0, 0, agentList[i].start, finishCG, agentList[i].end, newVisitedCG, newVisitedG, maxNodes, cg, &agentList[i].graph, false, emptyList, emptyList);
        }
    }

    // print results
}