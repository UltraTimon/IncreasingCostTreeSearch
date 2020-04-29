#ifndef LOWLVLSEARCH_H
#define LOWLVLSEARCH_H

#include "basics.h"
#include "generatePaths.h"

bool pathsHaveConflict(vector<int> pathA, vector<int> pathB);

void getAtLeastOnePathPerAgentWithoutConflict(vector<Agent> agentList, vector<int> optimalCostList);

#endif