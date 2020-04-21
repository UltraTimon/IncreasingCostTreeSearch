#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// Agent.cpp
class Agent {
    public:
        int start;
        int end;
        list<list<int>> paths;
        Agent(int start, int end);
        void setPaths(list<list<int>> paths);
};

list<Agent> importAgents(string filename);