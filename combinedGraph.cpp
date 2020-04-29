#include "combinedGraph.h"

void CombinedGraph::combineGraph(int *current, int stepsLeft, vector<bool> *visited, deque<int> *pathUpToNow, vector<Graph*> graphList, 
    vector<CombinedGraphNode> *combinedGraphNodes, int previousCombinedGraphNode) {
    
    cout << "Hi from combineGraph!" << endl;
}

CombinedGraph::CombinedGraph(vector<Graph*> graphList, vector<Agent> agentList) {
    int nrOfNodes = graphList.front()->nodes.size();
    vector<bool> visited[graphList.size()];

    for (int i = 0; i < graphList.size(); i++)
    {
        for (int j; j < graphList.front()->nodes.size(); j++)
        {
            visited[i][j] = false;
        }
    }
    
    vector<CombinedGraphNode> combinedGraphNodes[graphList.size()];
    
    
    deque<int> pathUpToNow[graphList.size()];
    int current[graphList.size()];
    for (int i = 0; i < graphList.size(); i++)
    {
        current[i] = agentList[i].start;
    }
    

    combineGraph(current, -1, visited, pathUpToNow, graphList, combinedGraphNodes, -1);
}