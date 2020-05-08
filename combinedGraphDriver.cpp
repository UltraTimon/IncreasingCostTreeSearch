#include "combinedGraph.h"

void createInitialCombinedGraph(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg)
{
    Graph *g1 = &agentList[0].graph;
    Graph *g2 = &agentList[1].graph;

    bool visitedA[g1->nodes.size()];
    bool visitedB[g2->nodes.size()];

    for (int i = 0; i < g1->nodes.size(); i++)
    {
        visitedA[i] = false;
    }
    for (int i = 0; i < g2->nodes.size(); i++)
    {
        visitedB[i] = false;
    }

    int startA = agentList[0].start;
    int startB = agentList[1].start;

    int finishA = agentList[0].end;
    int finishB = agentList[1].end;

    int cost = max(optimalCostList[0], optimalCostList[1]);

    cg->combine2Graphs(0, cost, startA, startB, finishA, finishB, visitedA, visitedB, g1, g2, cg);

    vector<int> endList;
    endList.push_back(agentList[0].end);
    endList.push_back(agentList[1].end);

    int tempMax = 0;
    for (int i = 0; i < cost + 1; i++)
    {
        int size = cg->nodes[i].size();
        if (size > tempMax)
            tempMax = size;
    }
    int maxNodes = tempMax;

    vector<vector<bool>> newVisited;
    for (int k = 0; k < cost + 1; k++)
    {
        vector<bool> temp;
        for (int j = 0; j < maxNodes; j++)
        {
            temp.push_back(false);
        }
        newVisited.push_back(temp);
    }

    cg->combinedNodeIsUseful(0, 0, endList, cost, cg, newVisited, cost, maxNodes);
}

void printCombinedGraph(CombinedGraph *cg, int cost)
{
    if (cg->nodes[0].front().useful && verbose)
    {
        cout << "id lists: " << endl;
        for (int i = 0; i <= cost; i++)
        {
            for (auto cgn : cg->nodes[i])
            {
                if (cgn.useful)
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
                if (cgn.useful)
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
    }
    cout << "-- Done printing graph" << endl;
}

void printCombinedGraphAll(CombinedGraph *cg, int cost)
{
    cout << "id lists: " << endl;
    for (int i = 0; i <= cost; i++)
    {
        for (auto cgn : cg->nodes[i])
        {
            for (auto id : cgn.idList)
            {
                cout << id << " ";
            }
            cout << " - ";
        }
        if (cg->nodes[i].size() > 0)
            cout << endl;
    }

    cout << "egde lists: " << endl;
    for (int i = 0; i <= cost; i++)
    {
        for (auto cgn : cg->nodes[i])
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
    cout << "-- Done printing graph" << endl;
}

void CombinedGraph::createCombinedgraph(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg)
{
    if (agentList.size() <= 1)
    {
        cout << "Come on, give me a challenge! I won't do it for less than 2 agents." << endl;
    }
    if (agentList.size() == 2)
    {
        createInitialCombinedGraph(agentList, optimalCostList, cg);
        int cost = 0;
        for (int j : optimalCostList)
            if (j > cost)
                cost = j;
    }
    if (agentList.size() > 2)
    {
        createInitialCombinedGraph(agentList, optimalCostList, cg);

        int cost = 0;
        for (int j : optimalCostList)
            if (j > cost)
                cost = j;

        printCombinedGraph(cg, cost);

        // run method for each extra agent
        for (int i = 2; i < agentList.size(); i++)
        {
            vector<int> finishCombinedG;
            printf("finish list; ");
            for (int j = 0; j < agentList.size(); j++)
            {
                finishCombinedG.push_back(agentList[j].end);
                cout << agentList[j].end << " ";
            }
            cout << endl;
            

            int maxNodes = 0;
            for (int j = 0; j < cost + 1; j++)
            {
                int size = cg->nodes[j].size() * 3;
                if (size > maxNodes)
                    maxNodes = size;
            }

            vector<vector<bool>> newVisitedCG;
            for (int k = 0; k < cost + 1; k++)
            {
                vector<bool> temp;
                for (int j = 0; j < maxNodes; j++)
                {
                    temp.push_back(false);
                }
                newVisitedCG.push_back(temp);
            }

            bool newVisitedG[agentList[i].graph.nodes.size()];

            for (int j = 0; j < agentList[j].graph.nodes.size(); j++)
            {
                newVisitedG[j] = false;
            }

            vector<int> emptyList;

            CombinedGraph newCG = CombinedGraph(cost);
            cg->copyOldCombinedNodeToNewCombinedNodeWithSingleGraphNodeIncluded(cost, 0, 0, cg, agentList[i].start, &agentList[i].graph, &newCG);
            
            combinedNodeIsUseful(0, 0, finishCombinedG, cost, &newCG, newVisitedCG, cost, maxNodes);
            printCombinedGraphAll(&newCG, cost);
        }
    }

    // print results
}