#include "agent.h"

Agent::Agent(int givenStart, int givenEnd) {
    start = givenStart;
    end = givenEnd;
}

void Agent::setPaths(vector<vector<int>> givenPaths) {
    paths = givenPaths;
}


vector<Agent> importAgents(string filename) {
    vector<Agent> agentList;

    ifstream myfile(filename);
    int start, end;

    if (myfile.is_open())
    {
        while (myfile >> start >> end)
        {
            Agent newAgent = Agent(start, end);
            agentList.push_back(newAgent);
        }
        myfile.close();
    }
    else
    {
        cout << "ImportAgents: Unable to open file. Sorry" << endl;
    }

    return agentList;
}