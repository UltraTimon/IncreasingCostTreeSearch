#include "graph.h"

Graph::Graph(int V)
{
    this->V = V;
    adj = new vector<int>[V];
    this->pathCounter = 0;
}

void Graph::addEdge(int u, int v)
{
    adj[u].push_back(v); // Add v to u’s vector.
}

// Prints all paths from 's' to 'd'
void Graph::generateAllPaths(int s, int d, int exactCost)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];

    // Initialize all vertices as not visited
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // MDD that serves as endpoint
    Destination dest = Destination(d);

    // Create an array to store paths
    int *path = new int[V];
    int indexOfCurrentPath = 0; // Initialize path[] as empty

    // Call the recursive helper function to print all paths
    generateAllPathsRecursivePart(s, d, visited, path, indexOfCurrentPath, s, exactCost, dest);
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
MDD Graph::generateAllPathsRecursivePart(int u, int d, bool visited[],
                              int path[], int &path_index, int start, int numberOfStepsAllowed, Destination dest)
{
    // check if we are allowed another step
    if(numberOfStepsAllowed < 0)
        return;

    // Mark the current node and store it in path[]
    visited[u] = true;
    path[path_index] = u;
    path_index++;
    MDD newMDD = MDD(u);

    // If current vertex is same as destination and we're at exactly the right amount of steps, 
    // we're done. Add MDD to parents of dest and be done
    if (u == d && numberOfStepsAllowed == 0)
    {
        // add a new vector
        vector<int> newList;
        pathsTaken.push_back(newList);
        // print elements & add to vector of vectors
        for (int i = 0; i < path_index; i++)
        {
            pathsTaken.back().push_back(path[i]);
        }
        pathCounter++;

        // Add newMDD to list of parents of dest since we ended up at dest via newMDD
        dest.parents.push_back(newMDD);
        // Set flag to true so previous parents know they're important
        newMDD.pathLeadsToDestination = true;
        return newMDD;
    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
        vector<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i) {
            if (!visited[*i]) {
                MDD child = generateAllPathsRecursivePart(*i, d, visited, path, path_index, start, numberOfStepsAllowed - 1, dest);
                if(child.pathLeadsToDestination) {
                    newMDD.children.push_back(child);
                }
            }
        }
    }

    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[u] = false;
    return newMDD;
}

// Driver program
vector<vector<int>> generatePaths(string filename, int start, int end, int exactCost)
{

    // Reading in the edges from a file
    ifstream myfile(filename);
    int nodeA, nodeB; 
    int nrOfNodes = 999;
    if (myfile.is_open())
    {
        myfile >> nrOfNodes;
 
    }
    Graph g(nrOfNodes);
    if (myfile.is_open())
    {
        while (myfile >> nodeA >> nodeB)
        {
            g.addEdge(nodeA, nodeB);
            g.addEdge(nodeB, nodeA);
        }
        myfile.close();
    }
    else
    {
        cout << "generatePaths: Unable to open file. Sorry" << endl;
    }

    g.generateAllPaths(start, end, exactCost);

    return g.pathsTaken;
}

// generate paths for each agent, add paths to agent object
// This will first only determine what the optimal cost is for every agent by calculating paths with an iteratively increasing cost until
//      each agent has at least one path with that cost
vector<int> calculateOptimalCost(vector<Agent> agentList) {
    vector<int> optimalCostList;

    for (auto agentIterator = agentList.begin(); agentIterator != agentList.end(); ++agentIterator)
    {
        int optimalCost = 1; //  assuming a minimal cost of 1 for each agent
        while(true) {
            vector<vector<int>> generatedPaths = generatePaths("resources/graph.txt", agentIterator->start, agentIterator->end, optimalCost);
            if (generatedPaths.size() == 0)
            {
                optimalCost++;
            }
            else
            {
                optimalCostList.push_back(optimalCost);
                break;
            }
        }
    }


    return optimalCostList;
}