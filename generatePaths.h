#ifndef GENPATHS_H
#define GENPATHS_H

#include "basics.h"
#include "agent.h"
#include "graph.h"

// generatePaths.cpp
bool generatePaths(Graph *g, int start, int end, int exactCost, vector<vector<int>> *paths);

vector<int> calculateOptimalCost(vector<Agent> agentList, Graph *g);

#endif