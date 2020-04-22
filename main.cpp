#include "generatePaths.h"
#include "agent.h"
#include "lowlevelsearch.h"

using namespace std;

int main(int argc, char** argv) {
    int exactCost = atoi(argv[1]);

    // get list of agent objects read form file
    list<Agent> agentList = importAgents("resources/agents.txt");

    // generate paths for each agent, add paths to agent object
    // This will first only determine what the optimal cost is for every agent by calculating paths with an iteratively increasing cost until
    //      each agent has at least one path with that cost
    int optimalCost = 1;
    bool everyAgentHasAtLeastOnePath = false;
    list<Agent>::iterator agentIterator;

    while(!everyAgentHasAtLeastOnePath) {
        for(agentIterator = agentList.begin(); agentIterator != agentList.end(); ++agentIterator) {
            list<list<int>> generatedPaths = generatePaths("resources/graph.txt", agentIterator->start, agentIterator->end, optimalCost);
            if(generatedPaths.size() == 0) {
                optimalCost++;
                cout << "optimalCost increased to " << optimalCost << endl;
                break;
            } else {
                cout << "every agent has at least one path with length " << optimalCost << endl;
                everyAgentHasAtLeastOnePath = true;
            }
        }
    }

    // actually add the paths to the agent objects
    for(agentIterator = agentList.begin(); agentIterator != agentList.end(); ++agentIterator) {
        list<list<int>> generatedPaths = generatePaths("resources/graph.txt", agentIterator->start, agentIterator->end, optimalCost);
        agentIterator->setPaths(generatedPaths);
    }

    list<pair<list<int>, list<int>>> multiAgentPathPairs;
    list<Agent>::iterator masterIterator;
    list<Agent>::iterator subIterator;


    /*
        1) Does there exist a path of cost x?
        2) If yes, and we are going to examine this cost node, does there exist a path for each agent of that cost that do not conflict?
    */

    // Create ICT for up to 3 levels

    return 0;
}