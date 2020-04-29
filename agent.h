#ifndef AGENT_H
#define AGENT_H

#include "basics.h"

class Agent {
    public:
        int start;
        int end;
        vector<vector<int>> paths;
        Agent(int start, int end);
        void setPaths(vector<vector<int>> paths);
};

vector<Agent> importAgents(string filename);

#endif