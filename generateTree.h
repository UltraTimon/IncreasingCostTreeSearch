#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "tree.h"

using namespace std;

Node generateRoot(vector<int> rootCostList);

vector<Node> generateNextLevelOfChildren(Node tree);

void generateNextLevel(Node *tree);

void printTree(Node *tree);

void printTree(Node *tree, int depth);