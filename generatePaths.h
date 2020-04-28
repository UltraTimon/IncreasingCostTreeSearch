#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include "agent.h"

// generatePaths.cpp
void generatePaths(string filename, int start, int end, int exactCost, vector<vector<int>> *paths);

vector<int> calculateOptimalCost(vector<Agent> agentList);