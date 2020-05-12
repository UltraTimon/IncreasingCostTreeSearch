#include "generatePaths.h"
#include "basics.h"

// This method traverses the graph from the waypoint until the end node

// nodeIsUsefulWaypoint has to leave enough steps for this method to reach the end,
//  otherwise the cost will be increased
bool nodeIsUseful(int current, int end, int stepsLeft, Graph *g)
{
    printf("standard niu, current: %d, end: %d, stepsLeft: %d\n", current, end, stepsLeft);
    if (stepsLeft < 0)
        return false;

    if (current == end)
    {
        g->nodes[current].useful = true;
        return true;
    }


    if (stepsLeft > 0)
    {
        for (int i : g->nodes[current].edges)
        {
            if (nodeIsUseful(i, end, stepsLeft - 1, g))
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
bool nodeIsUsefulWaypoint(int current, int end, int stepsLeft, Graph *g)
{
    // TESTING
    g->stepsLeft = 5;
    return true;

    // printf("niu, current: %d, end: %d, stepsLeft: %d\n", current, end, stepsLeft);

    if (stepsLeft < 0) {
        return false;
    }

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
            if (nodeIsUsefulWaypoint(i, end, stepsLeft - 1, g))
            {
                g->nodes[current].usefulWaypoint = true;
            }
        }
    }

    return g->nodes[current].usefulWaypoint;
}

// Obsolete
void getPathsFromGraphRecursivePart(int current, int end, int stepsLeft, Graph *g, bool visited[], deque<int> pathUpToNow, vector<vector<int>> *paths)
{
    if (stepsLeft < 0 || (stepsLeft == 0 && current != end))
    {
        // pathUpToNow.pop_back();
        return;
    }

    visited[current] = true;
    pathUpToNow.push_back(current);

    if (stepsLeft == 0 && current == end)
    {
        vector<int> temp;
        for (int i : pathUpToNow)
            temp.push_back(i);

        paths->push_back(temp);
        return;
    }

    if (stepsLeft > 0)
    {
        for (int i : g->nodes[current].edges)
        {
            if (g->nodes[i].useful && !visited[i])
            {
                // make deepcopy of visited array s.t. other paths can still be discovered
                bool *newVisited = new bool[g->nodes.size()];
                for (int j = 0; j < g->nodes.size(); j++)
                {
                    newVisited[j] = visited[j];
                }
                getPathsFromGraphRecursivePart(i, end, stepsLeft - 1, g, newVisited, pathUpToNow, paths);
            }
        }
    }
}

// Obsolete
void getPathsFromGraph(int start, int end, int exactCost, Graph *g, vector<vector<int>> *paths) {
    if(verbose)
        cout << "Getting paths from graph -- start: " << start << ", end: " << end << ", exactCost: " << exactCost << endl;

    bool visited[g->nodes.size()];
    for (int i = 0; i < g->nodes.size(); i++)
    {
        visited[i] = false;
    }
    deque<int> pathUpToNow;
    getPathsFromGraphRecursivePart(start, end, exactCost, g, visited, pathUpToNow, paths);
}
