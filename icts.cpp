#include <iostream>
#include <list>
using namespace std;
//graph class for DFS travesal
class DFSGraph
{
    int V;                                        // No. of vertices
    list<int> *adjList;                           // adjacency list
    int DFS_util(int v, bool visited[], int end); // A function used by DFS
public:
    // class Constructor
    DFSGraph(int V)
    {
        this->V = V;
        adjList = new list<int>[V];
    }
    // function to add an edge to graph
    void addEdge(int v, int w)
    {
        adjList[v].push_back(w); // Add w to v’s list.
    }

    void DFS(int start, int end); // DFS traversal function
};
int DFSGraph::DFS_util(int v, bool visited[], int end)
{
    if (v == end)
    {
        cout << v << "\n";
        return 1;
    }

    // current node v is visited
    visited[v] = true;
    cout << v << " ";

    for (int i : adjList[v])
    {
        if (!visited[i])
            if (1 == DFS_util(i, visited, end))
                return 1;
    }
}

// DFS traversal
void DFSGraph::DFS(int start, int end)
{

    for (auto iter = adjList[start].begin(); iter != adjList[start].end(); iter++)
    {
        cout << *iter << endl;
    }

    // initially none of the vertices are visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // explore the
    for (int i = 0; i < 3; i++)
    {
        visited[0] = false;
        visited[3] = false;
        DFS_util(start, visited, end);
    }
}

int main()
{
    // Create a graph (will be created from file input later on)
    DFSGraph gdfs(5);
    gdfs.addEdge(0, 1);
    gdfs.addEdge(1, 3);
    gdfs.addEdge(0, 2);
    gdfs.addEdge(2, 3);
    gdfs.addEdge(0, 4);
    gdfs.addEdge(4, 3);

    cout << "Depth-first traversal for the given graph:" << endl;

    // here start and end are hardcoded, they will be based on the specific agent later on
    gdfs.DFS(0, 3);

    return 0;
}