#ifndef AGENT_H
#define AGENT_H

#include "basics.h"
#include "graph.h"

class Agent {
    public:
        int start;
        int end;
        vector<vector<int>> paths;
        Graph graph;
        Agent(int start, int end);
        void setPaths(vector<vector<int>> paths);
};

vector<Agent> importAgents(string filename);

#endif