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

// Makes the recursive calls to adjacentNodes and, if unvisited and leading to end node, 
// adds them to the list of children and sets the flag to true
int * Graph::makeRecursiveCallsToChildren(int currentNode, int endNode, bool visited[], vector<int> *adjacentNodes, int numberOfStepsAllowed, vector<MDD> MDDList, int currentNodeX) {
    // for each adjacent vertex ...
    for (int i : *(adjacentNodes)) {
        // if we did not visit it yet
        if (!visited[i]) {
            // generate the MDD for the child node, storing all child nodes in the child list of the generated MDD...
            MDD *child = generateAllPathsRecursivePart(i, endNode, visited, adjacentNodes, numberOfStepsAllowed, MDDList, currentNodeX);
            // ... but only if it leads to the destination. Otherwise we don't need that path, so we don't need this child
            if(child->pathLeadsToDestination) {
                MDDList[currentNode].children.push_back(child->data);
                MDDList[currentNode].pathLeadsToDestination = true;
            }
        }
    }
}

// DFS path builder that recursively walks through the graph, building a tree of MDD objects which will be later converted to paths
int Graph::generateAllPathsRecursivePart(int currentNode, int endNode, bool visited[], vector<int> *adjacentNodes, int numberOfStepsAllowed, vector<MDD> MDDList, int previousNode)
{
    // Mark the current node as visited
    visited[currentNode] = true;
    cout << "currentNode: " << currentNode << endl;

    MDDList[currentNode].parent = &MDDList[previousNode];

    // If current vertex is same as destination and we're at exactly the right amount of steps, 
    // we're done. Add MDD to parents of dest and be done
    if (currentNode == endNode && numberOfStepsAllowed == 0)
    {
        // set current flag to true
        MDDList[currentNode].pathLeadsToDestination = true;

        cout << "currentNode was final node. parent: " << MDDList[currentNode].parent->data << endl;

        // return currentMDD s.t. its parent can set its flag to true and add this node to its children
        return MDDList[currentNode];
    }
    // If current vertex is not destination but we are allowed to make steps
    else if(currentNode != endNode && numberOfStepsAllowed > 0) 
    {
        cout << "hi" << endl;
        makeRecursiveCallsToChildren(currentNode, endNode, visited, adjacentNodes, numberOfStepsAllowed - 1, MDDList, &currentMDD);
        cout << "hi 2" << endl;

        // return the current MDD with its child added to its list
        return currentMDD;
    } 
    // We're out of steps and current Node is not the endNode. Return currentMDD but without the flag enabled
    // This way it will be ignored at the parent
    else {
        visited[currentNode] = false;
        return currentMDD;
    }
}


// Prints all paths from 's' to 'd'
MDD Graph::generateAllPaths(int start, int end, int exactCost, vector<MDD> MDDList)
{
    // Initialize all vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Call the recursive helper function to print all paths
    return generateAllPathsRecursivePart(start, end, visited, adj, exactCost, &MDDList, start);
}


// Driver program
void generatePaths(string filename, int start, int end, int exactCost, vector<vector<int>> *paths)
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

    // create list that holds MDD structures
    vector<MDD> MDDList;
    for (int i = 0; i < nrOfNodes; i++)
    {
        MDD newMDD = MDD(i);
        MDDList.push_back(newMDD);
    }

    // for testing: hardcoded value for exactCost
    exactCost = 3;
    MDD mdd = g.generateAllPaths(start, end, exactCost, &MDDList);
    cout << "-- after genAllPaths #endPoints: " << MDDList.size() << endl;

    MDD final = MDDList.back();
    cout << "endpoint: data: " << final.data << " flag: " << final.pathLeadsToDestination << " parent " << final.parent->data; // << " parent child " << final.parent->children.front().data << " #children: " << final.children.size() << endl; 

    convertMDDToPathList(start, &MDDList, paths);
    cout << "mdd generated, #paths: " << paths->size() << endl;
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