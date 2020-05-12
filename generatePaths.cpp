#include "generatePaths.h"
#include "basics.h"
#include "combinedGraph.h"

// This method traverses the graph from the waypoint until the end node

// nodeIsUsefulWaypoint has to leave enough steps for this method to reach the end,
//  otherwise the cost will be increased
bool nodeIsUseful(int current, int end, int stepsLeft, bool *visited, Graph *g)
{
    if (stepsLeft < 0)
        return false;

    visited[current] = true;

    if (current == end)
    {
        g->nodes[current].useful = true;
        return true;
    }

    if (stepsLeft > 0)
    {
        for (int i : g->nodes[current].edges)
        {
            bool newVisited[g->nodes.size()];
            for (int i = 0; i < g->nodes.size(); i++)
            {
                newVisited[i] = visited[i];
            }

            if (!visited[i] && nodeIsUseful(i, end, stepsLeft - 1, newVisited, g))
            {
                g->nodes[current].useful = true;
            }
        }
    }

    return g->nodes[current].useful;
}


// This method marks nodes useful based on whether they are needed for the way to the waypoint
// This method traverses the graph from the start node to the waypoint node

// The number of stepsLeft is passed as a pointer s.t. the caller can keep track of the amount of steps that are left

// if nodeIsUsefulWaypoint is unable to reach the waypoint in the given amount of steps the cost will be increased

// if nodeIsUsefulWaypoint uses up too many steps of the total available amount but does reach the waypoint the second half
//      of the trip from the waypoint to the end will be impossible to calculate because there are not enough steps, thus
//      the cost will be increased
bool nodeIsUsefulWaypoint(int current, int end, int stepsLeft, bool *visited, Graph *g)
{
    if (stepsLeft < 0)
    {
        return false;
    }

    visited[current] = true;

    if (current == end)
    {
        g->nodes[current].usefulWaypoint = true;
        g->stepsLeft = stepsLeft;
        return true;
    }

    if (stepsLeft > 0)
    {
        for (int i : g->nodes[current].edges)
        {
            bool newVisited[g->nodes.size()];
            for (int i = 0; i < g->nodes.size(); i++)
            {
                newVisited[i] = visited[i];
            }

            if (!visited[i] && nodeIsUsefulWaypoint(i, end, stepsLeft - 1, newVisited, g))
            {
                g->nodes[current].usefulWaypoint = true;
            }
        }
    }

    return g->nodes[current].usefulWaypoint;
}

bool usefulWrapper(int current, int waypoint, int end, int stepsLeft, Graph *g)
{
    bool visited[g->nodes.size()];
    for (int i = 0; i < g->nodes.size(); i++)
    {
        visited[i] = false;
    }

    bool waypointReached = nodeIsUsefulWaypoint(current, waypoint, stepsLeft, visited, g);

    if (waypointReached)
    {
        bool visited[g->nodes.size()];
        for (int i = 0; i < g->nodes.size(); i++)
        {
            visited[i] = false;
        }
        int stepsLeftAfterWaypoint = g->stepsLeft;
        return nodeIsUseful(waypoint, end, stepsLeftAfterWaypoint, visited, g);
    }
}


// Asumption: 2 agents
void getPathsFromCombinedGraphRecursivePart(int graphListIndex, int currentGraphIndex, int end, CombinedGraph *cg, deque<int> pathUpToNowA, deque<int> pathUpToNowB, vector<vector<int>> *pathsA, vector<vector<int>> *pathsB)
{
    
    CombinedGraphNode currentNode = cg->nodes[graphListIndex][currentGraphIndex];


    pathUpToNowA.push_back(currentNode.idList.front());
    pathUpToNowB.push_back(currentNode.idList.back());

    if (graphListIndex == end)
    {
        vector<int> tempA;
        for (int i : pathUpToNowA)
            tempA.push_back(i);

        pathsA->push_back(tempA);

        vector<int> tempB;
        for (int i : pathUpToNowB)
            tempB.push_back(i);

        pathsB->push_back(tempB);
        return;
    }

    if (graphListIndex < end)
    {
        for (int i : cg->nodes[graphListIndex][currentGraphIndex].edges)
        {
            if (cg->nodes[graphListIndex + 1][i].useful)
            {
                getPathsFromCombinedGraphRecursivePart(graphListIndex + 1, i, end, cg, pathUpToNowA, pathUpToNowB, pathsA, pathsB);
            }
        }
    }
}

// Asumption: 2 agents
void getPathsFromCombinedGraph(int exactCost, CombinedGraph *cg, vector<vector<int>> *pathsA, vector<vector<int>> *pathsB)
{
    if (verbose)
        cout << "Getting paths from graph" << endl;

    deque<int> pathUpToNowA, pathUpToNowB;
    getPathsFromCombinedGraphRecursivePart(0, 0, exactCost, cg, pathUpToNowA, pathUpToNowB, pathsA, pathsB);
}
