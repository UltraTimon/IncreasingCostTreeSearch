#include "generatePaths.h"
#include "agent.h"

using namespace std;

int main() {
    list<Agent> agentList = importAgents("resources/agents.txt");

    list<Agent>::iterator it;
    for(it = agentList.begin(); it != agentList.end(); ++it) {
        list<list<int>> generatedPaths = generatePaths("resources/graph.txt", it->start, it->end);
        it->setPaths(generatedPaths);
    }

    int agentCounter = 0;
    for(auto agent : agentList) {
        cout << "Agent " << agentCounter << ":" << endl;
        agentCounter++;

        int printPathCounter = 0;
        for (auto list : agent.paths)
        {
            cout << "path " << printPathCounter << ": ";
            printPathCounter++;
            for (auto i : list)
            {
                cout << i << " ";
            }
            cout << endl;
        }
    }

    return 0;
}