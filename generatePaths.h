#ifndef GENPATHS_H
#define GENPATHS_H

#include "basics.h"
#include "graph.h"
#include "combinedGraph.h"

// generatePaths.cpp

void getPathsFromCombinedGraph(int exactCost, CombinedGraph *cg, vector<vector<int>> *pathsA, vector<vector<int>> *pathsB);

vector<int> calculateOptimalCost(vector<Agent> *agentList);

bool usefulWrapper(int current, vector<int> waypoints, int end, int stepsLeft, Graph *g);

#endif