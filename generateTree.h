#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "tree.h"

using namespace std;

Node generateRoot(list<int> rootCostList);

list<Node> generateNextLevelOfChildren(Node tree);

void generateNextLevel(Node *tree);

void printTree(Node *tree);

void printTree(Node *tree, int depth);