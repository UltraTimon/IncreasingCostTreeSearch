// C++ program to print all paths from a source to destination. 
#include<iostream> 
#include <list> 
#include <string>
#include <deque>
using namespace std; 
  
// A directed graph using adjacency list representation 
class Graph 
{ 
    int V; // No. of vertices in graph 
    list<int> *adj; // Pointer to an array containing adjacency lists 
    int pathCounter = 0; // index of which path is being filled up now
  
    // A recursive function used by printAllPaths() 
    void printAllPathsUtil(int , int , bool [], int [], int &, int start); 
  
public: 
    Graph(int V); // Constructor 
    void addEdge(int u, int v); 
    void printAllPaths(int s, int d); 
    list<list<int>> pathsTaken; // list of paths nodes that are traversed
}; 
  
Graph::Graph(int V) 
{ 
    this->V = V; 
    adj = new list<int>[V]; 
    this->pathCounter = 0;
} 
  
void Graph::addEdge(int u, int v) 
{ 
    adj[u].push_back(v); // Add v to u’s list. 
} 
  
// Prints all paths from 's' to 'd' 
void Graph::printAllPaths(int s, int d) 
{ 
    // Mark all the vertices as not visited 
    bool *visited = new bool[V]; 
  
    // Create an array to store paths 
    int *path = new int[V]; 
    int indexOfCurrentPath = 0; // Initialize path[] as empty 
  
    // Initialize all vertices as not visited 
    for (int i = 0; i < V; i++) 
        visited[i] = false; 
  
    // Call the recursive helper function to print all paths 
    printAllPathsUtil(s, d, visited, path, indexOfCurrentPath, s); 
} 
  
// A recursive function to print all paths from 'u' to 'd'. 
// visited[] keeps track of vertices in current path. 
// path[] stores actual vertices and path_index is current 
// index in path[] 
void Graph::printAllPathsUtil(int u, int d, bool visited[], 
                            int path[], int &path_index, int start) 
{ 
    // we're at the first node, so we're starting a new path. 
    // We add a new list to the list of lists so the nodes get added to the new list
    if(u == start) {
        list<int> newList;
        pathsTaken.push_back(newList);
        cout << "new list!" << endl;
    }

    // Mark the current node and store it in path[] 
    visited[u] = true; 
    path[path_index] = u; 
    path_index++; 
    cout << "visiting: " << u << endl;

  
    // If current vertex is same as destination, then print 
    // current path[] 
    if (u == d) 
    { 
        for (int i = 0; i < path_index; i++) {
            cout << path[i] << " "; 
            pathsTaken.back().push_back(path[i]);
        } 
        cout << endl; 
        pathCounter++;
    } 
    else // If current vertex is not destination 
    { 
        // Recur for all the vertices adjacent to current vertex 
        list<int>::iterator i; 
        for (i = adj[u].begin(); i != adj[u].end(); ++i) 
            if (!visited[*i]) 
                printAllPathsUtil(*i, d, visited, path, path_index, start); 
    } 
  
    // Remove current vertex from path[] and mark it as unvisited 
    path_index--; 
    visited[u] = false; 
} 
  
// Driver program 
int main() 
{ 
    // Create a graph given in the above diagram 
    Graph g(6); 
    g.addEdge(0, 1); 
    g.addEdge(1, 2); 
    g.addEdge(1, 3); 
    g.addEdge(2, 4); 
    g.addEdge(3, 4); 
    g.addEdge(4, 5); 
  
    int s = 0, d = 5; 
    cout << "Following are all different paths from " << s 
        << " to " << d << endl;

    g.printAllPaths(s, d); 

    cout << "all traversed nodes in a list: ";
    list<int>::iterator outerIterator;
    int printPathCounter = 0;
    for(auto list : g.pathsTaken) {
        cout << "path " << printPathCounter << ": ";
        printPathCounter++;
        for(auto i : list) {
            cout << i << " ";
        }
        cout << endl;
    }
  
    return 0; 
}