#include "graph.h"

bool nodeIsUseful(int current, int end, int stepsLeft, Graph *g, bool visited[])
{
    cout << "current: " << current << ", #stepsLeft: " << stepsLeft << endl;
    cout << "visited: " << endl;
    for (int i = 0; i < g->nodes.size(); i++)
    {
        cout << i << ": " << visited[i] << endl;
    }

    if (stepsLeft < 0 || (stepsLeft == 0 && current != end))
        return 0;

    visited[current] = true;

    if (stepsLeft == 0 && current == end)
    {
        g->nodes[current].useful = true;
        cout << current << " is useful and final." << endl;
        return true;
    }

    if (stepsLeft > 0)
    {
        for (int i : g->nodes[current].edges)
        {
            if (!visited[i])
            {
                cout << "call to " << i << endl;

                // make deepcopy of visited array s.t. other paths can still be discovered
                bool *newVisited = new bool[g->nodes.size()];
                for (int j = 0; j < g->nodes.size(); j++)
                {
                    newVisited[j] = visited[j];
                }

                if (nodeIsUseful(i, end, stepsLeft - 1, g, newVisited))
                {
                    g->nodes[current].useful = true;
                    cout << current << " is useful" << endl;
                }
            }
        }
    }

    if (g->nodes[current].useful)
    {
        return true;
    }
}

void getPathsFromGraph(int current, int end, int stepsLeft, Graph *g, bool visited[], deque<int> pathUpToNow, vector<vector<int>> *paths)
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
                getPathsFromGraph(i, end, stepsLeft - 1, g, newVisited, pathUpToNow, paths);
            }
        }
    }
}

// Generates paths from a given start to end node
// Returns true if at least one path is found, false otherwise
bool generatePaths(string filename, int start, int end, int exactCost, vector<vector<int>> *paths)
{
    // Reading in the edges from a file
    ifstream myfile(filename);
    int nodeA, nodeB;
    int nrOfNodes = 999;
    if (myfile.is_open())
    {
        myfile >> nrOfNodes;
    }
    Graph g = Graph(nrOfNodes);
    if (myfile.is_open())
    {
        while (myfile >> nodeA >> nodeB)
        {
            g.nodes[nodeA].edges.push_back(nodeB);
            g.nodes[nodeB].edges.push_back(nodeA);
        }
        myfile.close();
    }
    else
    {
        cout << "generatePaths: Unable to open file. Sorry" << endl;
    }

    // find paths
    bool visited[nrOfNodes];
    for (int i = 0; i < nrOfNodes; i++)
    {
        visited[i] = false;
    }
    nodeIsUseful(start, end, exactCost, &g, visited);

    cout << "all useful nodes: ";
    for (int i = 0; i < nrOfNodes; i++)
    {
        if (g.nodes[i].useful)
        {
            cout << i << " ";
        }
    }
    cout << endl;

    if (paths != 0)
    {
        bool visited[nrOfNodes];
        for (int i = 0; i < nrOfNodes; i++)
        {
            visited[i] = false;
        }
        deque<int> pathUpToNow;
        getPathsFromGraph(start, end, exactCost, &g, visited, pathUpToNow, paths);
    }

    // return result
    return g.nodes[start].useful;
}

// generate paths for each agent, add paths to agent object
// This will first only determine what the optimal cost is for every agent by calculating paths with an iteratively increasing cost until
//      each agent has at least one path with that cost
vector<int> calculateOptimalCost(vector<Agent> agentList)
{
    vector<int> optimalCostList;

    for (auto agentIterator = agentList.begin(); agentIterator != agentList.end(); ++agentIterator)
    {
        int optimalCost = 1; //  assuming a minimal cost of 1 for each agent
        while (true)
        {
            bool atLeastOnePath = generatePaths("resources/graph.txt", agentIterator->start, agentIterator->end, optimalCost, nullptr);
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