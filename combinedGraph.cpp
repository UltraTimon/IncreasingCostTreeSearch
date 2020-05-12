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

int repeatA(int stepsTaken, int cost, int currentB, int waypointB, int reachedWaypointB, int finishA, int finishB, Graph *g1, Graph *g2, CombinedGraph *cg) {
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
    
    // check for waypoints reached, set bools
    if(!reachedWaypointB && currentB == waypointB)
        reachedWaypointB = true;

    // Check for all the usefuls based on whether or not waypoint has been reached and return -1 if one of them is not
    if(!reachedWaypointB && !g2->nodes[currentB].usefulWaypoint[0])
        return -1;

    if(reachedWaypointB && !g2->nodes[currentB].useful)
        return -1;

    // create the new node
    CombinedGraphNode cgn = CombinedGraphNode(g1->nodes.size());
    cgn.idList.push_back(finishA);
    cgn.idList.push_back(currentB);

    if(currentB == finishB && reachedWaypointB) {
        cg->nodes[stepsTaken].push_back(cgn);
        return cg->nodes[stepsTaken].size() - 1;
    }

    for(int edgeB : g2->nodes[currentB].edges) {
        if(!(finishA == edgeB && finishA == currentB)) {
            int indexOfChild = repeatA(stepsTaken + 1, cost, edgeB, waypointB, reachedWaypointB, finishA, finishB, g1, g2, cg);

            if (indexOfChild >= 0)
            {
                cgn.edges.push_back(indexOfChild);
            }
        }
    }

    cg->nodes[stepsTaken].push_back(cgn);
    return cg->nodes[stepsTaken].size() - 1;
}

int repeatB(int stepsTaken, int cost, int currentA, int waypointA, int reachedWaypointA, int finishA, int finishB, Graph *g1, Graph *g2, CombinedGraph *cg) {
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
    
    // check for waypoints reached, set bools
    if(!reachedWaypointA && currentA == waypointA)
        reachedWaypointA = true;

    // Check for all the usefuls based on whether or not waypoint has been reached and return -1 if one of them is not
    if(!reachedWaypointA && !g1->nodes[currentA].usefulWaypoint[0])
        return -1;

    if(reachedWaypointA && !g1->nodes[currentA].useful)
        return -1;

    // create the new node
    CombinedGraphNode cgn = CombinedGraphNode(g1->nodes.size());
    cgn.idList.push_back(finishB);
    cgn.idList.push_back(currentA);

    if(currentA == finishA && reachedWaypointA) {
        cg->nodes[stepsTaken].push_back(cgn);
        return cg->nodes[stepsTaken].size() - 1;
    }

    for(int edgeA : g1->nodes[currentA].edges) {
        if(!(finishB == edgeA && finishB == currentA)) {
            int indexOfChild = repeatA(stepsTaken + 1, cost, edgeA, waypointA, reachedWaypointA, finishA, finishB, g1, g2, cg);

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
int CombinedGraph::combine2Graphs(int stepsTaken, int cost, int currentA, int currentB, int indexWaypointA, int indexWaypointB, vector<int> waypointsA, vector<int> waypointsB, vector<bool> reachedWaypointA, vector<bool> reachedWaypointB, int finishA, int finishB, Graph *g1, Graph *g2, CombinedGraph *cg)
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

    // check for waypoints reached, set bools
    if(!reachedWaypointA[indexWaypointA] && currentA == waypointsA[indexWaypointA]) {
        reachedWaypointA[indexWaypointA] = true;
        
        if(indexWaypointA < waypointsA.size() - 1)
            indexWaypointA++;
    }

    if(!reachedWaypointB[indexWaypointB] && currentB == waypointsB[indexWaypointB]) {
        reachedWaypointB[indexWaypointB] = true;
        
        if(indexWaypointB < waypointsB.size() - 1)
            indexWaypointB++;
    }

    // Check for all the usefuls based on whether or not waypoint has been reached and return -1 if one of them is not
    if(!reachedWaypointA[indexWaypointA] && !g1->nodes[currentA].usefulWaypoint[indexWaypointA])
        return -1;
    if(!reachedWaypointB[indexWaypointB] && !g2->nodes[currentB].usefulWaypoint[indexWaypointB])
        return -1;

    if(reachedWaypointA[indexWaypointA] && !g1->nodes[currentA].useful)
        return -1;
    if(reachedWaypointB[indexWaypointB] && !g2->nodes[currentB].useful)
        return -1;


    if(currentA == finishA && reachedWaypointA[indexWaypointA])
        return repeatA(stepsTaken, cost, currentB, waypointB, reachedWaypointB, finishA, finishB, g1, g2, cg);

    if(currentB == finishB && reachedWaypointB[indexWaypointB])
        return repeatB(stepsTaken, cost, currentA, waypointA, reachedWaypointA, finishA, finishB, g1, g2, cg);

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
                int indexOfChild = combine2Graphs(stepsTaken + 1, cost, edgeA, edgeB, indexWaypointA, indexWaypointB, waypointsA, waypointsB, reachedWaypointA, reachedWaypointB, finishA, finishB, g1, g2, cg);

                if (indexOfChild >= 0) // check if child is useful and not conflicting
                {
                    cgn.edges.push_back(indexOfChild);
                }
            }
        }
    }

    cg->nodes[stepsTaken].push_back(cgn);
    return cg->nodes[stepsTaken].size() - 1;
}