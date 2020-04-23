#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

// Agent.cpp
class Agent {
    public:
        int start;
        int end;
        vector<vector<int>> paths;
        Agent(int start, int end);
        void setPaths(vector<vector<int>> paths);
};

vector<Agent> importAgents(string filename);