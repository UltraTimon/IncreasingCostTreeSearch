#ifndef GENPATHS_H
#define GENPATHS_H

#include "basics.h"
#include "graph.h"

// generatePaths.cpp

void getPathsFromGraph(int start, int end, int exactCost, Graph *g, vector<vector<int>> *paths);

vector<int> calculateOptimalCost(vector<Agent> agentList);

bool nodeIsUseful(int current, int end, int stepsLeft, Graph *g);

#endif