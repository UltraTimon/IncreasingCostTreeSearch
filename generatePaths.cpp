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
MDD Graph::generateAllPaths(int s, int d, int exactCost, vector<MDD> & endPoints)
{
    // Initialize all vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    MDD startMDD = MDD(s);

    // Call the recursive helper function to print all paths
    return generateAllPathsRecursivePart(s, d, visited, exactCost, endPoints, startMDD);
}

// DFS path builder that recursively walks through the graph, building a tree of MDD objects which will be later converted to paths
MDD Graph::generateAllPathsRecursivePart(int currentNode, int endNode, bool visited[], int numberOfStepsAllowed, vector<MDD> endPoints, MDD & previousMDD)
{
    // Mark the current node as visited
    visited[currentNode] = true;

    if(numberOfStepsAllowed < 0) {
        // Remove current vertex from path[] and mark it as unvisited
        return previousMDD;
    }

    // create MDD object for current Node to store parent and children
    MDD currentMDD = MDD(currentNode);
    currentMDD.parent = &previousMDD;

    // If current vertex is same as destination and we're at exactly the right amount of steps, 
    // we're done. Add MDD to parents of dest and be done
    if (currentNode == endNode && numberOfStepsAllowed == 0)
    {
        // set current flag to true
        currentMDD.pathLeadsToDestination = true;

        // set currentMDD as one of the endpoints s.t. we can trace back from it later on
        endPoints.push_back(currentMDD);

        // return currentMDD s.t. its parent can set its flag to true and add this node to its children
        return currentMDD;
    }
    // If current vertex is not destination but we are allowed to make steps
    else if(currentNode != endNode && numberOfStepsAllowed > 0) 
    {
        // for each adjacent vertex ...
        for (int i : adj[currentNode]) {
            // if we did not visit it yet
            if (!visited[i]) {
                // generate the MDD for the child node, storing all child nodes in the child list of the generated MDD...
                MDD child = generateAllPathsRecursivePart(i, endNode, visited, numberOfStepsAllowed - 1, endpoints, currentMDD);
                // ... but only if it leads to the destination. Otherwise we don't need that path, so we don't need this child
                if(child.pathLeadsToDestination) {
                    currentMDD.children.push_back(child);
                    currentMDD.pathLeadsToDestination = true;
                    child.parent = &currentMDD;
                    // return the current MDD with its child add to its list
                }
            }
        }
        return currentMDD;
    } 
    // We're out of steps and current Node is not the endNode. Return currentMDD but without the flag enabled
    // This way it will be ignored at the parent
    else {
        visited[currentNode] = false;
        return currentMDD;
    }
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


    vector<MDD> endPoints;
    MDD mdd = g.generateAllPaths(start, end, exactCost, endPoints);
    cout << "-- #endPoints: " << endPoints.size() << endl;

    vector<vector<int>> paths;
    convertMDDToPathList(start, endPoints, paths);
    cout << "mdd generated, #paths: " << paths.size() << endl;

    return paths;
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