#include "header.h"

class Agent {
    public:
        int start;
        int end;
        list<list<int>> paths;

    public:
        Agent(int start, int end) {
            this->start = start;
            this->end = end;
        }

    public:
        void setPaths(list<list<int>> paths) {
            this->paths = paths;
        }
};

list<Agent> importAgents(string filename) {
    list<Agent> agentList;

    ifstream myfile(filename);
    int start, end;

    if (myfile.is_open())
    {
        while (myfile >> start >> end)
        {
            agentList.push_back(Agent(start, end));
        }
        myfile.close();
    }
    else
    {
        cout << "ImportAgents: Unable to open file. Sorry" << endl;
    }

    return agentList;
}