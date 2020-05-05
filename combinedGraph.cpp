#include "combinedGraph.h"

// assuming size of vectors is equal
bool vectorEquals(vector<int> vecA, vector<int> vecB)
{
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
    for (int i : vec)
        if (i == x)
            return true;
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

// combines graph, deflects conflicing node pairs
void CombinedGraph::addSingleGraphToCombinedGraph(int stepsTaken, int cost, int graphListIndex, int currentCG, int currentG, vector<int> finishCG, int finishG, vector<vector<bool>> visitedCG, bool *visitedG, int maxNodes, CombinedGraph *cg, Graph *g, bool repeatCG, vector<int> repeatedIdList)
{
    // if we are out of steps, stop building the graph
    if (stepsTaken > cost)
    {
        return;
    }

    // if the nodes are not useful, stop building the graph
    if (!g->nodes[currentG].useful || !cg->nodes[graphListIndex][currentCG].useful)
    {
        return;
    }

    // if they use the same node at this point in time we have a conflict and we can't use this path
    // second condition is for when the CG is at it's endpoint and the single graph wants to use one of the endpoint nodes
    if (vectorContains(cg->nodes[graphListIndex][currentCG].idList, currentG) || (repeatCG && vectorContains(repeatedIdList, currentG)))
    {
        return;
    }

    visitedG[currentG] = true;
    visitedCG.at(graphListIndex).at(currentCG) = true;

    // Add current node in single graph to corresponding place in combinedgraph nodes matrix

    // These three will only be used if repeatCG is true
    CombinedGraphNode cgn = CombinedGraphNode();
    int cgnIndex;

    // If cg was done in previous call but g not yet, we will repeat the CombinedGraphNode (cgn).
    // to do this we need to copy the ids from the previous cgn to the idlist of the current cgn
    //      and then add the id of the current node in the single graph to it
    if (repeatCG)
    {
        for (int i : repeatedIdList)
        {
            cgn.idList.push_back(i);
        }
        cgn.idList.push_back(currentG);
    }
    else
    {
        cg->nodes[graphListIndex][currentCG].idList.push_back(currentG);
    }

    // if one of them is already at the final node, allow it to stay there until the other one takes its last steps
    bool gIsDone = currentG == finishG;
    bool cgIsDone = vectorEquals(finishCG, cg->nodes[graphListIndex][currentCG].idList);
    bool thisIsTheFinalStep = stepsTaken == cost;

    vector<int> edgesG, edgesCG;

    if (gIsDone && cgIsDone && thisIsTheFinalStep)
    {
        if (repeatCG)
        {
            cg->nodes[graphListIndex].push_back(cgn);
            return;
        }
        else
        {
            int indexInMatrix = cg->nodes[graphListIndex].size();
            cg->nodes[graphListIndex].push_back(cgn);
            return;
        }
    }
    else if (gIsDone && !cgIsDone)
    {
        edgesG.push_back(currentG);
        visitedG[currentG] = false;
        edgesCG = cg->nodes[graphListIndex][currentCG].edges;
    }
    else if (!gIsDone && cgIsDone)
    {
        repeatCG = true;
        edgesG = g->nodes[currentG].edges;
        // copy current CGN to next slot in node matrix,
        // put reference to it in edgesCG
        cg->nodes[graphListIndex + 1].push_back(cgn);
        edgesCG.push_back(cg->nodes[graphListIndex + 1].size() - 1);
        visitedCG[graphListIndex + 1][cg->nodes[graphListIndex + 1].size() - 1] = false;
    }
    else
    {
        edgesCG = cg->nodes[graphListIndex][currentCG].edges;
        edgesG = g->nodes[currentG].edges;
    }

    // make recursive calls
    for (int edgeG : edgesG)
    {
        for (int edgeCG : edgesCG)
        {
            if (!visitedG[edgeG] && !visitedCG.at(graphListIndex + 1).at(edgeCG))
            {
                // 2 agents cannot use the same edge from opposite sides at the same time
                if (!usingTheSameEdge(cg->nodes[graphListIndex][currentCG].idList, cg->nodes[graphListIndex + 1][edgeCG].idList, currentG, edgeG))
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
                            temp.push_back(visitedCG.at(k).at(j));
                        }
                        newVisitedCG.push_back(temp);
                    }

                    vector<int> newRepeatedIdList;
                    for (int i : cg->nodes[graphListIndex][currentCG].idList)
                    {
                        newRepeatedIdList.push_back(i);
                    }

                    // the normal call in which both next edges are new and the graphList is advanced
                    addSingleGraphToCombinedGraph(stepsTaken + 1, cost, graphListIndex + 1, edgeCG, edgeG, finishCG, finishG, newVisitedCG, newVisitedG, maxNodes, cg, g, repeatCG, newRepeatedIdList);

                    // Also a call where one of the agents does not move but stays on it's place -- CG does not move here
                    addSingleGraphToCombinedGraph(stepsTaken + 1, cost, graphListIndex, currentG, edgeG, finishCG, finishG, newVisitedCG, newVisitedG, maxNodes, cg, g, repeatCG, newRepeatedIdList);

                    // Also a node where each of the agents does not move but stays on it's place -- G does not move here
                    addSingleGraphToCombinedGraph(stepsTaken + 1, cost, graphListIndex + 1, edgeCG, currentG, finishCG, finishG, newVisitedCG, newVisitedG, maxNodes, cg, g, repeatCG, newRepeatedIdList);
                }
            }
        }
    }
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

    for (auto agent : agentList)
    {
        endList.push_back(agent.end);
    }
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
    }
    if (agentList.size() > 2)
    {
        for (int i = 2; i < agentList.size(); i++)
        {
            int cost;
            for (int j : optimalCostList)
                if (j > cost)
                    cost = j;

            vector<int> finishCG;
            for (int j = 0; j < i; j++)
            {
                finishCG.push_back(agentList[j].end);
            }

            int maxNodes = 0;
            for (int j = 0; j < cost + 1; j++)
            {
                int size = cg->nodes[j].size();
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
            cg->addSingleGraphToCombinedGraph(0, cost, 0, 0, agentList[i].start, finishCG, agentList[i].end, newVisitedCG, newVisitedG, maxNodes, cg, &agentList[i].graph, false, emptyList);

            printCombinedGraph(cg, cost);
        }
    }

    // print results
}