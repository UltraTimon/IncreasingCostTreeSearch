#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "generatePaths.h"

using namespace std;

bool pathsHaveConflict(list<int> pathA, list<int> pathB);

list<list<list<int>>> getAtLeastOnePathPerAgentWithoutConflict(list<Agent> agentList, int optimalCost);