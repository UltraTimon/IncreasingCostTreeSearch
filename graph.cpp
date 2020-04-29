#include "graph.h"

Graph importGraph(string filename) {
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

    return g;
}