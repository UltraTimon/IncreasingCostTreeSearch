#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include "agent.h"

// generatePaths.cpp
vector<vector<int>> generatePaths(string filename, int start, int end, int exactCost);

vector<int> calculateOptimalCost(vector<Agent> agentList);