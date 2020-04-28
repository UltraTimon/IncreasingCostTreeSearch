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

class GraphNode 
{
    public:
        int id;
        vector<int> edges;
        bool useful;
        GraphNode(int i) {
            id = i;
        }
};

class Graph
{
    public:
        vector<GraphNode> nodes;
        Graph(int nrOfNodes) {
            nodes = vector<GraphNode>(nrOfNodes);
            for (int i = 0; i < nrOfNodes; i++)
            {
                GraphNode node = GraphNode(i);
                nodes.push_back(node);
            }
        }
};