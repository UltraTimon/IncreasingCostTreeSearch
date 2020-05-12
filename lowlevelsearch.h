#ifndef LOWLVLSEARCH_H
#define LOWLVLSEARCH_H

#include "basics.h"
#include "generatePaths.h"
#include "combinedGraph.h"

bool pathsHaveConflict(vector<int> pathA, vector<int> pathB);

CombinedGraph getAtLeastOnePathPerAgentWithoutConflict(vector<Agent> agentList, vector<int> optimalCostList, CombinedGraph *cg, int maxCost);

#endif