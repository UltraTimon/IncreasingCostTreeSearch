// C++ program to print all paths from a source to destination.
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

// #include "generatePaths.h"
#include "mdd.h"

// A directed graph using adjacency vector representation
class Graph
{
    int V;               // No. of vertices in graph
    vector<int> *adj;      // Pointer to an array containing adjacency vectors
    int pathCounter = 0; // index of which path is being filled up now

    // A recursive function used by generateAllPaths()
    MDD generateAllPathsRecursivePart(int, int, bool[], int[], int &, int start, int numberOfStepsAllowed, Destination dest);

public: 
    Graph(int V); // Constructor
    void addEdge(int u, int v);
    MDD generateAllPaths(int s, int d, int exactCost);
    vector<vector<int>> pathsTaken; // vector of paths nodes that are traversed
};