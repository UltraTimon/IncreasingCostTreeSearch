// C++ program to print all paths from a source to destination.
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <deque>
using namespace std;

// #include "generatePaths.h"
#include "mdd.h"

// A directed graph using adjacency vector representation
class Graph
{
    int V;               // No. of vertices in graph
    vector<int> *adj;      // Pointer to an array containing adjacency vectors
    int pathCounter = 0; // index of which path is being filled up now
    MDD * generateAllPathsRecursivePart(int currentNode, int endNode, bool visited[], vector<int> *adjacentNodes, int numberOfStepsAllowed, vector<MDD> MDDList, int previousNode);
    MDD * makeRecursiveCallsToChildren(int currentNode, int endNode, bool visited[], vector<int> *adjacentNodes, int numberOfStepsAllowed, vector<MDD> MDDList, int currentNodeX);

public: 
    Graph(int V); // Constructor
    void addEdge(int u, int v);
    MDD generateAllPaths(int start, int end, int exactCost, vector<MDD> MDDList);
    vector<vector<int>> pathsTaken; // vector of paths nodes that are traversed
};