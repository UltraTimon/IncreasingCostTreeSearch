#include "generatePaths.h"
#include "basics.h"

// This method traverses the graph from the waypoint until the end node

// nodeIsUsefulWaypoint has to leave enough steps for this method to reach the end,
//  otherwise the cost will be increased
bool nodeIsUseful(int current, int end, int stepsLeft, Graph *g)
{
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
bool nodeIsUsefulWaypoint(int current, int end, int *stepsLeft, Graph *g)
{
    if (*stepsLeft < 0)
        return false;

    if (current == end)
    {
        g->nodes[current].usefulWaypoint = true;
        return true;
    }


    if (*stepsLeft > 0)
    {
        for (int i : g->nodes[current].edges)
        {
            *stepsLeft--;
            if (nodeIsUsefulWaypoint(i, end, stepsLeft, g))
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

// generate paths for each agent, add paths to agent object
// This will first only determine what the optimal cost is for every agent by calculating paths with an iteratively increasing cost until
//      each agent has at least one path with that cost
vector<int> calculateOptimalCost(vector<Agent> agentList)
{
    vector<int> optimalCostList;

    for (auto agent = agentList.begin(); agent != agentList.end(); ++agent)
    {
        int optimalCost = 1; //  assuming a minimal cost of 1 for each agent
        while (true)
        {
            int stepsLeft = optimalCost;
            bool atLeastOnePathToWaypoint = nodeIsUsefulWaypoint(agent->start, agent->waypoint, &stepsLeft, &agent->graph);
            
            if(atLeastOnePathToWaypoint) {
            // the first part reached the end
                bool atLeastOnePathWaypointToEnd = nodeIsUseful(agent->waypoint, agent->end, stepsLeft, &agent->graph);
                
                // the second part reached the end
                if (atLeastOnePathWaypointToEnd)
                {
                    optimalCostList.push_back(optimalCost);
                    break;
                }
            }
            else
            {
                optimalCost++;
            }
        }
    }

    return optimalCostList;
}