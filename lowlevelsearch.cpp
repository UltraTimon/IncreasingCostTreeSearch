#include "lowlevelsearch.h"
#include "generatePaths.h"

/* 2 paths have a conflict if either:
    - they visit the same node at the same time, i.e. at most one agent may
        be at a node per time unit
    - one path uses an edge (a, b) at the same time another path 
        uses the edge (b, a), i.e. at most one agent may use an edge per time unit 
*/
bool pathsHaveConflict(vector<int> pathA, vector<int> pathB)
{
    vector<int>::iterator a, b;
    a = pathA.begin();
    b = pathB.begin();

    for (int i = 0; i < pathA.size(); i++)
    {
        // same node on same time
        if (*a == *b)
        {
            return true;
        }

        // same edge on same moment of transition
        vector<int>::iterator nextPositionOfA = next(a, 1);
        vector<int>::iterator nextPositionOfB = next(b, 1);

        if (*a == *nextPositionOfB && *b == *nextPositionOfA)
        {
            return true;
        }

        ++a;
        ++b;
    }
    return false;
}

// generate paths for each agent, add paths to agent object
// This will first only determine what the optimal cost is for every agent by calculating paths with an iteratively increasing cost until
//      each agent has at least one path with that cost
vector<int> calculateOptimalCost(vector<Agent> agentList)
{
    vector<int> optimalCostList;

    for (auto agent = agentList.begin(); agent != agentList.end(); ++agent)
    {
        int optimalCost = 1; //  assuming a minimal cost of 1 for each agent

        printf("agent with start %d waypoint %d end %d\n", agent->start, agent->waypoint, agent->end);

        while (true)
        {   
            if(usefulWrapper(agent->start, agent->waypoint, agent->end, optimalCost, &agent->graph)) {
                optimalCostList.push_back(optimalCost);
                break;
            }
            else
            {
                optimalCost++;
            }
        }
    }

    return optimalCostList;
}

// ASSUMPTIONS:
// - 2 agents
bool getAtLeastOnePathPerAgentWithoutConflict(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg, int maxCost)
{

    // generate a useful graph for each agent and add to graphList
    for (int i = 0; i < agentList.size(); i++)
    {
        usefulWrapper(agentList[i].start, agentList[i].waypoint, agentList[i].end, optimalCostList[i], &agentList[i].graph);

        // print useful nodes
        if(verbose) {
            printf("Useful nodes: ");
            for(auto node : agentList[i].graph.nodes) {
                if(node.useful)
                    cout << node.id << " ";
            }
            cout << endl;
        }
    }

    // combine those using the MDD magic from the paper and check for conflicts by running
    // modified nodeIsUseful method that marks nodes as useful based on whether or not they do or do not conflict
    // -> we need a special graph structure for this, CombinedGraph or something like that

    CombinedGraph finalCG = cg->createCombinedgraph(agentList, optimalCostList, maxCost);

    // if the combined source node has been marked as useful we know that there exists at least one path and we can return true
    // then we can add the paths to a given pointer that holds the path data (which is stored in ICTS() or even main()), and add it just like in getPathsFromGraph()

    // TODO:    delete cg somewhere here

    return finalCG.nodes[0].front().useful;

    // And then we're done... Sounds good, right?
}
