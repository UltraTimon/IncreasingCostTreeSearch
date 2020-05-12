#include "combinedGraph.h"

void printCombinedGraph(CombinedGraph *cg, int cost, bool alsoIncludeNonUsefulNodes)
{
    if(alsoIncludeNonUsefulNodes) {
        cout << "GRAPH (all nodes):" << endl;
    } else {
        cout << "GRAPH (only useful nodes):" << endl;
    }

    cout << "id lists: " << endl;
    for (int i = 0; i <= cost; i++)
    {
        for (auto cgn : cg->nodes[i])
        {
            if (cgn.useful || alsoIncludeNonUsefulNodes)
            {
                for (auto id : cgn.idList)
                {
                    cout << id << " ";
                }
                cout << " - ";
            }
        }
        if (cg->nodes[i].size() > 0)
            cout << endl;
    }

    cout << "egde lists: " << endl;
    for (int i = 0; i <= cost; i++)
    {
        for (auto cgn : cg->nodes[i])
        {
            if (cgn.useful || alsoIncludeNonUsefulNodes)
            {
                cout << "(";
                for (int x : cgn.idList)
                    cout << x << " ";
                cout << "): ";

                for (auto edge : cgn.edges)
                {
                    cout << "(";
                    for (int id : cg->nodes[i + 1][edge].idList)
                        cout << id << " ";
                    cout << ") - ";
                }
                cout << endl;
            }
        }
    }
    cout << "-- Done printing graph" << endl;
}

void createCombinedGraph(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg)
{
    Graph *g1 = &agentList[0].graph;
    Graph *g2 = &agentList[1].graph;

    int startA = agentList[0].start;
    int startB = agentList[1].start;

    vector<int> waypointsA = agentList[0].waypoints;
    vector<int> waypointsB = agentList[1].waypoints;

    int finishA = agentList[0].end;
    int finishB = agentList[1].end;

    int cost = max(optimalCostList[0], optimalCostList[1]);

    vector<bool> reachedWaypointsA, reachedWaypointsB;
    for (int i = 0; i < waypointsA.size(); i++)
    {
        reachedWaypointsA.push_back(false);
        reachedWaypointsB.push_back(false);
    }
    

    cg->combine2Graphs(0, cost, startA, startB, 0, 0, waypointsA, waypointsB, false, false, finishA, finishB, g1, g2, cg);

    vector<int> endList;
    endList.push_back(agentList[0].end);
    endList.push_back(agentList[1].end);

    int maxNodes = 0;
    for (int i = 0; i < cost + 1; i++)
    {
        int size = cg->nodes[i].size();
        if (size > maxNodes)
            maxNodes = size;
    }

    if(cg->nodes[0].empty())
        return;

    // Remove illegal edges s.t. they do not help nodes become useful while they use an illegal edge
    cg->removeIllegalEdges(cg, 0, 0, cost);

    cg->combinedNodeIsUseful(0, 0, endList, cost, cg);
}

// ASSUMPTION: #agents >= 2
CombinedGraph CombinedGraph::createCombinedgraph(vector<Agent> agentList, vector<int> optimalCostList, int maxCost)
{

    CombinedGraph *cg = new CombinedGraph(maxCost);

    createCombinedGraph(agentList, optimalCostList, cg);

    if(verbose) {
        bool includeAlsoUselessEdges = !cg->nodes[0][0].useful;
        printCombinedGraph(cg, maxCost, includeAlsoUselessEdges);
    }

    return *cg;
}