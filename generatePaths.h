#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include "agent.h"

// generatePaths.cpp
list<list<int>> generatePaths(string filename, int start, int end, int exactCost);

list<int> calculateOptimalCost(list<Agent> agentList);