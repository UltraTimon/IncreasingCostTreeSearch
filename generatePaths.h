#ifndef GENPATHS_H
#define GENPATHS_H

#include "basics.h"
#include "graph.h"

// generatePaths.cpp
bool generateUsefulGraph(Graph *g, int start, int end, int exactCost);

void getPathsFromGraph(int start, int end, int exactCost, Graph *g, vector<vector<int>> *paths);

vector<int> calculateOptimalCost(vector<Agent> agentList);

#endif