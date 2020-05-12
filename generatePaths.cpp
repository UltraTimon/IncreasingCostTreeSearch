#include "generatePaths.h"
#include "basics.h"

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
            bool atLeastOnePath = nodeIsUseful(agent->start, agent->end, optimalCost, &agent->graph);
            if (atLeastOnePath)
            {
                optimalCostList.push_back(optimalCost);
                break;
            }
            else
            {
                optimalCost++;
            }
        }
    }

    return optimalCostList;
}