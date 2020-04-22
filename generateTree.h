#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "tree.h"

using namespace std;

Node generateRoot(int rootCost, int numberOfAgents);

list<Node> generateNextLevelOfChildren(Node tree);